#include "cethreads.c"

int i = 2;
int fact = 1;
int n = 10;

/***
 *  @brief Calculates the factorial of a number.
	@return the factorial of n
 */
void factorial() {

    while (i <= n) {
		CEmutex_trylock();
        fact *= i;
		printf( "factorial(%d) = %d\n", i, fact);
		i++;
		CEmutex_unlock();
		sleep( 1 );
		CEthread_yield();
    }
}

void restarter(){
	CEmutex_trylock();
	i = 2;
	fact = 1;
	n = 10;
	printf( "i: %d  fact: %d  n: %d\n", i, fact, n);

	CEmutex_unlock();
}



int main(){
	initCEthreads();
	CEmutex_init();

	srand(time(0)); 

	int ID = CEthread_create( &factorial, NULL);
	int ID2 = CEthread_create( &factorial, NULL);
	int ID3 = CEthread_create( &restarter, NULL);
	CEthread_join(ID2);


	CEthread_wait();
	
	return 0;
}
