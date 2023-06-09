#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <mutex>

using namespace std;

const int MAX_CLIENTES = 2;
int clientesConectados = 0;
pthread_t hilos[MAX_CLIENTES];
const int TAM_TABLERO = 15;
const int TAM_BUFFER = 2048;
const int TAMANO_BUFFER = 50000;
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

string tableroToStringCPU(const Tablero &tableroCpu)
{
    string tableroString;

    // Etiquetas de los tableros
    tableroString += "\nRival\n";
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
            char valor = tableroCpu.m_tablero[i][j];
            if (valor == TOCADO || valor == AGUA)
            {
                tableroString += valor;
            }
            else
            {
                tableroString += MAR;
            }
            tableroString += '|'; // Cambiar el carácter de separación
        }
        tableroString += '\n';
    }

    return tableroString;
}

// Función para colocar los barcos aleatoriamente en el tablero
void colocarBarcosAleatoriamente(Tablero &tablero, Tablero &tableroCpu)
{
    srand(time(NULL));

    vector<Barco> barcos = {
        {'L', 1}, // Lanchas
        {'L', 1}, // Lanchas
        {'L', 1}, // Lanchas
        {'S', 3}, // Submarino
        {'S', 3}, // Submarino
        {'B', 4}, // Buque
        {'B', 4}, // Buque
        {'P', 5}, //Portaaviones
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

    // Colocar barcos aleatoriamente en el tablero de la CPU (sin mostrarlos)
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
                        if (tableroCpu.m_tablero[filaInicial][j] != MAR)
                        {
                            ocupado = true;
                            break;
                        }
                    }

                    if (!ocupado)
                    {
                        for (int j = columnaInicial; j < columnaInicial + barco.tamano; ++j)
                        {
                            tableroCpu.m_tablero[filaInicial][j] = barco.tipo;
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
                        if (tableroCpu.m_tablero[i][columnaInicial] != MAR)
                        {
                            ocupado = true;
                            break;
                        }
                    }

                    if (!ocupado)
                    {
                        for (int i = filaInicial; i < filaInicial + barco.tamano; ++i)
                        {
                            tableroCpu.m_tablero[i][columnaInicial] = barco.tipo;
                        }

                        colocado = true;
                    }
                }
            }
        }
    }
}

void enviarDatosDoble(int clienteSocket, const std::string &mensaje, const std::string &otroMensaje)
{
    // Convertir el mensaje a un arreglo de caracteres
    const char *buffer = mensaje.c_str();

    // Enviar los datos al cliente
    int bytesEnviados = send(clienteSocket, buffer, mensaje.length(), 0);

    if (bytesEnviados == -1)
    {
        std::cerr << "Error al enviar los datos al cliente." << std::endl;
    }

    // Convertir el otro mensaje a un arreglo de caracteres
    const char *bufferOtroMensaje = otroMensaje.c_str();

    // Enviar los datos del otro mensaje al cliente
    int bytesEnviadosOtroMensaje = send(clienteSocket, bufferOtroMensaje, otroMensaje.length(), 0);

    if (bytesEnviadosOtroMensaje == -1)
    {
        std::cerr << "Error al enviar el otro mensaje al cliente." << std::endl;
    }
}

void enviarDatos(int clienteSocket, const std::string &mensaje)
{
    // Convertir el mensaje a un arreglo de caracteres
    const char *buffer = mensaje.c_str();

    // Enviar los datos al cliente
    int bytesEnviados = send(clienteSocket, buffer, mensaje.length(), 0);

    if (bytesEnviados == -1)
    {
        std::cerr << "Error al enviar los datos al cliente." << std::endl;
    }
}

void enviarDatosCPU(int clienteSocket, const std::string &mensaje)
{
    // Convertir el mensaje a un arreglo de caracteres
    const char *buffer = mensaje.c_str();

    // Enviar los datos al cliente
    int bytesEnviados = send(clienteSocket, buffer, mensaje.length(), 0);

    if (bytesEnviados == -1)
    {
        std::cerr << "Error al enviar los datos al cliente." << std::endl;
    }
}

bool verificarGanador(const Tablero &tablero)
{
    for (int i = 0; i < TAM_TABLERO; ++i)
    {
        for (int j = 0; j < TAM_TABLERO; ++j)
        {
            char c = tablero.m_tablero[i][j];
            if (c != AGUA && c != MAR && c != TOCADO)
            {
                return false; // Si se encuentra un barco no destruido, el juego continúa
            }
        }
    }
    return true; // Si no se encontró ningún barco no destruido, el jugador ha ganado
}

void procesarAtaqueCliente(Tablero &tablero, Tablero &tableroCpu, int fila, int columna, int clienteSocket, int clienteID)
{
    std::string mensaje;

    if (tableroCpu.m_tablero[fila][columna] == MAR)
    {
        tableroCpu.m_tablero[fila][columna] = AGUA;
        mensaje = "[cliente " + std::to_string(clienteID) + "] ¡El Usuario hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") fue al mar!\n";
    }
    else if (tableroCpu.m_tablero[fila][columna] == AGUA)
    {
        tableroCpu.m_tablero[fila][columna] = AGUA;
        mensaje = "[cliente " + std::to_string(clienteID) + "] ¡El Usuario hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") Otra vez al mar!\n";
    }
    else if (tableroCpu.m_tablero[fila][columna] == 'L')
    {
        tableroCpu.m_tablero[fila][columna] = 'X';
        mensaje = "[cliente " + std::to_string(clienteID) + "] ¡El Usuario hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") Disparo exitoso a la lancha!\n";
    }
    else if (tableroCpu.m_tablero[fila][columna] == 'S')
    {
        tableroCpu.m_tablero[fila][columna] = 'X';
        mensaje = "[cliente " + std::to_string(clienteID) + "] ¡El Usuario hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") Disparo exitoso al submarino!\n";
    }
    else if (tableroCpu.m_tablero[fila][columna] == 'B')
    {
        tableroCpu.m_tablero[fila][columna] = 'X';
        mensaje = "[cliente " + std::to_string(clienteID) + "] ¡El Usuario hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") Disparo exitoso aL buque!\n";
    }
    else
    {
        tableroCpu.m_tablero[fila][columna] = 'X';
        mensaje = "[cliente " + std::to_string(clienteID) + "] ¡El Usuario hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") Disparo exitoso al portaaviones!\n";
    }

    std::cout << mensaje;
    enviarDatos(clienteSocket, mensaje);
    enviarDatos(clienteSocket, tableroToStringCPU(tableroCpu));

    if (verificarGanador(tableroCpu))
    {
        string mensajeGanadorServer = "[cliente " + to_string(clienteID) + "] ¡Felicidades, has ganado!\n";
        cout << mensajeGanadorServer;
        string mensajeGanador = "\n¡¡¡Felicidades Ganaste!!!\n";
        enviarDatos(clienteSocket, mensajeGanador);
        sleep(30);
        close(clienteSocket);
    }
}

void procesarAtaqueCPU(Tablero &tablero, Tablero &tableroCpu, int fila, int columna, int clienteSocket, int clienteID)
{
    std::string mensajeCPU; // Declarar la variable mensaje fuera de los bloques condicionales

    if (tablero.m_tablero[fila][columna] == MAR)
    {
        tablero.m_tablero[fila][columna] = AGUA;
        mensajeCPU = "[cliente " + std::to_string(clienteID) + "] ¡La CPU hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") fue al mar!\n"; // Asignar un valor a mensaje con las coordenadas
    }
    else if (tablero.m_tablero[fila][columna] == AGUA)
    {
        tablero.m_tablero[fila][columna] = AGUA;
        mensajeCPU = "[cliente " + std::to_string(clienteID) + "] ¡La CPU hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") Otra vez al mar!\n"; // Asignar un valor a mensaje con las coordenadas
    }
    else if (tableroCpu.m_tablero[fila][columna] == 'L')
    {
        tablero.m_tablero[fila][columna] = 'X';
        mensajeCPU = "[cliente " + std::to_string(clienteID) + "] ¡La CPU hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") Disparo exitoso a la lancha!\n";
    }
    else if (tablero.m_tablero[fila][columna] == 'S')
    {
        tablero.m_tablero[fila][columna] = 'X';
        mensajeCPU = "[cliente " + std::to_string(clienteID) + "] ¡La CPU hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") Disparo exitoso al submarino!\n";
    }
    else if (tablero.m_tablero[fila][columna] == 'B')
    {
        tablero.m_tablero[fila][columna] = 'X';
        mensajeCPU = "[cliente " + std::to_string(clienteID) + "] ¡La CPU hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") Disparo exitoso a la buque!\n";
    }
    else
    {
        tablero.m_tablero[fila][columna] = 'X';
        mensajeCPU = "[cliente " + std::to_string(clienteID) + "] ¡La CPU hizo un tiro en las coordenadas (" + std::to_string(fila) + ", " + std::to_string(columna) + ") Disparo exitoso al portaaviones!\n";
    }

    std::cout << mensajeCPU;
    enviarDatos(clienteSocket, mensajeCPU);
    enviarDatos(clienteSocket, tableroToString(tablero));
}

std::mutex mtx;

// Función que se ejecuta en el hilo del cliente
void *conexionCliente(void *datosCliente)
{
    DatosCliente *datos = static_cast<DatosCliente *>(datosCliente);
    int clienteSocket = datos->socket;
    int clienteID = datos->id;
    int indiceHilo = datos->indiceHilo;

    char buffer[TAM_BUFFER] = {0};
    Tablero tablero;
    Tablero tableroCpu;

    // Inicializar los tableros
    for (int i = 0; i < TAM_TABLERO; ++i)
    {
        for (int j = 0; j < TAM_TABLERO; ++j)
        {
            tablero.m_tablero[i][j] = MAR;
            tableroCpu.m_tablero[i][j] = MAR;
        }
    }

    colocarBarcosAleatoriamente(tablero, tableroCpu);

    string mensajeBienvenida = "¡Bienvenido al juego de Batalla Naval! Eres el Cliente " + to_string(clienteID) + "\n";
    send(clienteSocket, mensajeBienvenida.c_str(), mensajeBienvenida.length(), 0);
    std::cout << mensajeBienvenida << std::endl;

    // Envío de los tableros iniciales
    enviarDatosDoble(clienteSocket, tableroToString(tablero), tableroToStringCPU(tableroCpu));

    // Ciclo principal del juego
    while (true)
    {
        std::cout << "Esperando ataque del cliente " << clienteID << std::endl;

        // Esperar el ataque del cliente

        int bytesRecibidosFila = recv(clienteSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRecibidosFila <= 0)
        {
            // Error al recibir la fila o el cliente se desconectó
            break;
        }
        buffer[bytesRecibidosFila] = '\0'; // Agregar el terminador nulo al final de los datos recibidos
        int fila = std::stoi(buffer);      // Convertir los datos recibidos a entero
        std::cout << "[cliente " + std::to_string(clienteID) + "] Fila: " << fila << std::endl;

        // Recibir la columna del cliente
        int bytesRecibidosColumna = recv(clienteSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRecibidosColumna <= 0)
        {
            // Error al recibir la columna o el cliente se desconectó
            break;
        }
        buffer[bytesRecibidosColumna] = '\0'; // Agregar el terminador nulo al final de los datos recibidos
        int columna = std::stoi(buffer);      // Convertir los datos recibidos a entero
        std::cout << "[cliente " + std::to_string(clienteID) + "] Columna: " << columna << std::endl;

        // Procesar el ataque del cliente llamando a la función 'procesarAtaqueCliente'
        procesarAtaqueCliente(tablero, tableroCpu, fila, columna, clienteSocket, clienteID);
        std::cout << "[cliente " + std::to_string(clienteID) + "] Se envió el resultado del ataque del cliente" << std::endl;

        // Realizar el ataque de la CPU
        int filaCPU = rand() % TAM_TABLERO;
        int columnaCPU = rand() % TAM_TABLERO;
        procesarAtaqueCPU(tablero, tableroCpu, filaCPU, columnaCPU, clienteSocket, clienteID);
        std::cout << "[cliente " + std::to_string(clienteID) + "] Se envió el resultado del ataque del servidor" << std::endl;

        std::cout << "[cliente " + std::to_string(clienteID) + "] Tableros actualizados:" << std::endl;
    }

    // Cerrar la conexión con el cliente
    close(clienteSocket);
    clientesConectados--;
    cout << "Cliente " << clienteID << " se ha desconectado." << endl;
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Indicar puerto" << std::endl;
        return -1;
    }
    int serverPort = atoi(argv[1]);
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
    direccion.sin_port = htons(serverPort);
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
        std::cout << "Cliente " << datosCliente.id << " conectado PID: " << resultado << std::endl;
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