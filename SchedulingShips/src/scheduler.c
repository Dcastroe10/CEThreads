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


void ship_generation_test() {
    // Create two ship lists: one for ships on the left and one for ships on the right
    ShipList leftSideShips;
    ShipList rightSideShips;
    
    // Initialize both ship lists
    initList(&leftSideShips);
    initList(&rightSideShips);

    channelSide_t side;
    int position;

    // Create an array of ships and add them to the respective lists based on their side
    for (int i = 0; i < 10; i++) {
        // Create a ship (you can adjust the type, side, and other parameters as needed)

        side = rand() % 2 == 0 ? LEFT : RIGHT;
        ship_t* ship;

        if (side == LEFT) {
            position = getNextShipPosition(&leftSideShips, side);
            ship = create_ship(NORMAL, side, 0, position);
        } else {
            position = getNextShipPosition(&rightSideShips, side);
            ship = create_ship(NORMAL, side, 0, position);
        }

        // Check the side of the ship and add to the appropriate list
        if (side == LEFT) {
            addShip(&leftSideShips, ship);
        } else {
            addShip(&rightSideShips, ship);
        }
    }

    // Print the ships in each list for verification
    printf("Ships on the left side:\n");
    printList(&leftSideShips);

    printf("Ships on the right side:\n");
    printList(&rightSideShips);

    // Ordenar las listas de acuerdo con el calendarizador
    switch (scheduler) {
        case RR:
            // Code to handle the Round Robin scheduling algorithm.
            printf("Executing Round Robin scheduling.\n");
            break;

        case PRIORITY:
            // Code to handle the Priority-based scheduling algorithm.
            printf("Executing Priority-based scheduling.\n");
            break;

        case SJF:
            // Code to handle the Shortest Job First (SJF) scheduling algorithm.
            printf("Executing Shortest Job First scheduling.\n");
            break;

        case FCFS:
            // Code to handle the First In First Out (FIFO) scheduling algorithm.
            printf("Executing First Come First Served scheduling.\n");
            break;

        case REAL_TIME:
            // Code to handle the Real-Time scheduling algorithm.
            printf("Executing Real-Time scheduling.\n");
            break;

        default:
            // Code to handle an unrecognized scheduler value.
            printf("Unknown scheduling algorithm.\n");
            break;
    }

    switch (workflow) {
        case EQUITY:
            printf("Processing EQUITY workflow.\n");
            side = rand() % 2 == 0 ? LEFT : RIGHT;
            ShipList* currentList; 
            if (side == LEFT)
            {
                currentList = &leftSideShips;
            }
            else
            {
                currentList = &rightSideShips;
            }
            
            
            while (getShipCount(&leftSideShips) > 0 || getShipCount(&rightSideShips) > 0 )
            {
                int shipsRowSize = 0;
                if (getShipCount(currentList) >= EQUITY_W)
                {
                    shipsRowSize = EQUITY_W;
                }
                else
                {
                    shipsRowSize = getShipCount(currentList);
                }
                
                
                int* shipsRow = (int*)malloc(shipsRowSize * sizeof(int));
                for (int i = 0; i < shipsRowSize; i++) {
                    shipsRow[i] = getShipIdByPosition(currentList, i);
                }
                int shipsCrossed = 0;
                int active = 1;
                while (shipsCrossed < shipsRowSize)
                {
                    for (int i=0; i<shipsRowSize; i++) {
                        if (shipsRow[i] != -1)
                        {
                            active = set_context(shipsRow[i]); // ejecuta el hilo y guarda su estado
                            if (active == 0)
                            {
                                active = 1;
                                removeShip(currentList, shipsRow[i]);
                                shipsRow[i] = -1;
                                shipsCrossed++;
                            }
                        }
                    }
                 
                }
                //printf("se ejecutaron todos los de un lado");
                
                //checkShip(currentList);///

                if (side == LEFT) {
                    //printf("Hola, como estas");
                    currentList = &rightSideShips;
                    side = RIGHT;
                }
                else
                {
                    currentList = &leftSideShips;
                    side = LEFT;
                }
                
            }
            
            break;
            
        case SIGN:
            printf("Processing SIGN workflow.\n");
            break;

        case TICO:
            printf("Processing TICO workflow.\n");
            break;

        default:
            printf("Unknown workflow type.\n");
            break;
    }
    
    // Wait for threads to finish (if necessary for synchronization)
    //CEthread_wait();
}

int main() {
    initCEthreads();
    CEmutex_init();
    
    srand(time(0)); 
    
    ship_generation_test();
    
    return 0;
}
