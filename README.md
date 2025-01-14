Autores:

Brayan Israel Lira Gutiérrez bi.liragutierrez@ugto.mx

Ponce Ruiz Angel Mario am.ponceruiz@ugto.mx

Jorge Andrés Jiménez Salazar ja.jimenez.salazar@ugto.mx

Eduardo Ramírez de la Fuente e.ramirezdelafuente@ugto.mx

Profesor: Huetzin Aaron Pérez Olivas

Descripción del proyecto:

El proyecto consiste en generar un servidor en el ESP32 para mostrar los valores de un sensor MPU6050 y colocar un botón en el servidor para mandar la señal de apagado y encendido de un led.

Introducción

El ESP32 es una potente microcontroladora que, gracias a su capacidad de conexión Wi-Fi y Bluetooth, se ha convertido en una de las plataformas favoritas para proyectos de Internet de las Cosas (IoT). Entre sus múltiples funcionalidades destaca la posibilidad de configurar un servidor web embebido, lo que permite que la placa actúe como un servidor HTTP ligero. Esto abre un mundo de posibilidades para interactuar de manera remota con dispositivos y sensores desde cualquier navegador web.

Un servidor web en un ESP32 permite recibir y responder peticiones HTTP, lo que lo hace ideal para aplicaciones como el control de dispositivos a distancia, la monitorización de datos en tiempo real, o la implementación de interfaces web intuitivas sin necesidad de hardware adicional. Por ejemplo, es posible encender y apagar LEDs, controlar motores, o mostrar lecturas de sensores como temperatura o humedad directamente desde una página web accesible en la misma red local o incluso por Internet.

Este tipo de servidor puede funcionar de dos maneras principales: conectándose a una red Wi-Fi existente como cliente o creando su propio punto de acceso (AP), permitiendo que otros dispositivos se conecten a él. Utilizando bibliotecas como ESPAsyncWebServer o WebServer, es posible definir rutas HTTP personalizadas que respondan a comandos específicos, mostrando contenido en HTML, manejando formularios o devolviendo datos en formato JSON.

Sensor MPU6050

El MPU6050 es un sensor avanzado que combina un acelerómetro y un giroscopio de 3 ejes en un solo módulo, lo que permite medir tanto la aceleración lineal como la rotación angular de un objeto en el espacio. Además, cuenta con un procesador de movimiento digital (DMP) incorporado, lo que facilita la captura de datos precisos y la fusión de sensores para aplicaciones que requieren control de movimiento y orientación.

Aplicaciones del sensor

Estabilización de drones y robótica: Ayuda a mantener el equilibrio y la orientación.

Controles de juegos y realidad virtual: Permite el seguimiento de movimientos precisos.

Podómetros y wearables: Monitorea la actividad física detectando cambios en aceleración.

Vehículos autónomos: Utilizado en sistemas de navegación inercial para estimar la posición en ausencia de GPS.

