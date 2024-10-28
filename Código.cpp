#include <Wire.h>
#include <MPU6050.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

MPU6050 mpu;  // Instancia del sensor

// Configuración de la red Wi-Fi
const char* ssid = "ZTE Blade V30 Vita";
const char* password = "12345678";

// Crear el servidor web
AsyncWebServer server(80);

// Variables para la aceleración
float aceleracionX = 0.0;
float aceleracionY = 0.0;
float aceleracionZ = 0.0;


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Acelerometro ESP32</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    body { font-family: Arial; text-align: center; margin: 20px; }
    canvas { max-width: 600px; margin: 20px auto; }
    button { padding: 10px 20px; font-size: 16px; margin-top: 20px; }
  </style>
</head>
<body>
  <h1>Acelerometro MPU6050</h1>
  <canvas id="chartAceleracion"></canvas>

  <button id="ledButton" onclick="toggleLED()">Toggle LED</button>

  <script>
    // Inicializa los graficos con valores vacios
    const ctx = document.getElementById('chartAceleracion').getContext('2d');
    const chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: Array(20).fill(''),  // Etiquetas vacias
        datasets: [
          { label: 'Aceleracion X', borderColor: 'red', data: [] },
          { label: 'Aceleracion Y', borderColor: 'green', data: [] },
          { label: 'Aceleracion Z', borderColor: 'blue', data: [] }
        ]
      },
      options: {
        scales: {
          x: { display: true },
          y: { beginAtZero: true }
        }
      }
    });

    // Funcion para actualizar los datos del grafico
    async function fetchAceleracion() {
      const response = await fetch('/aceleracion');
      const data = await response.json();

      chart.data.datasets[0].data.push(data.aceleracionX);
      chart.data.datasets[1].data.push(data.aceleracionY);
      chart.data.datasets[2].data.push(data.aceleracionZ);

      if (chart.data.datasets[0].data.length > 20) {
        chart.data.datasets[0].data.shift();
        chart.data.datasets[1].data.shift();
        chart.data.datasets[2].data.shift();
      }

      chart.update();
    }

    // Funcion para encender/apagar el LED
    async function toggleLED() {
      const response = await fetch('/toggleLED');
      const result = await response.text();
      alert("LED Status: " + result);
    }

    // Actualizar los datos cada 500ms
    setInterval(fetchAceleracion, 500);
  </script>
</body>
</html>
)rawliteral";



// Función para obtener la aceleración y enviar datos en formato JSON
String getAceleracionJSON() {
  // Leer valores del acelerómetro
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);
  
  aceleracionX = ax / 16384.0;
  aceleracionY = ay / 16384.0;
  aceleracionZ = az / 16384.0;

  // Crear un JSON para enviar al cliente
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["aceleracionX"] = aceleracionX;
  jsonDoc["aceleracionY"] = aceleracionY;
  jsonDoc["aceleracionZ"] = aceleracionZ;

  String jsonString;
  serializeJson(jsonDoc, jsonString);
  return jsonString;
}
// Variable para almacenar el estado del LED
bool ledState = false;

void setup() {
  // Inicializa la comunicación serie
  Serial.begin(9600);  // Usa 115200 para evitar caracteres raros en el monitor serie
  delay(1500);
  
  // Inicializa el I2C y el sensor MPU6050
  Wire.begin();
  mpu.initialize();
  
  // Verifica la conexión con el MPU6050
  if (!mpu.testConnection()) {
    Serial.println("Error al conectar con el MPU6050");
    while (1);  // Si no se conecta, entra en un bucle infinito
  } else {
    Serial.println("MPU6050 conectado correctamente");
  }

  // Conectar a la red Wi-Fi
  Serial.println("Conectando a Wi-Fi...");
  WiFi.begin(ssid, password);

  int attempts = 0;  // Contador de intentos
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {  // Máximo 20 intentos
    delay(1000);  // Espera de 1 segundo entre intentos
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("Conectado a la red Wi-Fi");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Error: No se pudo conectar a la red Wi-Fi.");
  }

  // Configura el pin 27 como salida para el LED
  pinMode(27, OUTPUT);
  digitalWrite(27, LOW);  // Apaga el LED al inicio

  // Iniciar el servidor web si está conectado
  if (WiFi.status() == WL_CONNECTED) {
    // Ruta para servir la página HTML
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", index_html);
    });

    // Ruta para obtener los valores de aceleración
    server.on("/aceleracion", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "application/json", getAceleracionJSON());
    });

    // Ruta para encender/apagar el LED
    server.on("/toggleLED", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledState = !ledState;  // Cambiar el estado del LED
      digitalWrite(27, ledState ? HIGH : LOW);  // Encender o apagar el LED
      String ledStatus = ledState ? "Encendido" : "Apagado";
      request->send(200, "text/plain", ledStatus);  // Enviar el estado del LED como respuesta
    });

    // Iniciar el servidor
    server.begin();
  }
}

void loop() {
  // Nada que hacer en el bucle principal
}

