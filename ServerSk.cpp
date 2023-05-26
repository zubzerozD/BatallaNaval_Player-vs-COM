#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "protocolo.h"

void manejarCliente(int clienteSocket) {
    // Implementa el manejo de un cliente individual aquí
    // Puede incluir la lógica del juego y la comunicación con el cliente

    close(clienteSocket);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <puerto>\n";
        return 1;
    }

    int puertoServidor = std::stoi(argv[1]);

    // Crear socket
    int servidorSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidorSocket == -1) {
        std::cerr << "Error al crear el socket\n";
        return 1;
    }

    // Enlace del socket
    sockaddr_in servidorAddr{};
    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_addr.s_addr = INADDR_ANY;
    servidorAddr.sin_port = htons(puertoServidor);

    if (bind(servidorSocket, reinterpret_cast<sockaddr *>(&servidorAddr), sizeof(servidorAddr)) == -1) {
        std::cerr << "Error al enlazar el socket\n";
        return 1;
    }

    // Escuchar conexiones entrantes
    if (listen(servidorSocket, SOMAXCONN) == -1) {
        std::cerr << "Error al escuchar en el socket\n";
        return 1;
    }

    std::cout << "Servidor iniciado. Esperando conexiones...\n";

    while (true) {
        // Aceptar una conexión entrante
        sockaddr_in clienteAddr{};
        socklen_t clienteAddrLen = sizeof(clienteAddr);
        int clienteSocket = accept(servidorSocket, reinterpret_cast<sockaddr *>(&clienteAddr), &clienteAddrLen);
        if (clienteSocket == -1) {
            std::cerr << "Error al aceptar la conexión del cliente\n";
            continue;
        }

        // Crear un hilo para manejar al cliente
        std::thread hiloCliente(manejarCliente, clienteSocket);
        hiloCliente.detach();
    }

    // Cerrar el socket del servidor
    close(servidorSocket);

    return 0;
}