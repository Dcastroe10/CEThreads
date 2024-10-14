#include "../include/ship.h"
#include <stdio.h>

/**
 * @brief Creates a new ship with specified properties and starts its associated thread.
 * @param type The type of the ship (e.g., NORMAL, FISHING, PATROL).
 * @param side The side of the channel where the ship starts (LEFT or RIGHT).
 * @param priority The priority level of the ship.
 * @param position The starting position of the ship in the channel.
 * @return Pointer to the newly created ship structure.
 */
ship_t* create_ship(shipType_t type, channelSide_t side, short priority, int position) {
    // Dynamically allocate memory for the new ship
    ship_t *newShip = (ship_t *)malloc(sizeof(ship_t));

    // Initialize the ship values
    newShip->type = type;
    newShip->side = side;
    newShip->priority = priority;
    newShip->position = position; 
    newShip->time = CHANNEL_SIZE / type;

    // Create the thread to move the ship and get the ID
    //int id = CEthread_create(&move_ship, newShip);  //----------------------------------------------
    int id = CEthread_create((void (*)(void *))move_ship, (void *)newShip);

    printf("creation id %d\n", id);

    // Assign the thread ID to the ship
    newShip->threadId = id;

    // Return the pointer to the newly created ship
    return newShip;
}

/**
 * @brief Returns the execution context from the current thread back to the main context.
 * This function is used to temporarily suspend the execution of the current thread and
 * return control back to the main scheduler or main execution flow.
 */
void returnContext() {
    swapcontext(&cethreadList[currentcethread].context, &mainContext);
}

/**
 * @brief Simulates the movement of a ship through the channel.
 * This function is executed by each thread representing a ship.
 * The ship keeps moving until it crosses the channel completely.
 * @param ship Pointer to the ship structure that contains its movement information.
 */
void move_ship(ship_t *ship) {
    // Keep moving until the ship crosses the channel
    while (1)
    {
        // Try to lock the mutex to ensure safe access to shared resources
        CEmutex_trylock();
        
        printf("\nthread id: %d \n", ship->threadId);
        printf("before change ship position: %d \n", ship->position);

        // Update the ship's position based on its side
        if (ship->side == LEFT) {
            // If the ship is on the LEFT side, move it forward (rightward)
            ship->position = ship->position + ship->type;

            // Check if the ship has crossed the channel
            if (ship->position > CHANNEL_SIZE) {
                CEthread_end();
            }
        } else {
            // If the ship is on the RIGHT side, move it backward (leftward)
            ship->position = ship->position - ship->type;

            // Check if the ship has crossed the channel
            if (ship->position <= 0) {
                CEthread_end();
            }
        }

        printf("after change ship position: %d \n", ship->position);

        // Unlock the mutex to allow other ships to access shared resources
        CEmutex_unlock();

        // Sleep for a defined period to simulate movement delay
        usleep(500000);

        // Return control back to the main context to allow other threads to execute
		returnContext();
    }
}

/**
 * @brief Initializes an empty ship list.
 * @param list Pointer to the ship list to be initialized.
 */
void initList(ShipList* list) {
    list->head = NULL;
}

void print_desde_Ship() {
    printf(" ESTO SE IMPRIME DESDE Ship: ");
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
 * @brief Counts the number of ships in the list.
 * @param list Pointer to the ship list.
 * @return The number of ships in the list.
 */
int getShipCount(ShipList* list) {
    int count = 0;
    ShipNode* current = list->head;
    
    // Traverse the list and count each node.
    while (current != NULL) {
        count++;
        current = current->next;
    }
    
    return count;
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
 * @brief Retrieves a ship node from the list by its index.
 * @param list Pointer to the ship list.
 * @param index The index of the ship node to retrieve (0-based).
 * @return Pointer to the ship node at the specified index, or NULL if the index is out of bounds.
 */
ShipNode* getShipByIndex(ShipList* list, int index) {
    ShipNode* current = list->head;
    int count = 0;

    // Iterate through the list until the specified index is reached
    while (current != NULL) {
        if (count == index) {
            return current; // Return the node if the index matches
        }
        count++;
        current = current->next; // Move to the next node in the list
    }

    // Return NULL if the index is out of bounds (greater than the number of ships in the list)
    return NULL; 
}

/**
 * @brief Retrieves the ID of the first ship in the list.
 * @param list Pointer to the ship list.
 * @return The ID of the first ship, or -1 if the list is empty.
 */
int getFirstShipID(ShipList* list) {
    if (list->head == NULL) {
        return -1; // La lista está vacía
    }

    return list->head->ship->threadId; // Retornar el ID del primer barco/hilo
}

/**
 * @brief Retrieves the thread ID of a ship at a specific position in the list.
 * @param list Pointer to the ship list.
 * @param position The index position (0-based) in the list.
 * @return The thread ID of the ship at the specified position, or -1 if the position is invalid.
 */
int getShipIdByPosition(ShipList* list, int position) {
    ShipNode* node = getShipByIndex(list, position);
    if (node == NULL) {
        return -1; // The position is invalid or does not exist.
    }

    return node->ship->threadId;
}

/**
 * @brief Retrieves a ship by its position in the list.
 * @param list Pointer to the ship list.
 * @param position The index position of the ship in the list (starting from 0).
 * @return Pointer to the ship if found, or NULL if the position is invalid or the ship does not exist.
 */
ship_t* getShipByPosition(ShipList* list, int position) {
    ShipNode* node = getShipByIndex(list, position);
    if (node == NULL) {
        return NULL; // The position is invalid or does not exist.
    }

    return node->ship;
}

/**
 * @brief Retrieves a ship by its ID from the list.
 * @param list Pointer to the ship list.
 * @param id The ID of the ship to search for.
 * @return Pointer to the ship if found, or NULL if no ship with the specified ID exists.
 */
ship_t* getShipById(ShipList* list, int id) {
    if (list->head == NULL) {
        return NULL; // The list is empty
    }

    ShipNode* current = list->head;
    
    // Traverse the list to find the ship with the matching ID
    while (current != NULL) {
        if (current->ship->threadId == id) {
            return current->ship; // Ship with the matching ID found
        }
        current = current->next; // Move to the next node
    }

    return NULL; // No ship with the specified ID found
}

/**
 * @brief Retrieves the last ship in the list.
 * @param list Pointer to the ship list.
 * @return Pointer to the last ship node, or NULL if the list is empty.
 */
ship_t* getLastShip(ShipList* list) {
    if (list->head == NULL) {
        return NULL; // The list is empty
    }

    ShipNode* current = list->head;
    while (current->next != NULL) {
        current = current->next; // Move to the next node until reaching the last one
    }

    return current->ship; // Return the last node
}

/**
 * @brief Determines the next position for a ship based on the side of the channel.
 * @param list Pointer to the ship list.
 * @param side The side of the channel (LEFT or RIGHT) where the ship is located.
 * @return The next position for the ship in the list.
 *         If the list is empty, returns 0 for LEFT side or CHANNEL_SIZE + 1 for RIGHT side.
 */
int getNextShipPosition(ShipList* list, channelSide_t side) {
    // Retrieve the last ship in the list to determine the next position.
    ship_t* tempShip = getLastShip(list);

    // If the list is empty (no ships present), set the starting position.
    if (tempShip == NULL){
        if (side == LEFT) {
            return 0; // Start position for ships on the left side.
        } else {
            return CHANNEL_SIZE+1; // Start position for ships on the right side.
        }
    }

    // If there are ships in the list, calculate the next position based on the side.
    if (side == LEFT) {
        return tempShip->position--;
    } else {
        return tempShip->position++;
    }
}

/**
 * @brief Updates the position of all ships in the waiting line.
 * @param list Pointer to the ship list.
 */
void updateWaitingLine(ShipList* list) {
    for (int i=0; i<getShipCount(list); i++) {
        asignShipPosition(getShipByPosition(list, i), i);
    }
}

/**
 * @brief Reasign ship position according with the scheduler.
 * @param ship Pointer to the ship.
 * @param index Relative position on the list.
 */
void asignShipPosition(ship_t* ship, int index) {
    if (ship->side == LEFT && !(ship->position > 0)) {
        ship->position = -index;
    }
    else if (ship->side == RIGHT && !(ship->position < CHANNEL_SIZE)) {
        ship->position = CHANNEL_SIZE + index + 1;
    }
    else {
        return;
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

    updateWaitingLine(list);
}

/**
 * @brief Sorts the ships in the list based on their remaining time in ascending order.
 * @param list Pointer to the ship list to be sorted.
 */
void sortShipsByShortestTime(ShipList* list) {
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
            if (current->ship->time > current->next->ship->time) {
                ship_t* temp = current->ship;
                current->ship = current->next->ship;
                current->next->ship = temp;
                swapped = 1;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);

    updateWaitingLine(list);
}

/**
 * @brief Combines two ship lists into a single list.
 * @param leftList Pointer to the left-side ship list.
 * @param rightList Pointer to the right-side ship list.
 * @return Pointer to the combined ship list.
 */
ShipList* combineShipLists(ShipList* leftList, ShipList* rightList) {
    ShipList* combinedList = (ShipList*)malloc(sizeof(ShipList));
    if (!combinedList) {
        fprintf(stderr, "Memory allocation failed for combinedList.\n");
        return NULL;
    }
    initList(combinedList);

    // Copy ships from the left list to the combined list
    ShipNode* current = leftList->head;
    while (current != NULL) {
        addShip(combinedList, current->ship);
        current = current->next;
    }

    // Copy ships from the right list to the combined list
    current = rightList->head;
    while (current != NULL) {
        addShip(combinedList, current->ship);
        current = current->next;
    }

    return combinedList;
}

/**
 * @brief Moves a ship identified by its thread ID to the end of the list.
 * @param list Pointer to the ship list.
 * @param shipId The thread ID of the ship to be moved.
 */
void moveShipToEnd(ShipList* list, int shipId) {
    ShipNode* current = list->head;
    ShipNode* previous = NULL;
    ShipNode* targetNode = NULL;

    // Find the target ship and its previous node
    while (current != NULL && current->ship->threadId != shipId) {
        previous = current;
        current = current->next;
    }

    // If the ship was not found, return
    if (current == NULL) {
        printf("Ship with ID %d not found in the list.\n", shipId);
        return;
    }

    // If the ship is already at the end, no need to move it
    if (current->next == NULL) {
        return;
    }

    // Remove the node from its current position
    if (previous == NULL) {
        // The target node is the head of the list
        list->head = current->next;
    } else {
        previous->next = current->next;
    }

    // Move the target node to the end of the list
    targetNode = current;
    targetNode->next = NULL;

    // Find the last node in the list
    ShipNode* last = list->head;
    if (last == NULL) {
        // If the list is empty after removing the node, make the targetNode the head
        list->head = targetNode;
    } else {
        while (last->next != NULL) {
            last = last->next;
        }
        // Attach the targetNode at the end of the list
        last->next = targetNode;
    }

    printf("Ship with ID %d moved to the end of the list.\n", shipId);
}

/**
 * @brief Prints the details of all ships in the list.
 * @param list Pointer to the ship list to be printed.
 */
void printList(const ShipList* list) {
    ShipNode* current = list->head;
    while (current != NULL) {
        printf("Ship ID: %d, Type: %d, Time: %d, Priority: %d, Side: %d, Position: %d\n", 
            current->ship->threadId, current->ship->type, current->ship->time, current->ship->priority, 
            current->ship->side, current->ship->position);
        current = current->next;
    }
}
