// ship.c

#include "../include/linkedList.h"
#include "../lib/cethreads.c"


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
