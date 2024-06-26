#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>

// Estructura para representar un punto con coordenadas x e y
struct Punto {
    int x, y;
    Punto(int _x, int _y) : x(_x), y(_y) {}
};

// Esta función recorre e imprime cada celda del laberinto.
void imprimirLaberinto(const std::vector<std::vector<char>>& laberinto) {
    for (const auto& fila : laberinto) {
        for (const auto& celda : fila) {
            std::cout << celda << ' ';
        }
        std::cout << '\n';
    }
}

// Función para generar el laberinto usando retroceso (Backtracking)
void generarLaberinto(std::vector<std::vector<char>>& laberinto, int x, int y) {
    const int dx[] = { -1, 1, 0, 0 }; // Desplazamientos posibles en x (arriba, abajo, izquierda, derecha)
    const int dy[] = { 0, 0, -1, 1 }; // Desplazamientos posibles en y (arriba, abajo, izquierda, derecha)
    std::vector<int> direcciones = { 0, 1, 2, 3 }; // Direcciones posibles

    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(direcciones.begin(), direcciones.end(), rng); // Mezcla aleatoria de direcciones

    for (int i : direcciones) {
        int nx = x + dx[i] * 2;
        int ny = y + dy[i] * 2;

        // Comprueba si la nueva posición es válida
        if (nx >= 0 && nx < laberinto[0].size() && ny >= 0 && ny < laberinto.size() && laberinto[ny][nx] == '|') {
            laberinto[ny][nx] = ' ';
            laberinto[y + dy[i]][x + dx[i]] = ' ';
            generarLaberinto(laberinto, nx, ny); // Llamada recursiva
        }
    }
}

// Verifica si el punto está dentro de los límites, no es una pared y no ha sido visitado
bool esValido(const std::vector<std::vector<char>>& laberinto, int x, int y, std::vector<std::vector<bool>>& visitado) {
    return x >= 0 && x < laberinto[0].size() && y >= 0 && y < laberinto.size() && laberinto[y][x] != '|' && !visitado[y][x];
}

// Función para resolver el laberinto usando búsqueda en anchura (BFS)
bool resolverLaberinto(std::vector<std::vector<char>>& laberinto) {
    const int dx[] = { -1, 1, 0, 0 };
    const int dy[] = { 0, 0, -1, 1 };
    std::queue<Punto> cola; // Cola para BFS

    // Matriz para marcar las celdas visitadas
    std::vector<std::vector<bool>> visitado(laberinto.size(), std::vector<bool>(laberinto[0].size(), false));
    std::vector<std::vector<Punto>> padre(laberinto.size(), std::vector<Punto>(laberinto[0].size(), Punto(-1, -1)));

    const int inicioX = 1, inicioY = 0; // Coordenadas de inicio
    const int finX = laberinto[0].size() - 2, finY = laberinto.size() - 1; // Coordenadas de fin

    cola.push(Punto(inicioX, inicioY));
    visitado[inicioY][inicioX] = true;

    while (!cola.empty()) {
        Punto p = cola.front();
        cola.pop();

        // Si hemos llegado a la salida
        if (p.x == finX && p.y == finY) {
            while (!(p.x == inicioX && p.y == inicioY)) {
                laberinto[p.y][p.x] = '*'; // Marca el camino solucionado
                p = padre[p.y][p.x];
            }
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];

            if (esValido(laberinto, nx, ny, visitado)) {
                cola.push(Punto(nx, ny));
                visitado[ny][nx] = true;
                padre[ny][nx] = p; // Guarda el punto padre para reconstruir el camino
            }
        }
    }

    return false; // No se encontró solución
}

// Función principal
int main() {
    int ancho, alto;
    std::cout << "Introduce el ancho del laberinto (debe ser un numero impar): ";
    std::cin >> ancho;
    std::cout << "Introduce la altura del laberinto (debe ser un numero impar): ";
    std::cin >> alto;

    if (ancho % 2 == 0 || alto % 2 == 0) {
        std::cerr << "El ancho y la altura deben ser números impares.\n";
        return 1;
    }

    // Inicializa el laberinto con paredes
    std::vector<std::vector<char>> laberinto(alto, std::vector<char>(ancho, '|'));
    laberinto[1][1] = ' '; // Punto de inicio del algoritmo de generación
    generarLaberinto(laberinto, 1, 1);

    // Define las entradas y salidas del laberinto
    laberinto[0][1] = 'S'; // Entrada
    laberinto[alto - 1][ancho - 2] = 'E'; // Salida

    imprimirLaberinto(laberinto); // Muestra el laberinto generado
    std::cout << "Resolviendo el laberinto...\n";

    // Resuelve el laberinto y muestra el resultado
    if (resolverLaberinto(laberinto)) {
        std::cout << "Laberinto resuelto:\n";
        imprimirLaberinto(laberinto);
    } else {
        std::cout << "No se encontró solución.\n";
    }

    return 0;
}