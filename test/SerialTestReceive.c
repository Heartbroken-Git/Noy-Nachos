/**
 * @file SerialTestReceive.c
 * @brief Test program to check if the ACIA serial simulation works by receiving a message through it
 * @details Awaits a message coming through the serial connection and displays it along with its size. Should be run in another nachos instance than SerialTestSend and beforehand
 */
 
#include "userlib/syscall.h"
#include "userlib/libnachos.h"

int main() {

	char * receivedMessage = "";

	n_printf("Awaiting message transmission\n");
	int retValue = TtyReceive(receivedMessage,99);
	n_printf("Message successfully received : %s\n",receivedMessage);
	n_printf("Size upon reception : %i\n", retValue);
	
	return 0;
}
