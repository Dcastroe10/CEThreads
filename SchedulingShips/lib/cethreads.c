#include "cethreads.h"

void initCEthreads() {
	int i;
	for (i = 0; i < MAX_THREADS; ++i) {
		cethreadList[i].active = 0;
	}
	return;
}


void CEthread_yield() {
	if (incethread) {
		printf("thread id: %d \n", cethreadList[currentcethread].id);
		swapcontext(&cethreadList[currentcethread].context, &mainContext);
	} else {
		if (activeThreads == 0) {
            return;
        }
		// switch to the next thread
		currentcethread = (currentcethread + 1) % activeThreads;
		if (cethreadList[currentcethread].pause != 0) {
			clock_t end;
			double now = 0;
			end = clock();
			now = (double)(end) / (double)(CLOCKS_PER_SEC);
			printf("%d Now time yield: %lf\n", cethreadList[currentcethread].id, now);
			double previous = (double)(cethreadList[currentcethread].time) / (double)(CLOCKS_PER_SEC);
			while (now - previous < cethreadList[currentcethread].pause) {
				end = clock();
				now = (double)(end) / (double)(CLOCKS_PER_SEC);
				printf("%d now time while: %lf\n", cethreadList[currentcethread].id, now);
				currentcethread = (currentcethread + 1) % activeThreads;
				previous = (double)(cethreadList[currentcethread].time) / (double)(CLOCKS_PER_SEC);
			}
			usleep(200000);
		}
		incethread = 1;
		swapcontext(&mainContext, &cethreadList[currentcethread].context);
		incethread = 0;
        // switch to the main thread when a thread ends
		if (cethreadList[currentcethread].active == 0) {
			printf("cethread has finished \n");
			free(cethreadList[currentcethread].context.uc_stack.ss_sp);
			--activeThreads;
			if (currentcethread != activeThreads) {
				cethreadList[currentcethread] = cethreadList[activeThreads];
			}
			cethreadList[activeThreads].active = 0;
		}
	}
	return;
}


static void simple_thread_wrapper(void (*func)(void)) {
	cethreadList[currentcethread].active = 1;
	func();
	cethreadList[currentcethread].active = 0;

	// Yields control, but since the active state is 0, it returns control to the main thread
	CEthread_yield();
}


static void thread_wrapper(void (*func)(void *), void *arg) {
	//cethreadList[currentcethread].active = 1;
	func(arg);
	cethreadList[currentcethread].active = 0;

	// Yields control, but since the active state is 0, it returns control to the main thread
	CEthread_yield();
}

int getValidID(){
    if(activeThreads == MAX_THREADS){
        return -1; // ERROR: maximun thread capacity cant generate another threads
    }
    else
    {
        for (int i = 0; i < MAX_THREADS; i++)
        {
            if (cethreadList[i].active == 0)
            {
                return i;
            } 
        }
    }
}


int CEthread_create(void (*func)(void *), void *arg) {
	int id = getValidID();

	if (id == -1)
	{

        printf("Maximun threads reached");

		return -1;  // threads has reached its max capacity
	}
	
	// Adds the new function to the end of the thread list
	getcontext(&cethreadList[id].context);

	cethreadList[id].context.uc_link = 0;
	cethreadList[id].context.uc_stack.ss_sp = malloc(THREAD_STACK);
	cethreadList[id].context.uc_stack.ss_size = THREAD_STACK;
	cethreadList[id].context.uc_stack.ss_flags = 0;
	cethreadList[id].id = id;
	cethreadList[id].pause = 0;
	cethreadList[id].time = 0;
	if (cethreadList[id].context.uc_stack.ss_sp == 0) {
		return -1;
	}

    currentcethread = id;

	cethreadList[id].active = 1;
	if (arg == NULL) {
		makecontext(&cethreadList[id].context, (void (*)(void)) &simple_thread_wrapper, 1, func);
	} else {
		makecontext(&cethreadList[id].context, (void (*)(void)) &thread_wrapper, 2, func, arg);
	}

	++activeThreads;

	return id;
}

int CEthread_wait() {
	int threadsRemaining = 0;

	// If it's in a thread, wait for all other threads to finish
	if (incethread) threadsRemaining = 1;
	// Run all threads until they finish
	while (activeThreads > threadsRemaining) {
		CEthread_yield();
	}

	return 0;
}

int CEthread_end() {
	cethreadList[currentcethread].active = 0;
    activeThreads--;
	return 0;
}

int CEthread_join(int id) {
	for (int i = 0; i <= activeThreads; i++) {
		if (cethreadList[i].id == id) {
			while (cethreadList[i].active) {
				CEthread_yield();
			}
		}
	}
	return 0;
}

void CEthread_pause(double pause) {
	clock_t start, end;
	start = clock();
	cethreadList[currentcethread].time = start;
	cethreadList[currentcethread].pause = pause;
	CEthread_yield();
	printf("%d \n", cethreadList[currentcethread].id);
	cethreadList[currentcethread].pause = 0;
}

int set_context(int i) {
	currentcethread = i;

	incethread = 1;
	swapcontext(&mainContext, &cethreadList[currentcethread].context);
	incethread = 0;
	
	if (cethreadList[currentcethread].active == 0) {
		printf("cethread has finished.\n");
		free(cethreadList[currentcethread].context.uc_stack.ss_sp);
		--activeThreads;

		return 0;
	}
	return 1;
}

int set_context_with_quantum(int i, int quantum) {
    currentcethread = i;
    
    // Record the start time
    struct timeval start, end;
    gettimeofday(&start, NULL);

    while (1) {
        // Switch to the thread's context
        swapcontext(&mainContext, &cethreadList[i].context);

        // Check if the thread has finished its execution
        if (cethreadList[currentcethread].active == 0) {
            printf("cethread has finished.\n");
            free(cethreadList[currentcethread].context.uc_stack.ss_sp);
            --activeThreads;

            if (currentcethread != activeThreads) {
                cethreadList[currentcethread] = cethreadList[activeThreads];
            }
            cethreadList[activeThreads].active = 0;
            return 0; // The thread has finished
        }

        // Calculate the elapsed time since the thread started running
        gettimeofday(&end, NULL);
        double elapsedTime = (end.tv_sec - start.tv_sec) + 
                             (end.tv_usec - start.tv_usec) / 1000000.0;

        // Check if the elapsed time exceeds the quantum
        if (elapsedTime >= quantum) {
            printf("Quantum expired for thread %d\n", i);
			printf("elapsedTime: %f\n", elapsedTime);
			printf("quantum: %d\n", quantum);
            return 1; // The thread is still active but time slice expired
        }
    }
}


void CEmutex_init() {
	mutex = 0;
	return;
}


void CEmutex_destroy() {
	// mutex = NULL;
	return;
}


void CEmutex_unlock() {
	mutex = 0;
	return;
}


void CEmutex_trylock() {
	mutex = 0;
	if (mutex == 0) {
		mutex = 1;
	} else {
		while (mutex == 1) {
			usleep(100);
			CEmutex_trylock();
		}
	}
	return;
}
