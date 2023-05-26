#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include <string>
#include <vector>
#include <array>

using Tablero = std::array<std::array<EstadoCelda, TAMANO_TABLERO>, TAMANO_TABLERO>;

struct EstadoCelda {
    enum class TipoEstado {
        VACIA,
        OCUPADA,
        AGUA,
        TOCADO,
        HUNDIDO
    };

    TipoEstado tipo;
    char nombreBarco;
};

struct TAMANO_TABLERO {
    int TAMANO_TABLERO =15;
};

// Estructura para representar las coordenadas de un disparo
struct CoordenadasDisparo {
    int fila;
    int columna;
};

// Enumeración para representar el resultado de un disparo
enum class ResultadoDisparo {
    AGUA,
    TOCADO,
    HUNDIDO
};

// Enumeración para representar el estado del juego
enum class EstadoJuego {
    EN_CURSO,
    FINALIZADO
};

// Enumeración para representar al ganador del juego
enum class Ganador {
    CLIENTE,
    SERVIDOR
};

// Estructura para representar una indicación enviada por el servidor al cliente
struct IndicacionServidor {
    enum class TipoIndicacion {
        INICIO,
        RESULTADO
    };

    TipoIndicacion tipo;
    EstadoJuego estadoJuego;
    Ganador ganador;
    ResultadoDisparo resultado;
};


// Función para enviar el nombre del cliente al servidor
bool enviarNombreCliente(int clienteSocket, const std::string& nombre);

// Función para recibir la posición de las embarcaciones del cliente
std::vector<std::string> recibirPosicionesEmbarcaciones(int clienteSocket);

// Función para enviar las coordenadas del disparo al servidor
bool enviarCoordenadasDisparo(int clienteSocket, const CoordenadasDisparo& coordenadas);

// Función para recibir las indicaciones del servidor
IndicacionServidor recibirIndicacionServidor(int clienteSocket);

#endif
