#include "ship.c"

void ship_generation_test() {
    // Create two ship lists: one for ships on the left and one for ships on the right
    ShipList leftSideShips;
    ShipList rightSideShips;
    
    // Initialize both ship lists
    initList(&leftSideShips);
    initList(&rightSideShips);

    // Create an array of ships and add them to the respective lists based on their side
    for (int i = 0; i < 5; i++) {
        // Create a ship (you can adjust the type, side, and other parameters as needed)
        ship_t* ship = create_ship(NORMAL, rand() % 2 == 0 ? LEFT : RIGHT, 0);

        // Check the side of the ship and add to the appropriate list
        if (ship->side == LEFT) {
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
