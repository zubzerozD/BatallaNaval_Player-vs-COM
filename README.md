# BatallaNaval_Player-vs-COM
Tarea de COMUNICACIÓN DE DATOS Y REDES

## Instrucciones de uso

1. Clonar el repositorio.
2. Abrir la carpeta del repositorio en la terminal.
3. Ejecutar el comando `make` para compilar el programa.
4. Ejecutar el comando y indicar el puerto del servidor `./server puerto` para iniciar el servidor. (Ejemplo: `./server 8080`)
5. Abrir otra terminal en la misma carpeta.
6. Ejecutar el comando y indicar la direccion Ip,y puerto `./client DirrecionIP puerto ` para iniciar el cliente. (Ejemplo: `./client 127.0.0.1 8080`)
7. Seguir las instrucciones del programa.

## Integrantes
By:
- Sebastian Canales Ortiz.
- Nelson Rubio Guerrero.

## Requerimientos

-Crear un juego de batalla naval en el cual se pueda jugar contra la computadora.
-Usar sockets para la comunicación entre el cliente y el servidor.
-El servidor debe ser capaz de manejar múltiples clientes.
-El servidor debe ser capaz de manejar múltiples partidas al mismo tiempo.

Por parte del cliente:
• Conectar al servidor.
• Definir posiciones de embarcaciones (puede ser aleatorea) y enviarlas al servidor.
• Visualizar tableros.
• Jugar:
◦ Indicar coordenadas de disparo.
◦ Esperar y visualizar el resultado.
◦ Lo anterior se repite hasta que existe un ganador.

Por parte del servidor:
• Levantar servicio y esperar por conexiones (clientes).
• Por cada jugador conectado (cada hebra):
◦ Definir posiciones de embarcaciones (aleatorea).
◦ Solicitar disparo o generar disparo al azar
◦ Verificar las jugadas.
◦ Enviar resultados.

## Descripción
- El juego consiste en un tablero de 15x15 en el cual se colocan 5 barcos de diferentes tamaños, los cuales son:
  - 1 barco de 5 casillas
  - 2 barco de 4 casillas
  - 2 barcos de 3 casillas
  - 3 barco de 1 casillas
