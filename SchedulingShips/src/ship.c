#include "../include/ship.h"

ship_t* create_ship(shipType_t type, channelSide_t side, short priority, int position) {
    // Asignar memoria dinámicamente para el nuevo barco
    ship_t *newShip = (ship_t *)malloc(sizeof(ship_t));


    // Inicializar los valores del barco
    newShip->type = type;
    newShip->side = side;
    newShip->priority = priority;
    newShip->position = position;  // Revisar lista de barcos y tomar la última posición o la debida

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
        
        printf("before ship position: %d \n", ship->position);
        if (ship->side == LEFT) {
            ship->position = ship->position + ship->type;
        }else {
            ship->position = ship->position - ship->type;
        }
        printf("after ship position: %d \n", ship->position);

        CEmutex_unlock();
		sleep( 1 );
		CEthread_yield();
    }
    
}

/**
 * @brief Initializes an empty ship list.
 * @param list Pointer to the ship list to be initialized.
 */
void initList(ShipList* list) {
    list->head = NULL;
}

/**
 * @brief Adds a ship to the beginning of the ship list.
 * @param list Pointer to the ship list.
 * @param ship Pointer to the ship to be added.
 */
void addShip(ShipList* list, ship_t* ship) {
    ShipNode* newNode = (ShipNode*)malloc(sizeof(ShipNode));

    newNode->ship = ship;
    newNode->next = list->head;
    list->head = newNode;
}

/**
 * @brief Removes a ship from the list by its thread ID.
 * @param list Pointer to the ship list.
 * @param id The thread ID of the ship to be removed.
 */
void removeShip(ShipList* list, int id) {
    ShipNode* current = list->head;
    ShipNode* previous = NULL;

    while (current != NULL && current->ship->threadId != id) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Ship with ID %d not found.\n", id);
        return;
    }

    if (previous == NULL) {
        list->head = current->next;
    } else {
        previous->next = current->next;
    }

    free(current->ship);
    free(current);
    printf("Ship with ID %d removed.\n", id);
}

/**
 * @brief Finds a ship in the list by its thread ID.
 * @param list Pointer to the ship list.
 * @param id The thread ID of the ship to find.
 * @return Pointer to the found ship node, or NULL if not found.
 */
ShipNode* findShip(ShipList* list, int id) {
    ShipNode* current = list->head;
    while (current != NULL) {
        if (current->ship->threadId == id) {
            return current;
        }
        current = current->next;
    }
    return NULL; // Not found
}

/**
 * @brief Retrieves the last ship in the list.
 * @param list Pointer to the ship list.
 * @return Pointer to the last ship node, or NULL if the list is empty.
 */
ship_t* getLastShip(ShipList* list) {
    if (list->head == NULL) {
        return NULL; // La lista está vacía
    }

    ShipNode* current = list->head;
    while (current->next != NULL) {
        current = current->next; // Avanzar al siguiente nodo hasta llegar al último
    }

    return current->ship; // Retornar el último nodo
}

int getNextShipPosition(ShipList* list, channelSide_t side) {
    ship_t* tempShip = getLastShip(list);

    if (tempShip == NULL){
        if (side == LEFT) {
            return 0;
        }else {
            return CHANNEL_SIZE+1;
        }
    }

    if (side == LEFT) {
        return tempShip->position--;
    }else {
        return tempShip->position++;
    }
}

/**
 * @brief Sorts the ships in the list by their priority in descending order.
 * @param list Pointer to the ship list to be sorted.
 */
void sortShipsByPriority(ShipList* list) {
    if (list->head == NULL || list->head->next == NULL) {
        return;
    }

    int swapped;
    ShipNode* current;
    ShipNode* last = NULL;

    do {
        swapped = 0;
        current = list->head;

        while (current->next != last) {
            if (current->ship->priority < current->next->ship->priority) {
                ship_t* temp = current->ship;
                current->ship = current->next->ship;
                current->next->ship = temp;
                swapped = 1;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);
}

/**
 * @brief Prints the details of all ships in the list.
 * @param list Pointer to the ship list to be printed.
 */
void printList(const ShipList* list) {
    ShipNode* current = list->head;
    while (current != NULL) {
        printf("Ship ID: %d, Type: %d, Priority: %d, Side: %d, Position: %d\n", 
            current->ship->threadId, current->ship->type, current->ship->priority, 
            current->ship->side, current->ship->position);
        current = current->next;
    }
}
