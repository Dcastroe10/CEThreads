#include "cethreads.c"


/***
 *  @brief Calculates the factorial of a number.
	@return the factorial of n
 */
void factorial() {
	int i = 2;
    int fact = 1;
	int n = 10;
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



int main(){
	initCEthreads();
	CEmutex_init();

	srand(time(0)); 

	CEthread_create( &factorial, NULL);
	CEthread_create( &factorial, NULL);

	CEthread_wait();
	
	return 0;
}
