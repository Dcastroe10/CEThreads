
#include "ship.c"

void ship_generation_test() {
    // Create two ship lists: one for ships on the left and one for ships on the right
    ShipList leftSideShips;
    ShipList rightSideShips;
    
    // Initialize both ship lists
    initList(&leftSideShips);
    initList(&rightSideShips);

    // Create 4 ships and add them to the respective lists
    ship_t* leftShip1 = create_ship(NORMAL, LEFT, 0);
    addShip(&leftSideShips, leftShip1);

    ship_t* leftShip2 = create_ship(NORMAL, LEFT, 0);
    addShip(&leftSideShips, leftShip2);

    ship_t* rightShip1 = create_ship(NORMAL, RIGHT, 0);
    addShip(&rightSideShips, rightShip1);

	 ship_t* leftShip3 = create_ship(NORMAL, LEFT, 0);
    addShip(&leftSideShips, leftShip3);

    ship_t* rightShip2 = create_ship(NORMAL, RIGHT, 0);
    addShip(&rightSideShips, rightShip2);

    // Print the ships in each list for verification
    printf("Ships on the left side:\n");
    printList(&leftSideShips);

    printf("Ships on the right side:\n");
    printList(&rightSideShips);
    
    // Wait for threads to finish (if necessary for synchronization)
    CEthread_wait();
}

int main() {
    initCEthreads();
    CEmutex_init();
    
    srand(time(0)); 
    
    ship_generation_test();
    
    return 0;
}
