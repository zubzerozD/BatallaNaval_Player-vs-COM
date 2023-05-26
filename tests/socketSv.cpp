#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 7777;

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    int clientAddressLength = sizeof(clientAddress);

    // Crear el socket del servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error al crear el socket del servidor." << std::endl;
        return -1;
    }

    // Configurar la dirección del servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Enlazar el socket a la dirección del servidor
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error al enlazar el socket a la dirección del servidor." << std::endl;
        return -1;
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 1) < 0) {
        std::cerr << "Error al intentar escuchar conexiones entrantes." << std::endl;
        return -1;
    }

    std::cout << "Esperando conexiones entrantes..." << std::endl;

    // Aceptar la conexión entrante
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t *)&clientAddressLength);
    if (clientSocket < 0) {
        std::cerr << "Error al aceptar la conexión entrante." << std::endl;
        return -1;
    }

    std::cout << "Conexión establecida con el cliente." << std::endl;

    // Lógica del juego aquí...

    // Cerrar los sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}

//gcc -o socketSv socketSv.cpp lstdc++
//./socketSv