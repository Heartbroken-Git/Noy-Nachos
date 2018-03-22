/**
 * @file semTest.c
 * @brief Test program to check the behavior of the implementation of semaphores and threads in our Nachos implementation
 * @details Creates two threads, one "producing" the other "consuming" faster than the production and hopefully being blocked by semaphores
 */
 
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#define PRODATTEMPT 100
#define STALLSIZE 5

SemId semEmpty;
SemId semFull;

volatile int readyProducts = 0;

// Production function
void production() {
	n_printf("Starting production\n");
	int productsMade = 0;
	while (productsMade < PRODATTEMPT) {
		if(P(semFull) != 0) { // Tries to produce if stall not full
			PError("Possible problem on sem use");
		} else {
			n_printf("Producing product %i\n", productsMade);
			readyProducts++;
			productsMade++;
			if(V(semEmpty) != 0) { // Makes the stall "less empty"
				PError("Possible problem on sem use");
			}
		
		}
	}
}

// Consumption function
void consumption() {
	n_printf("Starting consumption\n");
	int productsConsumed = 0;
	while (productsConsumed < PRODATTEMPT) {
		if(P(semEmpty) != 0) { // Tries to consume if stall not empty
			PError("Possible problem on sem use");
		} else {
			readyProducts--;
			productsConsumed++;
			if(V(semFull) != 0) { // Makes the stall "less full"
				PError("Possible problem on sem use");
			}
		}
	}
}

int main() {
	n_printf("Starting semaphore and threads test\n");
	
	void * addProd = &production;
	void * addCons = &consumption;
	
	semEmpty = SemCreate("semEmpty", 0);
	semFull = SemCreate("semFull", 5);
	
	ThreadId t1 = threadCreate("tProd", addProd);
	ThreadId t2 = threadCreate("tCons", addCons);
	
	Join(t2); // Wait for the consumption to finish (it should necessarily end after the production)
	
	return 0;
}
