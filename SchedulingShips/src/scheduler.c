#include "ship.c"

/* *********** PENDIENTE *********** */
//Preguntar por el tipo de flujo (equidad, letrero y tico)
//Si es equidad, consultar por EQUITY_W y permite que n barcos se muevan de izquierda a derecha y viceversa
//Si es letrero, definir por cuanta tiempo solo pasan barcos de un lado y luego del otro
//SI es tico, un random para definir de qué lado se mueve un barco

//Usar y probar sortShipsByPriority para tener listo el calendarizador de Equidad
//FIFO es el mismo que FCFS
//Luego sería RoundRobin que se mueve hasta donde llegue con el Quatum que le de
//SJF creo que toma en cuenta la velocidad del barco y la prioridad
//Tiempo_real se debe medir cuánta tarda un barco de x tipo en pasar el canal


// Function to handle the scheduling
void handle_scheduler(scheduler_t scheduler, ShipList* leftSideShips, ShipList* rightSideShips) {
    switch (scheduler) {
        case RR:
            printf("\nExecuting Round Robin scheduling.\n");
            break;

        case PRIORITY:
            printf("\nExecuting Priority-based scheduling.\n");

            sortShipsByPriority(leftSideShips);
            sortShipsByPriority(rightSideShips);

            // Print the ships in each list for verification
            printf("\nShips on the left side sorted by priority:\n");
            printList(leftSideShips);
            printf("\nShips on the right side orted by priority:\n");
            printList(rightSideShips);

            break;

        case SJF:
            printf("\nExecuting Shortest Job First scheduling.\n");
            break;

        case FCFS:
            printf("\nExecuting First Come First Served scheduling.\n");
            break;

        case REAL_TIME:
            printf("\nExecuting Real-Time scheduling.\n");
            break;

        default:
            printf("\nUnknown scheduling algorithm.\n");
            break;
    }
}

// Function to handle the workflow
void handle_workflow(workflow_t workflow, ShipList* leftSideShips, ShipList* rightSideShips) {
    channelSide_t side;
    switch (workflow) {
        case EQUITY:
            printf("\nProcessing EQUITY workflow.\n");
            
            // Preallocating the shipsRow array with the maximum possible size
            int* shipsRow = (int*)malloc(EQUITY_W * sizeof(int));
            if (!shipsRow) {
                // Handle memory allocation failure
                fprintf(stderr, "Memory allocation failed for shipsRow.\n");
                break;
            }

            // Randomly decide the initial side
            side = rand() % 2 == 0 ? LEFT : RIGHT;
            ShipList* currentList = (side == LEFT) ? leftSideShips : rightSideShips;
            ShipList* otherList = (side == LEFT) ? rightSideShips : leftSideShips;

            while (getShipCount(leftSideShips) > 0 || getShipCount(rightSideShips) > 0) {
                int shipsRowSize = (getShipCount(currentList) >= EQUITY_W) ? EQUITY_W : getShipCount(currentList);
                
                // Fill shipsRow with ship IDs up to shipsRowSize
                for (int i = 0; i < shipsRowSize; i++) {
                    shipsRow[i] = getShipIdByPosition(currentList, i);
                }

                int shipsCrossed = 0;
                while (shipsCrossed < shipsRowSize) {
                    for (int i = 0; i < shipsRowSize; i++) {
                        if (shipsRow[i] != -1) {
                            int active = set_context(shipsRow[i]);
                            if (active == 0) {
                                removeShip(currentList, shipsRow[i]);
                                shipsRow[i] = -1; // Mark as crossed
                                shipsCrossed++;
                            }
                        }
                    }
                }

                // Swap currentList and otherList
                ShipList* temp = currentList;
                currentList = otherList;
                otherList = temp;
            }

            free(shipsRow);
            break;

        case SIGN:
            printf("\nProcessing SIGN workflow.\n");

            // Preallocating the shipsRow array with a reasonable initial size
            int* shipsRowSign = (int*)malloc(MAX_SHIPS * sizeof(int));
            if (!shipsRowSign) {
                // Handle memory allocation failure
                fprintf(stderr, "Memory allocation failed for shipsRowSign.\n");
                break;
            }

            // Randomly decide the initial side
            side = rand() % 2 == 0 ? LEFT : RIGHT;
            ShipList* currentSignList = (side == LEFT) ? leftSideShips : rightSideShips;
            ShipList* otherSignList = (side == LEFT) ? rightSideShips : leftSideShips;

            // Record the start time
            time_t startTime, currentTime;
            time(&startTime);
            double elapsedTime;

            while (getShipCount(leftSideShips) > 0 || getShipCount(rightSideShips) > 0) {

                // Only proceed if there are ships in the current list
                if (getShipCount(currentSignList) == 0) {
                    printf("\nNo ships in the current list, checking the other side.\n");
                    // Switch to the other side if it has ships
                    if (getShipCount(otherSignList) > 0) {
                        ShipList* temp = currentSignList;
                        currentSignList = otherSignList;
                        otherSignList = temp;
                        time(&startTime); // Restart the timer for the new side
                        continue; // Restart the loop with the new side
                    } else {
                        // If no ships on either side, break the loop to avoid infinite looping
                        printf("\nNo ships left on either side, exiting.\n");
                        break;
                    }
                }

                // Get the number of ships from the current list
                int shipsRowSizeSign = getShipCount(currentSignList);

                // Fill shipsRowSign with ship IDs up to shipsRowSizeSign
                for (int i = 0; i < shipsRowSizeSign; i++) {
                    shipsRowSign[i] = getShipIdByPosition(currentSignList, i);
                }

                int shipsCrossedSign = 0;
                while (shipsCrossedSign < shipsRowSizeSign) {
                    for (int i = 0; i < shipsRowSizeSign; i++) {
                        if (shipsRowSign[i] != -1) {
                            // Execute the context of the ship
                            int active = set_context(shipsRowSign[i]);

                            // Check if the ship has completed its execution
                            if (active == 0) {
                                removeShip(currentSignList, shipsRowSign[i]);
                                shipsRowSign[i] = -1; // Mark as crossed
                                shipsCrossedSign++;
                            }

                            // Re-check elapsed time immediately after each context switch
                            time(&currentTime);
                            elapsedTime = difftime(currentTime, startTime);

                            if (elapsedTime >= SIGN_TIME) {
                                printf("TIME OVER\n");
                                printf("Elapsed time: %f seconds\n", elapsedTime);
                                printf("SIGN_TIME: %d seconds\n", SIGN_TIME);

                                // Only switch sides if the other side has ships
                                if (getShipCount(otherSignList) > 0) {
                                    // Reset the timer and switch sides
                                    time(&startTime); // Restart the timer
                                    ShipList* temp = currentSignList;
                                    currentSignList = otherSignList;
                                    otherSignList = temp;

                                    printf("Switching sides to process the other side.\n");

                                    // Re-fetch the number of ships for the new current side
                                    shipsRowSizeSign = getShipCount(currentSignList);

                                    // Re-fill shipsRowSign with the IDs of the ships from the new side
                                    for (int j = 0; j < shipsRowSizeSign; j++) {
                                        shipsRowSign[j] = getShipIdByPosition(currentSignList, j);
                                    }
                                } else {
                                    // If the other side is empty, continue with the current list
                                    printf("The other side has no ships, continuing with the current side.\n");
                                    time(&startTime); // Restart the timer for the same side
                                }

                                // Break out of the inner while loop to process the side accordingly
                                break;
                            }
                        }
                    }
                }
            }

            free(shipsRowSign);
            break;

        case TICO:
            printf("\nProcessing TICO workflow.\n");

        default:
            if (workflow != TICO) {
                printf("\nUnknown workflow type, falling back to default behavior.\n");
            }

            // Preallocate a single slot for the shipsRow array since it only processing one ship at a time
            int* shipRowTico = (int*)malloc(sizeof(int));
            if (!shipRowTico) {
                // Handle memory allocation failure
                fprintf(stderr, "Memory allocation failed for shipRowTico.\n");
                break;
            }

            while (getShipCount(leftSideShips) > 0 || getShipCount(rightSideShips) > 0) {
                // Decide from which side to pick a ship
                if (getShipCount(leftSideShips) > 0 && getShipCount(rightSideShips) > 0) {
                    // Randomly choose a side if both have ships
                    side = rand() % 2 == 0 ? LEFT : RIGHT;
                } else if (getShipCount(leftSideShips) > 0) {
                    // Pick from the left if only the left side has ships
                    side = LEFT;
                } else if (getShipCount(rightSideShips) > 0) {
                    // Pick from the right if only the right side has ships
                    side = RIGHT;
                } else {
                    // No ships left to process
                    break;
                }

                // Select the current list based on the chosen side
                ShipList* currentTicoList = (side == LEFT) ? leftSideShips : rightSideShips;

                // Get the ID of a random ship from the selected list
                int shipCount = getShipCount(currentTicoList);
                //int randomIndex = rand() % shipCount;
                int shipId = getShipIdByPosition(currentTicoList, 0);

                // Store the selected ship ID in shipRowTico
                shipRowTico[0] = shipId;

                // Process the selected ship until it completes its crossing
                while (shipRowTico[0] != -1) {
                    // Execute the context of the ship
                    int active = set_context(shipRowTico[0]);

                    // Check if the ship has completed its execution
                    if (active == 0) {
                        // Remove the ship from the list and mark the slot as free
                        removeShip(currentTicoList, shipRowTico[0]);
                        shipRowTico[0] = -1; // Mark as crossed
                    }
                }
            }

            free(shipRowTico);
            break;
    }
}

// Main function
void ship_generation_test() {
    ShipList leftSideShips;
    ShipList rightSideShips;
    
    // Initialize both ship lists
    initList(&leftSideShips);
    initList(&rightSideShips);

    channelSide_t side;
    int position;
    int priority;

    // Create an array of ships and add them to the respective lists based on their side
    for (int i = 0; i < 7; i++) {
        side = rand() % 2 == 0 ? LEFT : RIGHT;
        ship_t* ship;
        position = (side == LEFT) ? getNextShipPosition(&leftSideShips, side) : getNextShipPosition(&rightSideShips, side);
        priority = rand() % 6; // Generates a number between 0 and 5
        ship = create_ship(NORMAL, side, priority, position);

        if (side == LEFT) {
            addShip(&leftSideShips, ship);
        } else {
            addShip(&rightSideShips, ship);
        }
    }

    // Print the ships in each list for verification
    printf("\nShips on the left side:\n");
    printList(&leftSideShips);
    printf("\nShips on the right side:\n");
    printList(&rightSideShips);

    // Handle scheduler and workflow
    handle_scheduler(scheduler, &leftSideShips, &rightSideShips);
    handle_workflow(workflow, &leftSideShips, &rightSideShips);

    // Wait for threads to finish (if necessary for synchronization)
    // CEthread_wait();
}

int main() {
    initCEthreads();
    CEmutex_init();
    
    srand(time(0)); 
    
    ship_generation_test();
    
    return 0;
}
