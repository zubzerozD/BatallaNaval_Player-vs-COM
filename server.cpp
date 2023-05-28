#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

const int PUERTO = 8080;
const int MAX_CLIENTES = 2;
int clientesConectados = 0;
pthread_t hilos[MAX_CLIENTES];
const int TAM_TABLERO = 15;
const int TAM_BUFFER = 4096;
const int TAMANO_BUFFER = 4096;
const char MAR = ' ';
const char AGUA = '~';
const char TOCADO = 'X';

struct DatosCliente
{
    int socket;
    int id;
    int indiceHilo;
};

struct Barco
{
    char tipo;
    int tamano;
};

struct Tablero
{
    char m_tablero[TAM_TABLERO][TAM_TABLERO];
};

struct TableroCPU
{
    char m_tablero[TAM_TABLERO][TAM_TABLERO];
};

// Función para convertir el tablero a una cadena de caracteres
string tableroToString(const Tablero &tablero)
{
    string tableroString;

    // Etiquetas de los tableros
    tableroString += "Usuario\n";
    tableroString += "   A B C D E F G H I J K L M N O\n"; // Letras de columna
    tableroString += " ------------------------------------\n";
    for (int i = 0; i < TAM_TABLERO; ++i)
    {
        if (i < 10)
        {
            tableroString += " ";
        }
        tableroString += to_string(i) + "|"; // Números de fila
        for (int j = 0; j < TAM_TABLERO; ++j)
        {
            tableroString += tablero.m_tablero[i][j];
            tableroString += '|'; // Cambiar el carácter de separación
        }
        tableroString += '\n';
    }
    return tableroString;
}

// Función para convertir el tablero a una cadena de caracteres
string tableroToString(const TableroCPU &tableroCPU)
{
    string tableroString;

    // Etiquetas de los tableros
    tableroString += "CPU\n";
    tableroString += "   A B C D E F G H I J K L M N O\n"; // Letras de columna
    tableroString += " ------------------------------------\n";
    for (int i = 0; i < TAM_TABLERO; ++i)
    {
        if (i < 10)
        {
            tableroString += " ";
        }
        tableroString += to_string(i) + "|"; // Números de fila
        for (int j = 0; j < TAM_TABLERO; ++j)
        {
            tableroString += tableroCPU.m_tablero[i][j];
            tableroString += '|'; // Cambiar el carácter de separación
        }
        tableroString += '\n';
    }
    return tableroString;
}

// Función para colocar los barcos aleatoriamente en el tablero
void colocarBarcosAleatoriamente(Tablero &tablero, TableroCPU &tableroCPU)
{
    srand(time(NULL));

    vector<Barco> barcos = {

        {'L', 1}, // Lanchas
    };

    for (const Barco &barco : barcos)
    {
        bool colocado = false;

        while (!colocado)
        {
            int filaInicial = rand() % TAM_TABLERO;
            int columnaInicial = rand() % TAM_TABLERO;
            bool orientacionHorizontal = rand() % 2 == 0;

            if (orientacionHorizontal)
            { // Colocar horizontalmente
                if (columnaInicial + barco.tamano <= TAM_TABLERO)
                { // Verificar que quepa en el tablero
                    bool ocupado = false;

                    for (int j = columnaInicial; j < columnaInicial + barco.tamano; ++j)
                    {
                        if (tablero.m_tablero[filaInicial][j] != MAR)
                        {
                            ocupado = true;
                            break;
                        }
                    }

                    if (!ocupado)
                    {
                        for (int j = columnaInicial; j < columnaInicial + barco.tamano; ++j)
                        {
                            tablero.m_tablero[filaInicial][j] = barco.tipo;
                        }

                        colocado = true;
                    }
                }
            }
            else
            { // Colocar verticalmente
                if (filaInicial + barco.tamano <= TAM_TABLERO)
                { // Verificar que quepa en el tablero
                    bool ocupado = false;

                    for (int i = filaInicial; i < filaInicial + barco.tamano; ++i)
                    {
                        if (tablero.m_tablero[i][columnaInicial] != MAR)
                        {
                            ocupado = true;
                            break;
                        }
                    }

                    if (!ocupado)
                    {
                        for (int i = filaInicial; i < filaInicial + barco.tamano; ++i)
                        {
                            tablero.m_tablero[i][columnaInicial] = barco.tipo;
                        }

                        colocado = true;
                    }
                }
            }
        }
    }
}

// Función para colocar los barcos aleatoriamente en el tablero
void colocarBarcosAleatoriamente(TableroCPU &tableroCPU)
{
    srand(time(NULL));

    vector<Barco> barcos = {

        {'L', 1}, // Lanchas
    };

    for (const Barco &barco : barcos)
    {
        bool colocado = false;

        while (!colocado)
        {
            int filaInicial = rand() % TAM_TABLERO;
            int columnaInicial = rand() % TAM_TABLERO;
            bool orientacionHorizontal = rand() % 2 == 0;

            if (orientacionHorizontal)
            { // Colocar horizontalmente
                if (columnaInicial + barco.tamano <= TAM_TABLERO)
                { // Verificar que quepa en el tablero
                    bool ocupado = false;

                    for (int j = columnaInicial; j < columnaInicial + barco.tamano; ++j)
                    {
                        if (tableroCPU.m_tablero[filaInicial][j] != MAR)
                        {
                            ocupado = true;
                            break;
                        }
                    }

                    if (!ocupado)
                    {
                        for (int j = columnaInicial; j < columnaInicial + barco.tamano; ++j)
                        {
                            tableroCPU.m_tablero[filaInicial][j] = barco.tipo;
                        }

                        colocado = true;
                    }
                }
            }
            else
            { // Colocar verticalmente
                if (filaInicial + barco.tamano <= TAM_TABLERO)
                { // Verificar que quepa en el tablero
                    bool ocupado = false;

                    for (int i = filaInicial; i < filaInicial + barco.tamano; ++i)
                    {
                        if (tableroCPU.m_tablero[i][columnaInicial] != MAR)
                        {
                            ocupado = true;
                            break;
                        }
                    }

                    if (!ocupado)
                    {
                        for (int i = filaInicial; i < filaInicial + barco.tamano; ++i)
                        {
                            tableroCPU.m_tablero[i][columnaInicial] = barco.tipo;
                        }

                        colocado = true;
                    }
                }
            }
        }
    }
}

// Función para procesar el ataque del cliente
void procesarAtaqueCliente(Tablero &tablero, TableroCPU &tableroCPU, int fila, int columna)
{
    if (tableroCPU.m_tablero[fila][columna] == MAR)
    {
        tableroCPU.m_tablero[fila][columna] = AGUA;
        cout << "El Jugador disparasparo al agua." << endl;
    }
    else if (tableroCPU.m_tablero[fila][columna] == AGUA)
    {
        cout << "El Jugador disparado a esa posición antes." << endl;
    }
    else
    {
        tableroCPU.m_tablero[fila][columna] = TOCADO;
        cout << "El Jugador disparo a un barco." << endl;
    }
}

void procesarAtaqueCPU(Tablero &tablero, TableroCPU &tableroCPU, int fila, int columna)
{
    if (tablero.m_tablero[fila][columna] == MAR)
    {
        tablero.m_tablero[fila][columna] = AGUA;
        cout << "La CPU disparo al agua." << endl;
    }
    else if (tablero.m_tablero[fila][columna] == AGUA)
    {
        tablero.m_tablero[fila][columna] = AGUA;
        cout << "La CPU disparo a esa posición antes." << endl;
    }
    else
    {
        tablero.m_tablero[fila][columna] = TOCADO;
        cout << "La CPU disparo a un barco." << endl;
    }
}

bool barcosCPUDestruidos(const TableroCPU &tableroCPU)
{
    for (int i = 0; i < TAM_TABLERO; ++i)
    {
        for (int j = 0; j < TAM_TABLERO; ++j)
        {
            if (tableroCPU.m_tablero[i][j] != 'P' && tableroCPU.m_tablero[i][j] != 'B' && tableroCPU.m_tablero[i][j] != 'S' && tableroCPU.m_tablero[i][j] != 'L')
            {
                return false;
            }
        }
    }
    return true;
}

bool barcosJugadorDestruidos(const Tablero &tablero)
{
    for (int i = 0; i < TAM_TABLERO; ++i)
    {
        for (int j = 0; j < TAM_TABLERO; ++j)
        {
            if (tablero.m_tablero[i][j] != 'P' && tablero.m_tablero[i][j] != 'B' && tablero.m_tablero[i][j] != 'S' && tablero.m_tablero[i][j] != 'L')
            {
                return false;
            }
        }
    }
    return true;
}

// Función que se ejecuta en el hilo del cliente
void *conexionCliente(void *datosCliente)
{
    DatosCliente *datos = static_cast<DatosCliente *>(datosCliente);
    int clienteSocket = datos->socket;
    int clienteID = datos->id;
    int indiceHilo = datos->indiceHilo;

    char buffer[TAM_BUFFER] = {0};
    Tablero tablero;
    TableroCPU tableroCPU;

    // Inicializar los tableros
    for (int i = 0; i < TAM_TABLERO; ++i)
    {
        for (int j = 0; j < TAM_TABLERO; ++j)
        {
            tablero.m_tablero[i][j] = MAR;
            tableroCPU.m_tablero[i][j] = MAR;
        }
    }

    vector<Barco> barcos = {{'A', 5}, {'B', 4}, {'C', 3}, {'D', 3}, {'E', 2}};
    colocarBarcosAleatoriamente(tablero, tableroCPU);

    string mensajeBienvenida = "¡Bienvenido al juego de Batalla Naval! Eres el Cliente " + to_string(clienteID) + "\n";
    send(clienteSocket, mensajeBienvenida.c_str(), mensajeBienvenida.length(), 0);

    // Enviar el tablero al cliente
    string tableroString = tableroToString(tablero);

    //Enviar el tablero al CPU
    string tableroCPUString = tableroToString(tableroCPU);

    send(clienteSocket, tableroString.c_str(), tableroString.length(), 0);
    // Ciclo principal del juego
    while (true)
    {
        // Esperar el ataque del cliente
        memset(buffer, 0, sizeof(buffer));
        char bufferFila[TAMANO_BUFFER];

        int bytesRecibidosFila = recv(clienteSocket, bufferFila, sizeof(bufferFila) - 1, 0);
        if (bytesRecibidosFila <= 0)
        {
            // Error al recibir la fila o el cliente se desconectó
            break;
        }
        bufferFila[bytesRecibidosFila] = '\0'; // Agregar el terminador nulo al final de los datos recibidos
        int fila = std::stoi(bufferFila);      // Convertir los datos recibidos a entero

        // Recibir la columna del cliente
        char bufferColumna[TAMANO_BUFFER]; // Definir un búfer para almacenar los datos recibidos
        int bytesRecibidosColumna = recv(clienteSocket, bufferColumna, sizeof(bufferColumna) - 1, 0);
        if (bytesRecibidosColumna <= 0)
        {
            // Error al recibir la columna o el cliente se desconectó
            break;
        }
        bufferColumna[bytesRecibidosColumna] = '\0'; // Agregar el terminador nulo al final de los datos recibidos
        int columna = std::stoi(bufferColumna);      // Convertir los datos recibidos a entero

        // Procesar el ataque del cliente llamando a la función 'procesarAtaqueCliente'
        procesarAtaqueCliente(tablero, tableroCPU, fila, columna);

        // Realizar el ataque de la CPU
        int filaCPU = rand() % TAM_TABLERO;
        int columnaCPU = rand() % TAM_TABLERO;
        procesarAtaqueCPU(tablero, tableroCPU, filaCPU, columnaCPU);

        // Enviar mensaje de resultado del disparo al cliente
        std::string mensajeResultadoJugador;
        if (tableroCPU.m_tablero[fila][columna] == MAR)
        {
            mensajeResultadoJugador = "El Jugador disparas paro al agua.\n";
        }
        else if (tableroCPU.m_tablero[fila][columna] == TOCADO)
        {
            mensajeResultadoJugador = "El Jugador disparo a un barco.\n";
        }
        else if (tableroCPU.m_tablero[fila][columna] == AGUA)
        {
            mensajeResultadoJugador = "El Jugador disparas paro al agua.\n";
        }
        send(clienteSocket, mensajeResultadoJugador.c_str(), mensajeResultadoJugador.length(), 0);

        // Enviar mensaje de resultado del disparo de la CPU al cliente
        std::string mensajeResultadoCPU;
        if (tablero.m_tablero[fila][columna] == MAR)
        {
            mensajeResultadoCPU = "La CPU disparo al agua.\n'";
        }
        else if (tablero.m_tablero[fila][columna] == TOCADO)
        {
            mensajeResultadoCPU = "La CPU disparo a un barco.\n";
        }
        else if (tablero.m_tablero[fila][columna] == AGUA)
        {
            mensajeResultadoCPU = "La CPU disparado a esa posición antes.\n";
        }

        bool barcosCPUHundidos = barcosCPUDestruidos(tableroCPU);
        if (barcosCPUHundidos)
        {
            // Mensaje de victoria
            std::string Ganadormensaje = "¡Has ganado! Has destruido todos los barcos de la CPU.";
            // Enviar el mensaje al cliente
            send(clienteSocket, Ganadormensaje.c_str(), Ganadormensaje.size(), 0);
            break;
        }

        // Verificar si todos los barcos del jugador han sido destruidos
        bool barcosJugadorHundidos = barcosJugadorDestruidos(tablero);
        if (barcosJugadorHundidos)
        {
            // Mensaje de derrota
            cout << "¡Has perdido! Todos tus barcos han sido destruidos por la CPU." << endl;
            break;
        }
    }

    // Cerrar la conexión con el cliente
    close(clienteSocket);
    clientesConectados--;
    pthread_exit(NULL);
}

int main()
{
    srand(time(NULL));

    int servidorSocket, clienteSocket;
    struct sockaddr_in direccion;

    // Crear el socket
    servidorSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidorSocket == -1)
    {
        cout << "Error al crear el socket." << endl;
        return 1;
    }

    // Configurar la dirección del servidor
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(PUERTO);
    direccion.sin_addr.s_addr = INADDR_ANY;

    // Vincular el socket a la dirección del servidor
    if (bind(servidorSocket, (struct sockaddr *)&direccion, sizeof(direccion)) == -1)
    {
        cout << "Error al vincular el socket." << endl;
        return 1;
    }

    // Escuchar nuevas conexiones
    if (listen(servidorSocket, MAX_CLIENTES) == -1)
    {
        cout << "Error al iniciar la escucha." << endl;
        return 1;
    }

    cout << "Servidor iniciado. Esperando conexiones..." << endl;

    while (true)
    {
        // Aceptar una nueva conexión
        clienteSocket = accept(servidorSocket, NULL, NULL);
        if (clienteSocket == -1)
        {
            cout << "Error al aceptar la conexión." << endl;
            return 1;
        }

        if (clientesConectados == MAX_CLIENTES)
        {
            string mensajeMaxClientes = "Ya se han conectado el máximo número de clientes. Inténtalo más tarde.\n";
            send(clienteSocket, mensajeMaxClientes.c_str(), mensajeMaxClientes.length(), 0);
            close(clienteSocket);
            continue;
        }

        // Crear un hilo para el cliente
        DatosCliente datosCliente;
        datosCliente.socket = clienteSocket;
        datosCliente.id = clientesConectados + 1;
        datosCliente.indiceHilo = clientesConectados;
        int resultado = pthread_create(&hilos[clientesConectados], NULL, conexionCliente, (void *)&datosCliente);
        if (resultado != 0)
        {
            cout << "Error al crear el hilo para el cliente." << endl;
            return 1;
        }

        clientesConectados++;
        if (clientesConectados == MAX_CLIENTES)
        {
            break;
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < MAX_CLIENTES; ++i)
    {
        pthread_join(hilos[i], NULL);
    }

    // Cerrar el socket del servidor
    close(servidorSocket);

    return 0;
}
