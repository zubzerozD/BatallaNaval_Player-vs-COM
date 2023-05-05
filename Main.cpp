#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <map>
#include <set>
#include <utility>
#include <queue>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <stdio.h>
#include <ncurses.h>

using namespace std;

class enemigo
{
private:
public:
    void pintar();
};

void enemigo::pintar()
{
    move(5, 5);cout << "O\n";
}

int main()
{
    enemigo a;
    a.pintar();
    return 0;
}