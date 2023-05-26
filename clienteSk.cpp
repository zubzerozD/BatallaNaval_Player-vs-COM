#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "protocolo.h"

void imprimirTablero(const Tablero& tablero) {
    // Imprime los encabezados de las columnas
    
    std::cout << " ";
    for (int columna = 0; columna < TAMANO_TABLERO; columna++) {
        std::cout << " " << static_cast<char>('A' + columna);
    }
    std::cout << std::endl;


    // Imprime las filas y los elementos del tablero
    for (int fila = 0; fila < TAMANO_TABLERO; fila++) {
        std::cout << static_cast<char>('A' + fila);
        for (int columna = 0; columna < TAMANO_TABLERO; columna++) {
            std::cout << " ";
            switch (tablero[fila][columna]) {
                case EstadoCelda::VACIA:
                    std::cout << " ";
                    break;
                case EstadoCelda::OCUPADA:
                    std::cout << "O";
                    break;
                case EstadoCelda::AGUA:
                    std::cout << "~";
                    break;
                case EstadoCelda::TOCADO:
                    std::cout << "X";
                    break;
                case EstadoCelda::HUNDIDO:
                    std::cout << "H";
                    break;
            }
        }
        std::cout << std::endl;
    }
}


bool validarCoordenadas(int fila, int columna) {
    // Implementa la función para validar las coordenadas ingresadas por el cliente
    // Puedes verificar si las coordenadas están dentro de los límites del tablero
    // y si la celda seleccionada no ha sido disparada previamente
}

void jugarBatallaNaval(int clienteSocket) {
    // Inicia el juego
    std::cout << "Conectado al servidor. Inicia el juego.\n";

    // Recibir información inicial del servidor
    InfoInicial infoInicial;
    if (recv(clienteSocket, reinterpret_cast<char*>(&infoInicial), sizeof(infoInicial), 0) == -1) {
        std::cerr << "Error al recibir información inicial del servidor\n";
        return;
    }

    // Mostrar información inicial recibida del servidor
    std::cout << "¡Comienza el juego de Batalla Naval!\n";
    std::cout << "Tamaño del tablero: " << infoInicial.tamanoTablero << "x" << infoInicial.tamanoTablero << "\n";
    std::cout << "Número de embarcaciones:\n";
    std::cout << " - Portaaviones: " << infoInicial.numeroPortaaviones << "\n";
    std::cout << " - Buques: " << infoInicial.numeroBuques << "\n";
    std::cout << " - Submarinos: " << infoInicial.numeroSubmarinos << "\n";
    std::cout << " - Lanchas: " << infoInicial.numeroLanchas << "\n";

    // Solicitar al cliente que defina las posiciones de las embarcaciones (puede ser aleatorio)
    std::cout << "Define las posiciones de tus embarcaciones:\n";
    // Implementa la lógica para que el cliente defina las posiciones de las embarcaciones

    // Bucle principal del juego
    while (true) {
        // Recibir tablero actualizado del servidor
        Tablero tablero;
        if (recv(clienteSocket, reinterpret_cast<char*>(&tablero), sizeof(Tablero), 0) == -1) {
            std::cerr << "Error al recibir el tablero del servidor\n";
            return;
        }

        // Imprimir el tablero actualizado
        imprimirTablero(tablero);

        // Recibir indicación del servidor (turno o resultado del disparo)
        IndicacionServidor indicacion;
        if (recv(clienteSocket, reinterpret_cast<char*>(&indicacion), sizeof(IndicacionServidor), 0) == -1) {
            std::cerr << "Error al recibir la indicación del servidor\n";
            return;
        }

        if (indicacion.tipo == TipoIndicacion::TURNO) {
            std::cout << "¡Es tu turno! Ingresa las coordenadas del disparo (fila, columna): ";

            // Leer las coordenadas del disparo ingresadas por el cliente
            int fila, columna;
            std::cin >> fila >> columna;

            // Validar las coordenadas ingresadas
            if (!validarCoordenadas(fila, columna)) {
                std::cout << "Coordenadas inválidas. Inténtalo de nuevo.\n";
                continue;
            }

            // Enviar las coordenadas del disparo al servidor
            CoordenadasDisparo coordenadasDisparo;
            coordenadasDisparo.fila = fila;
            coordenadasDisparo.columna = columna;
            if (send(clienteSocket, reinterpret_cast<const char*>(&coordenadasDisparo), sizeof(CoordenadasDisparo), 0) == -1) {
                std::cerr << "Error al enviar las coordenadas del disparo al servidor\n";
                return;
            }
        } else if (indicacion.tipo == TipoIndicacion::RESULTADO) {
            // Mostrar el resultado del disparo
            std::cout << "Resultado del disparo: ";
            if (indicacion.resultado == ResultadoDisparo::AGUA) {
                std::cout << "Agua\n";
            } else if (indicacion.resultado == ResultadoDisparo::TOCADO) {
                std::cout << "Tocado\n";
            } else if (indicacion.resultado == ResultadoDisparo::HUNDIDO) {
                std::cout << "Hundido\n";
            }

            if (indicacion.estadoJuego == EstadoJuego::FINALIZADO) {
                // El juego ha finalizado
                if (indicacion.ganador == Ganador::SERVIDOR) {
                    std::cout << "¡Perdiste! El servidor ha ganado el juego.\n";
                } else {
                    std::cout << "¡Felicitaciones! Ganaste el juego.\n";
                }

                break;
            }
        }
    }

    // Cerrar la conexión con el servidor
    close(clienteSocket);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <IP> <puerto>\n";
        return 1;
    }

    const char* ipServidor = argv[1];
    int puertoServidor = std::stoi(argv[2]);

    // Crear el socket del cliente
    int clienteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clienteSocket == -1) {
        std::cerr << "Error al crear el socket del cliente\n";
        return 1;
    }

    // Establecer la dirección del servidor
    sockaddr_in servidorAddr{};
    servidorAddr.sin_family = AF_INET;
    servidorAddr.sin_port = htons(puertoServidor);
    if (inet_pton(AF_INET, ipServidor, &(servidorAddr.sin_addr)) == -1) {
        std::cerr << "Error al convertir la dirección IP del servidor\n";
        return 1;
    }

    // Conectar al servidor
    if (connect(clienteSocket, reinterpret_cast<sockaddr*>(&servidorAddr), sizeof(servidorAddr)) == -1) {
        std::cerr << "Error al conectar al servidor\n";
        return 1;
    }

    // Jugar Batalla Naval
    jugarBatallaNaval(clienteSocket);

    return 0;
}