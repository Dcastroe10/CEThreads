#include "ship.c"


void ship_generation_test(){

    ship_t* newShip1 = create_ship(NORMAL, RIGHT, 0);
    ship_t* newShip2 = create_ship(FISHING, RIGHT, 0);
    CEthread_wait();


}

int main(){
	initCEthreads();
	CEmutex_init();

	srand(time(0)); 
	
	ship_generation_test();
	
	return 0;
}