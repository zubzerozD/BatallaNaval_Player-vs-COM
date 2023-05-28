#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdexcept>

#define TAM_BUFFER 50000

class ClienteSocket {
private:
    int socketCliente;
    struct sockaddr_in servidorDir;

public:
    ClienteSocket(int puerto) {
        // Crear socket
        socketCliente = socket(AF_INET, SOCK_STREAM, 0);
        if (socketCliente == -1) {
            throw std::runtime_error("Error al crear el socket.");
        }

        // Configurar dirección del servidor
        servidorDir.sin_family = AF_INET;
        servidorDir.sin_port = htons(puerto);             // Puerto del servidor
        servidorDir.sin_addr.s_addr = inet_addr("127.0.0.1"); // Dirección IP del servidor

        // Conectar al servidor
        if (connect(socketCliente, (struct sockaddr *)&servidorDir, sizeof(servidorDir)) == -1) {
            throw std::runtime_error("Error al conectar al servidor.");
        }
    }

    std::string recibirDatos() {
        char buffer[TAM_BUFFER];
        std::string datosRecibidos;
        int bytesRecibidos;

        do {
            bytesRecibidos = recv(socketCliente, buffer, TAM_BUFFER - 1, 0);
            if (bytesRecibidos == -1) {
                throw std::runtime_error("Error al recibir datos del servidor");
            }
            buffer[bytesRecibidos] = '\0';
            datosRecibidos += buffer;
        } while (bytesRecibidos == TAM_BUFFER - 1);

        return datosRecibidos;
    }

    bool enviarDatos(const std::string &datos) {
        if (send(socketCliente, datos.c_str(), datos.length(), 0) == -1) {
            std::cerr << "Error al enviar datos al servidor" << std::endl;
            return false;
        }
        return true;
    }

    void cerrarConexion() {
        close(socketCliente);
    }
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Indicar puerto" << std::endl;
        return -1;
    }

    int serverPort = atoi(argv[1]);

    try {
        ClienteSocket cliente(serverPort);

        // Recibir mensaje de bienvenida
        std::string mensajeBienvenida = cliente.recibirDatos();
        std::cout << mensajeBienvenida << std::endl;

        // Recibir y mostrar los tableros inicializados
        std::string tablerosIniciales = cliente.recibirDatos();
        std::cout << "Tableros iniciales:" << std::endl;
        std::cout << tablerosIniciales << std::endl;

        while (true) {
            // Solicitar la fila al usuario
            std::cout << "Ingrese la fila de ataque: ";
            int fila;
            std::cin >> fila;

            // Enviar la fila al servidor
            if (!cliente.enviarDatos(std::to_string(fila))) {
                return 1;
            }

            // Solicitar la columna al usuario
            std::cout << "Ingrese la columna de ataque: ";
            int columna;
            std::cin >> columna;

            // Enviar la columna al servidor
            if (!cliente.enviarDatos(std::to_string(columna))) {
                return 1;
            }

            std::string mensaje = cliente.recibirDatos();
            std::cout << mensaje << std::endl;
        }

        cliente.cerrarConexion();
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}