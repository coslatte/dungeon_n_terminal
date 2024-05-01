#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

const char ROAD = 0;     // Los caminos son representados con espacios vacíos ' ', y tienen valor 0 en el mapa.
const char WALL = 1;     // Las paredes son representadas con 'X', y tienen valor 1 en el mapa.
const char ENEMY = 2;    // Los enemigos se representan con la letra 'E', y tienen valor 2 en el mapa.
const char HERO = 3;     // El héroe se representa con la letra 'H', y tienen valor 3 en el mapa.
const char UNKNOWN = -1; // Valor que se asigna a un tipo de entidad desconocido.

// Mapa 1.
char map[8][8] = {
    {1, 0, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 0, 1}};

/**
 * Define una entidad en el mapa.
 *
 * @param n
 * Fila donde se definirá la entidad.
 * @param m
 * Columna donde se definirá la entidad.
 * @param entity_type
 * Tipo de la entidad a definir. Si no es un tipo conocido, se muestra un mensaje en la consola y se cierra la aplicación.
 */
void set_entity(int n, int m, char entity_type)
{
    if (map[n][m] != WALL)
    {
        map[n][m] = entity_type;
    }
    else
    {
        printf("Alto error elermano, se trato de insertar una entidad en una pared xd\n");
    }
}

/**
 * Actualiza la posición de la entidad en el mapa.
 *
 * @param n
 * Fila donde se encuentra.
 * @param m
 * Columna donde se encuentra.
 * @param x
 * Fila donde se movió.
 * @param y
 * Columna donde se movió.
 * @param entity_type
 * El tipo de entidad que se va a mover (2: enemy, 3: hero, etc..).
 */
void update_entity_position(int n, int m, int x, int y, char entity_type)
{
    if (entity_type == UNKNOWN || entity_type == WALL)
    {
        printf("No se pudo actualizar la posición de la entidad del tipo %c. \n", entity_type);
        return;
    }
    map[n][m] = ROAD;        // Se elimina donde sea que se encontraba la entidad.
    map[x][y] = entity_type; // Entidad se traslada.
}

/**
 * Traduce los valores del tablero del mapa a diversos caracteres más representativos.
 *
 * @param character
 * El caracter a traducir.
 */
char translate(char character)
{
    switch (character)
    {
    case 0:
        return ' ';
        break;
    case 1:
        return 'X';
        break;
    case 2:
        return 'E';
        break;
    case 3:
        return 'H';
        break;
    default:
        return '?';
        break;
    }
}

/**
 * Renderiza en el terminal de comandos el calabozo (el mapa).
 */
void render_dungeon()
{
    for (size_t i = 0; i < 8; i++)
    {
        for (size_t j = 0; j < 8; j++)
        {
            printf("%c", translate(map[i][j]));
        }
        printf("\n");
    }
}

/**
 * Determina si existe el héroe o no.
 *
 * @return `true` si se encuentra el héroe, `false` en caso contrario.
 */
bool hero_exist()
{
    for (size_t i = 0; i < 8; i++)
        for (size_t j = 0; j < 8; j++)
            if (map[i][j] == 3)
                return true;
    return false;
}

/**
 * Devuelve un puntero de tipo char a un arreglo con un par de elementos (x, y)
 * que contiene las coordenadas del héroe.
 *
 * @returns
 * `pos[0] = y, pos[1] = x` --> `[y, x]`. Una especie de tupla con los valores de `x` y `y`
 * donde se encuentra el héroe en la matriz.
 */
int *get_hero_position()
{
    // En este caso, las 'x' son las ordenadas, y las 'y' las absisas, ojo.
    /*
      0 1 2 3
    0 x x x x
    1 x x x x
    2 x x x x
    3 x x h x
    ------------------------------------
    filas = y
    columnas = x
    ------------------------------------
    h = [y, x] = [3, 2] = [y = 3, x = 2]
    */
    for (size_t y = 0; y < 8; y++)
        for (size_t x = 0; x < 8; x++)
            if (map[y][x] == HERO)
            {
                int *pos = (int *)malloc(2 * sizeof(int)); // Reservar memoria para el puntero.
                pos[0] = (int)y;                           // [y, *] = pos[0]
                pos[1] = (int)x;                           // [*, x] = pos[1]
                return pos;                                // [y, x] --> Puntero a estos valores.
            }
    return NULL;
}

/**
 * Función que verifica las colisiones circundantes.
 *
 * @param direction De acuerdo a su valor (1: arriba, 2: abajo, 3: izq, 4: der) se verifica si existe colisión o no.
 *
 * @return Devuelve `true` si existe colisión en la dirección indicada, `false` si no existe.
 */
bool __check_collision(char direction)
{
    // 1: arriba, 2: abajo, 3: izq, 4: der
    /*
     * ↑: x = 0, y = 1
     * -----
     * ↓: x = 0, y = -1
     * -----
     * ←: x = -1, y = 0
     * -----
     * →: x = 1, y = 0
     */

    int *hero_position = get_hero_position();
    // TODO! Ver asunto de colisiones aqui
    if ( // Comprobando héroe no se salga del mapa.
        (hero_position[0] == 8 || hero_position[1] == 8) ||
        (hero_position[0] == -1 || hero_position[1] == -1))
    {
        return true;
    }
    else
    {
        switch (direction)
        {
        case 1:
            return (map[hero_position[0] - 1][hero_position[1]] == WALL) ? true : false;
            break;
        case 2:
            return (map[hero_position[0] + 1][hero_position[1]] == WALL) ? true : false;
            break;
        case 3:
            return (map[hero_position[0]][hero_position[1] - 1] == WALL) ? true : false;
            break;
        case 4:
            return (map[hero_position[0]][hero_position[1] + 1] == WALL) ? true : false;
            break;
        default:
            return true;
            break;
        }
    }
    free(hero_position);
}

/**
 * Mueve el héroe hacia arriba. Antes del movimiento se comprueban las colisiones con paredes.
 */
void _move_hero_up()
{
    // 1: arriba, 2: abajo, 3: izq, 4: der
    if (!__check_collision(1))
    {
        int *hero_pos = get_hero_position();
        update_entity_position(hero_pos[0], hero_pos[1], hero_pos[0] - 1, hero_pos[1], 3);
        free(hero_pos);
    }
    else
    {
        printf("Colision ARRIBA.\n");
        Sleep(1500);
    }
}

/**
 * Mueve el héroe hacia abajo. Antes del movimiento se comprueban las colisiones con paredes.
 */
void _move_hero_down()
{
    // 1: arriba, 2: abajo, 3: izq, 4: der
    if (!__check_collision(2))
    {
        int *hero_pos = get_hero_position();
        // printf("Antes >> [%d, %d]\n", hero_pos[0], hero_pos[1]);
        // printf("Despu >> [%d, %d]\n", hero_pos[0] + 1, hero_pos[1]);
        update_entity_position(hero_pos[0], hero_pos[1], hero_pos[0] + 1, hero_pos[1], 3);
        free(hero_pos);
    }
    else
    {
        printf("Colision ABAJO.\n");
        Sleep(1500);
    }
}

/**
 * Mueve el héroe hacia la izquierda. Antes del movimiento se comprueban las colisiones con paredes.
 */
void _move_hero_left()
{
    // 1: arriba, 2: abajo, 3: izq, 4: der
    if (!__check_collision(3))
    {
        int *hero_pos = get_hero_position();
        update_entity_position(hero_pos[0], hero_pos[1], hero_pos[0], hero_pos[1] - 1, 3);
        free(hero_pos);
    }
    else
    {
        printf("Colision IZQUIERDA.\n");
        Sleep(1500);
    }
}

/**
 * Mueve el héroe hacia la derecha. Antes del movimiento se comprueban las colisiones con paredes.
 */
void _move_hero_right()
{
    // 1: arriba, 2: abajo, 3: izq, 4: der
    if (!__check_collision(4))
    {
        int *hero_pos = get_hero_position();
        update_entity_position(hero_pos[0], hero_pos[1], hero_pos[0], hero_pos[1] + 1, 3);
        free(hero_pos);
    }
    else
    {
        printf("Colision DERECHA.\n");
        Sleep(1500);
    }
}

/**
 * A partir de un caracter que se ingrese por consola, se toma una acción en el juego.
 *
 * Acciones de movimiento del héroe
 * --------------------------------
 *
 * `W`: Mueve el héroe hacia arriba.
 * `S`: Mueve el héroe hacia abajo.
 * `A`: Mueve el héroe hacia izquierda.
 * `D`: Mueve el héroe hacia derecha.
 *
 * Acciones de menú y control del juego
 * ------------------------------------
 *
 * `C`: Sale del juego.
 */
void action(char operation)
{
    if (
        operation == 'w' ||
        operation == 's' ||
        operation == 'a' ||
        operation == 'd' ||
        operation == 'c')
    {
        operation -= 32; // convertir a mayúscula...
    }
    if (hero_exist())
    {
        switch (operation)
        {
        case 'W':
            _move_hero_up();
            break;
        case 'S':
            _move_hero_down();
            break;
        case 'A':
            _move_hero_left();
            break;
        case 'D':
            _move_hero_right();
            break;
        case 'C': // Para salir del juego.
            exit(1);
            break;
        default:
            break;
        }
    }
}

/**
 * Para entrada de instrucciones por consola.
 * Básicamente el medio de interacción con el juego.
 */
char get_operation()
{
    char buf[100];
    scanf("%c", buf);
    return buf[0];
}

/**
 * Funcion principal del juego.
 */
int main()
{
    set_entity(7, 6, 3); // Héroe.
    set_entity(6, 3, 2); // Enemigo.
    set_entity(3, 6, 2); // Enemigo.

    char operation;
    for (;;)
    {
        system("cls");
        printf("\n");
        render_dungeon();
        printf("Accion: ");
        operation = get_operation();
        action(operation);
        system("cls");
    }
    printf("Se termino el programa.");
    Sleep(5000);
    return 0;
}