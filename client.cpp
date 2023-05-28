#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define TAM_BUFFER 2048
const char* SERVER_IP = "127.0.0.1";
const int PORT = 8080;

// Función para recibir datos del servidor
std::string recibirDatos(int socket) {
    char buffer[TAM_BUFFER];
    std::string datosRecibidos;
    int bytesRecibidos;

    do {
        bytesRecibidos = recv(socket, buffer, TAM_BUFFER - 1, 0);
        if (bytesRecibidos == -1) {
            throw std::runtime_error("Error al recibir datos del servidor");
        }
        buffer[bytesRecibidos] = '\0';
        datosRecibidos += buffer;
    } while (bytesRecibidos == TAM_BUFFER - 1);

    return datosRecibidos;
}

// Función para enviar datos al servidor
bool enviarDatos(int socket, const std::string& datos) {
    if (send(socket, datos.c_str(), datos.length(), 0) == -1) {
        std::cerr << "Error al enviar datos al servidor" << std::endl;
        return false;
    }
    return true;
}

int main() {
    int socketFd;
    struct sockaddr_in serverAddress;

    // Crear el socket del cliente
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) {
        std::cerr << "Error al crear el socket" << std::endl;
        return 1;
    }

    // Configurar la dirección del servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Dirección IP inválida o no soportada" << std::endl;
        return 1;
    }

    // Conectar con el servidor
    if (connect(socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error al conectar con el servidor" << std::endl;
        return 1;
    }

    // Recibir mensaje de confirmación de conexión
    char connectMsg[256];
    int bytesRead = read(socketFd, connectMsg, sizeof(connectMsg) - 1);
    if (bytesRead < 0) {
        std::cerr << "Error al recibir la confirmación de conexión" << std::endl;
        return 1;
    }
    connectMsg[bytesRead] = '\0';

    std::cout << "Mensaje del servidor: " << connectMsg << std::endl;

    // Recibir mensaje de inicio del juego
    char startMsg[256];
    bytesRead = read(socketFd, startMsg, sizeof(startMsg) - 1);
    if (bytesRead < 0) {
        std::cerr << "Error al recibir el mensaje de inicio del juego" << std::endl;
        return 1;
    }
    startMsg[bytesRead] = '\0';

    std::cout << "Mensaje del servidor: " << startMsg << std::endl;


    while (true) {
         // Recibir mensaje de bienvenida
        std::string mensajeBienvenida = recibirDatos(socketFd);
        std::cout << mensajeBienvenida << std::endl;

        std::cout << "Tableros iniciales:" << std::endl;
        std::string tableroString = recibirDatos(socketFd);
        std::string tableroCPUString = recibirDatos(socketFd);
        // Solicitar la fila al usuario
        std::cout << "Ingrese la fila de ataque: ";
        int fila;
        std::cin >> fila;

        // Enviar la fila al servidor
        if (!enviarDatos(socketFd, std::to_string(fila))) {
            return 1;
        }

        // Solicitar la columna al usuario
        std::cout << "Ingrese la columna de ataque: ";
        int columna;
        std::cin >> columna;

        // Enviar la columna al servidor
        if (!enviarDatos(socketFd, std::to_string(columna))) {
            return 1;
        }

        // Recibir mensaje de resultado del disparo
        std::string mensajeResultadoJugador = recibirDatos(socketFd);
        std::cout << mensajeResultadoJugador << std::endl;

        // Recibir mensaje de resultado del disparo
        std::string mensajeResultadoCPU = recibirDatos(socketFd);
        std::cout << mensajeResultadoCPU << std::endl;

        std::string mensajeGanador = recibirDatos(socketFd);
        std::cout << mensajeGanador << std::endl;

        std::string mensajePerdedor = recibirDatos(socketFd);
        std::cout << mensajePerdedor << std::endl;
    }

    // Cerrar conexión
    close(socketFd);
    return 0;
}