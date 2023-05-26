#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

const std::string SERVER_IP = "127.0.0.1";
const int PORT = 7777;

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;

    // Crear el socket del cliente
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error al crear el socket del cliente." << std::endl;
        return -1;
    }

    // Configurar la dirección del servidor
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    // Convertir la dirección IP de formato de cadena a formato binario
    if (inet_pton(AF_INET, SERVER_IP.c_str(), &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Dirección IP inválida o no soportada." << std::endl;
        return -1;
    }

// Conectar al servidor
if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
    std::cerr << "Error al intentar conectar al servidor." << std::endl;
    return -1;
}

std::cout << "Conexión establecida con el servidor." << std::endl;

// Lógica del juego aquí...

// Cerrar el socket del cliente
close(clientSocket);

return 0;
}

//gcc -o socketCl socketCl.cpp lstdc++
//./socketCl

