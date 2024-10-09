#include "../include/ship.h"
#include "../lib/cethreads.c"


ship_t* create_ship(shipType_t type, channelSide_t side, short priority) {
    // Asignar memoria dinámicamente para el nuevo barco
    ship_t *newShip = (ship_t *)malloc(sizeof(ship_t));


    // Inicializar los valores del barco
    newShip->type = type;
    newShip->side = side;
    newShip->priority = priority;
    newShip->position = 0;  // Revisar lista de barcos y tomar la última posición o la debida

    // Crear el hilo para mover el barco y obtener el ID
    int id = CEthread_create(&move_ship, newShip);  // Pasar el puntero directamente
    printf("creation id %d\n", id);

    // Asignar el ID del hilo al barco
    newShip->threadId = id;

    // Retornar el puntero al barco recién creado
    return newShip;
}

void move_ship(ship_t *ship){
    for (int i = 0; i < 10; i++)
    {
        CEmutex_trylock();
        ship->position++;
        printf("ship position: %d \n", ship->position);
        CEmutex_unlock();
		sleep( 1 );
		CEthread_yield();
    }
    
}