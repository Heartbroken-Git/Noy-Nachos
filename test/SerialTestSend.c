/**
 * @file SerialTestSend.c
 * @brief Test program to check if the ACIA serial simulation works by sending a message through it
 * @details Sends a specified message through the serial connection. SerialTestReceive.c should be running in a parallel nachos instance before actually running the program to send data through.
 */
 
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

char * testMessage = "Le Coeur a ses raisons que la Raison ignore";
int testMessageSize = 43;

int main() {
	n_printf("Beginning message transmission\n");
	int retValue = TtySend(testMessage);
	n_printf("Message successfully sent\n");
	n_printf("Expected size : %i\n", testMessageSize);
	n_printf("Sent size : %i\n", retValue);
	return 0;
}
