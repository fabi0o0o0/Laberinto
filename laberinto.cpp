#include <iostream>  // Incluye la biblioteca para manejar la entrada y salida estándar
#include <vector>    // Incluye la biblioteca para usar el contenedor vector
#include <queue>     // Incluye la biblioteca para usar el contenedor queue
#include <stack>     // Incluye la biblioteca para usar el contenedor stack
#include <ctime>     // Incluye la biblioteca para manejar el tiempo
#include <cstdlib>   // Incluye la biblioteca para funciones generales como rand() y srand()
#include <algorithm> // Incluye la biblioteca para usar funciones de algoritmos como shuffle
#include <random>    // Incluye la biblioteca para generar números aleatorios

// Definición de las direcciones de movimiento
const int DX[4] = {1, 0, -1, 0}; // Movimiento en el eje x: derecha, sin movimiento, izquierda, sin movimiento
const int DY[4] = {0, 1, 0, -1}; // Movimiento en el eje y: sin movimiento, arriba, sin movimiento, abajo

// Clase para representar el laberinto
class Laberinto {
public:
    // Constructor que inicializa el laberinto con paredes y el vector de celdas visitadas
    Laberinto(int ancho, int alto) : ancho(ancho), alto(alto) {
        laberinto = std::vector<std::vector<char>>(alto, std::vector<char>(ancho, '#')); // Inicializa el laberinto lleno de paredes
        visitado = std::vector<std::vector<bool>>(alto, std::vector<bool>(ancho, false)); // Inicializa el vector de celdas visitadas como falso
    }

    // Función para generar el laberinto
    void generar() {
        srand(time(0)); // Inicializa la semilla para la generación de números aleatorios
        tallarPasaje(1, 1); // Comienza a tallar el laberinto desde la posición (1, 1)
        laberinto[1][0] = 'E'; // Marca la entrada del laberinto
        laberinto[alto - 2][ancho - 1] = 'S'; // Marca la salida del laberinto
    }

    // Función para imprimir el laberinto
    void imprimir() const {
        for (const auto& fila : laberinto) { // Itera sobre cada fila del laberinto
            for (char celda : fila) { // Itera sobre cada celda de la fila
                std::cout << celda; // Imprime la celda
            }
            std::cout << '\n'; // Salta a la siguiente línea después de imprimir una fila
        }
    }

    // Función para resolver el laberinto
    bool resolver() {
        std::queue<std::pair<int, int>> cola; // Cola para realizar la búsqueda en anchura (BFS)
        std::vector<std::vector<bool>> visitado(alto, std::vector<bool>(ancho, false)); // Vector de celdas visitadas
        std::vector<std::vector<std::pair<int, int>>> padre(alto, std::vector<std::pair<int, int>>(ancho, {-1, -1})); // Vector para rastrear el camino
        cola.push({1, 0}); // Empieza la búsqueda desde la entrada
        visitado[1][0] = true; // Marca la entrada como visitada

        while (!cola.empty()) { // Mientras haya celdas en la cola
            auto [x, y] = cola.front(); // Toma la celda al frente de la cola
            cola.pop(); // Remueve la celda de la cola

            if (x == alto - 2 && y == ancho - 1) { // Si se ha llegado a la salida
                marcarCamino(padre, x, y); // Marca el camino de la solución en el laberinto
                return true; // Retorna verdadero indicando que se encontró una solución
            }

            for (int i = 0; i < 4; ++i) { // Itera sobre las direcciones de movimiento
                int nx = x + DX[i]; // Calcula la nueva posición en x
                int ny = y + DY[i]; // Calcula la nueva posición en y

                if (esValido(nx, ny) && laberinto[nx][ny] != '#' && !visitado[nx][ny]) { // Si la nueva posición es válida, no es una pared y no ha sido visitada
                    visitado[nx][ny] = true; // Marca la nueva posición como visitada
                    padre[nx][ny] = {x, y}; // Guarda el padre de la nueva posición para rastrear el camino
                    cola.push({nx, ny}); // Añade la nueva posición a la cola
                }
            }
        }
        return false; // Retorna falso indicando que no se encontró una solución
    }

private:
    int ancho, alto; // Ancho y alto del laberinto
    std::vector<std::vector<char>> laberinto; // Matriz para representar el laberinto
    std::vector<std::vector<bool>> visitado; // Matriz para rastrear las celdas visitadas durante la generación del laberinto

    // Función para verificar si una posición es válida dentro del laberinto
    bool esValido(int x, int y) const {
        return x >= 0 && x < alto && y >= 0 && y < ancho; // La posición es válida si está dentro de los límites del laberinto
    }

    // Función para tallar pasajes en el laberinto
    void tallarPasaje(int x, int y) {
        visitado[x][y] = true; // Marca la posición actual como visitada
        laberinto[x][y] = ' '; // Crea un pasaje en la posición actual

        std::vector<int> dirs = {0, 1, 2, 3}; // Vector de direcciones de movimiento
        std::random_device rd; // Dispositivo para generación de números aleatorios
        std::mt19937 g(rd()); // Generador de números aleatorios
        std::shuffle(dirs.begin(), dirs.end(), g); // Mezcla aleatoriamente las direcciones de movimiento

        for (int dir : dirs) { // Itera sobre las direcciones de movimiento mezcladas
            int nx = x + DX[dir] * 2; // Calcula la nueva posición en x (movimiento de dos celdas)
            int ny = y + DY[dir] * 2; // Calcula la nueva posición en y (movimiento de dos celdas)

            if (esValido(nx, ny) && !visitado[nx][ny]) { // Si la nueva posición es válida y no ha sido visitada
                laberinto[x + DX[dir]][y + DY[dir]] = ' '; // Crea un pasaje entre la posición actual y la nueva posición
                tallarPasaje(nx, ny); // Llama recursivamente para tallar pasajes desde la nueva posición
            }
        }
    }

    // Función para marcar el camino de la solución en el laberinto
    void marcarCamino(const std::vector<std::vector<std::pair<int, int>>>& padre, int x, int y) {
        while (x != 1 || y != 0) { // Mientras no se haya regresado a la entrada
            laberinto[x][y] = '*'; // Marca la posición actual como parte del camino de la solución
            auto [px, py] = padre[x][y]; // Obtiene la posición del padre
            x = px; // Actualiza la posición actual a la posición del padre
            y = py; // Actualiza la posición actual a la posición del padre
        }
    }
};

int main() {
    int ancho, alto; // Ancho y alto del laberinto
    std::cout << "Ingrese el ancho del laberinto: "; // Solicita el ancho del laberinto
    std::cin >> ancho; // Lee el ancho del laberinto desde la entrada estándar
    std::cout << "Ingrese la altura del laberinto: "; // Solicita la altura del laberinto
    std::cin >> alto; // Lee la altura del laberinto desde la entrada estándar

    // Asegurarse de que el tamaño del laberinto sea impar
    if (ancho % 2 == 0) ancho += 1; // Si el ancho es par, lo incrementa en 1 para hacerlo impar
    if (alto % 2 == 0) alto += 1; // Si la altura es par, la incrementa en 1 para hacerla impar

    Laberinto laberinto(ancho, alto); // Crea un objeto Laberinto con el ancho y alto especificados
    laberinto.generar(); // Genera el laberinto

    std::cout << "Laberinto generado:\n"; // Imprime un mensaje indicando que el laberinto ha sido generado
    laberinto.imprimir(); // Imprime el laberinto generado

    if (laberinto.resolver()) { // Si se encuentra una solución para el laberinto
        std::cout << "Se encontró una solución para el laberinto:\n"; // Imprime un mensaje indicando que se encontró una solución
        laberinto.imprimir(); // Imprime el laberinto con la solución marcada
    } else { // Si no se encuentra una solución para el laberinto
        std::cout << "No se encontró una solución para el laberinto.\n"; // Imprime un mensaje indicando que no se encontró una solución
    }

    return 0; // Retorna 0 indicando que el programa terminó correctamente
}
