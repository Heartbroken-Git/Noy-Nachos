/**
 * @file semTest.c
 * @brief Test program to check the behavior of the implementation of semaphores and threads in our Nachos implementation
 * @details Creates two threads, one "producing" the other "consuming" faster than the production and hopefully being blocked by semaphores
 */
 
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#define PRODATTEMPT 5
#define cptSem (SemCreate("cptSem", 0))

volatile int readyProducts = 0;

// Production function
void production() {
	n_printf("Starting production\n");
	int productsMade = 0;
	while (productsMade < PRODATTEMPT) {
		int i = 0;
		while (i < 500000) {
			i++; // Simulate time to produce	
		}
		n_printf("Calling V\n");
		if(V(cptSem) != 0) {
			PError("Possible problem on sem use");
		} else {
			n_printf("Made one product\n");
			productsMade++;
		}
		
	}
}

// Consumption function
void consumption() {
	n_printf("Starting consumption\n");
	int productsConsumed = 0;
	while (productsConsumed < PRODATTEMPT) {
		n_printf("Calling P\n");
		if(P(cptSem) != 0) { // Tries immediately to take a product
			PError("Possible problem on sem use");
		} else {
			int poorMansTimer = 0;
			while (poorMansTimer < 20000) {
				poorMansTimer++; // Simulate time to produce
			}
			n_printf("Consumed one product\n"); 
			productsConsumed++;
		}
	}
}

int main() {
	n_printf("Starting semaphore and threads test\n");
	
	void * addProd = &production;
	void * addCons = &consumption;
	
	ThreadId t1 = threadCreate("tProd", addProd);
	ThreadId t2 = threadCreate("tCons", addCons);
	
	Join(t2); // Wait for the consumption to finish (it should necessarily end after the production)
	
	return 0;
}
