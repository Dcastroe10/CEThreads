#include <stdio.h>
#include <malloc.h>
#include <ucontext.h>
#include <time.h>
#include <math.h>

#define MAX_THREADS 10
#define THREAD_STACK 1024*1024

int mutex = 0;

static int currentcethread = -1;
static int activeThreads = 0;  // active threads
static int incethread = 0;    // if it is in the main thread


static ucontext_t mainContext;


typedef struct
{
	ucontext_t context;
	int id;
	int active;             // 0 inactive, 1 active
	clock_t time; 
	double pause;
} cethread;

static cethread cethreadList[MAX_THREADS];


/**
 * @brief Initializes the list of cethreads, marking all as inactive.
 */
void initCEthreads();

/**
 * @brief Creates a thread with the given function.
 * 
 * @param func Function that the thread will execute.
 * @param arg Argument for the function, NULL if it has none.
 * @return ID of the created thread.
 */
int CEthread_create(void (*func)(void *), void *arg);

/**
 * @brief Adds functionality to a simple function that you want to start in a thread.
 * 
 * @param func Function to be executed in the thread.
 */
static void simple_thread_wrapper(void (*func)(void));

/**
 * @brief Adds functionality to a function with parameters that you want to start in a thread.
 * 
 * @param func Function to be executed in the thread.
 * @param arg Argument for the function.
 * 
 */
static void thread_wrapper(void (*func)(void *), void *arg);

/**
 * @brief Ends the current thread.
 * 
 * @return 0 if the thread finishes correctly.
 */
int CEthread_end();

/**
 * @brief Yields execution to another thread.
 */
void CEthread_yield();

/**
 * @brief Waits for the thread with the specified ID to finish.
 * 
 * @param id ID of the thread to wait for.
 * @return 0 if the thread finished correctly.
 */
int CEthread_join(int id);

/**
 * @brief Waits for all threads to finish their execution.
 */
int CEthread_wait();

/**
 * @brief Initializes a mutex.
 */
void CEmutex_init();

/**
 * @brief Destroys a mutex.
 */
void CEmutex_destroy();

/**
 * @brief Unlocks a mutex.
 */
void CEmutex_unlock();

/**
 * @brief Attempts to lock a mutex.
 */
void CEmutex_trylock();
