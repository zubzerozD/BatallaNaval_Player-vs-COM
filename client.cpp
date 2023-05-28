#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

// Función para recibir datos del servidor
std::string recibirDatos(int socket) {
    char buffer[1024];
    std::string datosRecibidos;
    int bytesRecibidos;

    do {
        memset(buffer, 0, 1024);
        bytesRecibidos = recv(socket, buffer, 1024 - 1, 0);
        if (bytesRecibidos == -1) {
            throw std::runtime_error("Error al recibir datos del servidor");
        }
        buffer[bytesRecibidos] = '\0';
        datosRecibidos += buffer;
    } while (bytesRecibidos > 0);

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
    int socketCliente;
    struct sockaddr_in servidorDir;
    char mensaje[1024];

    // Crear socket
    socketCliente = socket(AF_INET, SOCK_STREAM, 0);
    if (socketCliente == -1) {
        std::cerr << "Error al crear el socket." << std::endl;
        return 1;
    }

    // Configurar dirección del servidor
    servidorDir.sin_family = AF_INET;
    servidorDir.sin_port = htons(8080); // Puerto del servidor
    servidorDir.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección IP del servidor

    // Conectar al servidor
    if (connect(socketCliente, (struct sockaddr*)&servidorDir, sizeof(servidorDir)) == -1) {
        std::cerr << "Error al conectar al servidor." << std::endl;
        return 1;
    }

    // Recibir y mostrar los tableros inicializados
    std::string tablerosIniciales = recibirDatos(socketCliente);
    std::cout << "Tableros iniciales:" << std::endl;
    std::cout << tablerosIniciales << std::endl;

    // Recibir mensaje de bienvenida
    std::string mensajeBienvenida = recibirDatos(socketCliente);
    std::cout << mensajeBienvenida << std::endl;

    while (true) {
        // Solicitar la fila al usuario
        std::cout << "Ingrese la fila de ataque: ";
        int fila;
        std::cin >> fila;

        // Enviar la fila al servidor
        if (!enviarDatos(socketCliente, std::to_string(fila))) {
            return 1;
        }

        // Solicitar la columna al usuario
        std::cout << "Ingrese la columna de ataque: ";
        int columna;
        std::cin >> columna;

        // Enviar la columna al servidor
        if (!enviarDatos(socketCliente, std::to_string(columna))) {
            return 1;
        }

        // Recibir y mostrar los tableros actualizados
        std::string tablerosActualizados = recibirDatos(socketCliente);
        std::cout << "Tableros actualizados:" << std::endl;
        std::cout << tablerosActualizados << std::endl;

        // Recibir mensaje de resultado del disparo
        std::string mensajeResultadoJugador = recibirDatos(socketCliente);
        std::cout << mensajeResultadoJugador << std::endl;

        // Recibir mensaje de resultado del disparo
        std::string mensajeResultadoCPU = recibirDatos(socketCliente);
        std::cout << mensajeResultadoCPU << std::endl;

        std::string mensajeGanador = recibirDatos(socketCliente);
        std::cout << mensajeGanador << std::endl;

        std::string mensajePerdedor = recibirDatos(socketCliente);
        std::cout << mensajePerdedor << std::endl;
    }

    // Cerrar conexión
    close(socketCliente);

    return 0;
}
