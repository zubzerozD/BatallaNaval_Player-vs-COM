#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <iostream>

using namespace std;

class tablero {
    public: tablero();
    public: void mostrar();
    public: void insertar(char, int, int);
    public: bool ganador(char);
    public: bool lleno();

    void disparar(int fila, int columna, int jugador);
    void limpiarTablero();

};
