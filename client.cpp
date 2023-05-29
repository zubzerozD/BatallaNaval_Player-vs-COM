#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdexcept>
#include <cctype> // Para std::toupper
#include <limits> // Para std::numeric_limits
#include <cctype> // Para std::toupper
#include <limits> // Para std::numeric_limits
#include <netdb.h> // Para gethostbyname

#define TAM_BUFFER 50000

class ClienteSocket
{
private:
    int socketCliente;
    struct sockaddr_in servidorDir;

public:
    ClienteSocket(const char* direccionIP,int puerto)
    {
        // Crear socket
        socketCliente = socket(AF_INET, SOCK_STREAM, 0);
        if (socketCliente == -1)
        {
            throw std::runtime_error("Error al crear el socket.");
        }

        // Configurar dirección del servidor
        servidorDir.sin_family = AF_INET;
        servidorDir.sin_port = htons(puerto);                 // Puerto del servidor
        servidorDir.sin_addr.s_addr = inet_addr(direccionIP); // Dirección IP del servidor
        // Conectar al servidor
        if (connect(socketCliente, (struct sockaddr *)&servidorDir, sizeof(servidorDir)) == -1)
        {
            throw std::runtime_error("Error al conectar al servidor.");
        }
    }

    std::string recibirDatos()
    {
        char buffer[TAM_BUFFER];
        std::string datosRecibidos;
        int bytesRecibidos;

        do
        {
            bytesRecibidos = recv(socketCliente, buffer, TAM_BUFFER - 1, 0);
            if (bytesRecibidos == -1)
            {
                throw std::runtime_error("Error al recibir datos del servidor");
            }
            buffer[bytesRecibidos] = '\0';
            datosRecibidos += buffer;
        } while (bytesRecibidos == TAM_BUFFER - 1);

        return datosRecibidos;
    }

    bool enviarDatos(const std::string &datos)
    {
        if (send(socketCliente, datos.c_str(), datos.length(), 0) == -1)
        {
            std::cerr << "Error al enviar datos al servidor" << std::endl;
            return false;
        }
        return true;
    }

    void cerrarConexion()
    {
        close(socketCliente);
    }
};

int main(int argc,char *argv[])
{
    if (argc < 3)
    {
        std::cout << "Indicar puerto" << std::endl;
        return -1;
    }
    const char* serverIP = argv[1];
    int serverPort = atoi(argv[2]);

    try
    {
        ClienteSocket cliente(serverIP, serverPort);

        // Recibir mensaje de bienvenida
        std::string mensajeBienvenida = cliente.recibirDatos();
        std::cout << mensajeBienvenida << std::endl;

        // Recibir y mostrar los tableros inicializados
        std::string tablerosIniciales = cliente.recibirDatos();
        std::cout << "Tableros iniciales:" << std::endl;
        std::cout << tablerosIniciales << std::endl;

        bool felicidades = false; // Variable para indicar si se ha recibido el mensaje "¡¡¡Felicidades Ganaste!!!"

        while (true)
        {
            while (true)
            {
                // Solicitar la fila al usuario
                std::cout << "Ingrese la fila de ataque (0-14): ";
                int fila;
                std::cin >> fila;

                if (std::cin.fail() || std::cin.peek() != '\n' || (fila < 0 || fila > 14))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Ingresar una fila válida." << std::endl;
                    continue; // Volver a solicitar la fila
                }

                // Enviar la fila al servidor
                if (!cliente.enviarDatos(std::to_string(fila)))
                {
                    return 1;
                }

                while (true)
                {
                    // Solicitar la columna al usuario
                    std::cout << "Ingrese la columna de ataque (A-O): ";
                    char columna;
                    std::cin >> columna;

                    columna = std::toupper(columna); // Convertir a mayúscula

                    if (std::cin.fail() || std::cin.peek() != '\n' || (columna < 'A' || columna > 'O'))
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Ingresar una columna válida." << std::endl;
                        continue; // Volver a solicitar la columna
                    }

                    std::string columnaStr = std::to_string(columna - 'A');
                    if (!cliente.enviarDatos(columnaStr))
                    {
                        return 1;
                    }

                    break;
                }

                break;
            }

            std::string mensaje = cliente.recibirDatos();
            std::cout << mensaje << std::endl;

            std::string mensaje2 = cliente.recibirDatos();
            std::cout << mensaje2 << std::endl;

            
        }

        cliente.cerrarConexion(); // Cerrar la conexión

    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
