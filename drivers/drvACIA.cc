/* \file drvACIA.cc
\brief Routines of the ACIA device driver
//
//      The ACIA is an asynchronous device (requests return
//      immediately, and an interrupt happens later on).
//      This is a layer on top of the ACIA.
//      Two working modes are to be implemented in assignment 2:
//      a Busy Waiting mode and an Interrupt mode. The Busy Waiting
//      mode implements a synchronous IO whereas IOs are asynchronous
//      IOs are implemented in the Interrupt mode (see the Nachos
//      roadmap for further details).
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.
//  See copyright_insa.h for copyright notice and limitation
//  of liability and disclaimer of warranty provisions.
//
*/

/* Includes */

#include "kernel/system.h"         // for the ACIA object
#include "kernel/synch.h"
#include "machine/ACIA.h"
#include "drivers/drvACIA.h"

//-------------------------------------------------------------------------
// DriverACIA::DriverACIA()
/*! Constructor.
Initialize the ACIA driver. In the ACIA Interrupt mode,
initialize the reception index and semaphores and allow
reception and emission interrupts.
In the ACIA Busy Waiting mode, simply inittialize the ACIA
working mode and create the semaphore.
*/
//-------------------------------------------------------------------------

DriverACIA::DriverACIA()
{
  #ifdef ETUDIANTS_TP
  send_sema = new Semaphore((char *)"send_sema",1);
  receive_sema = new Semaphore((char * )"receive_sema",1);
  ind_send = 0;
  ind_rec = 0;
  #endif
  #ifndef ETUDIANTS_TP
  printf("**** Warning: contructor of the ACIA driver not implemented yet\n");
  exit(-1);
  #endif
}

//-------------------------------------------------------------------------
// DriverACIA::TtySend(char* buff)
/*! Routine to send a message through the ACIA (Busy Waiting or Interrupt mode)
*/
//-------------------------------------------------------------------------

int DriverACIA::TtySend(char* buff)
{

  #ifdef ETUDIANTS_TP


  int mode = g_machine->acia->GetWorkingMode();
  if(g_cfg->ACIA == ACIA_BUSY_WAITING){// BUSY WAITING MODE

    g_machine->acia->SetWorkingMode(BUSY_WAITING);

    do{
      while(g_machine->acia->GetOutputStateReg() == FULL){}// can't send a character
      g_machine->acia->PutChar(buff[this->ind_send]); // send one character
      this->ind_send++;

    }while(buff[this->ind_send] != '\0');

    g_machine->acia->SetWorkingMode(mode);
    return ind_send;


  }else if (g_cfg->ACIA == ACIA_INTERRUPT){// INTERRUPTION MODE

  }else{

    exit(-1);
  }
  return 0;
  #endif

  #ifndef ETUDIANTS_TP
  printf("**** Warning: method Tty_Send of the ACIA driver not implemented yet\n");
  exit(-1);
  return 0;
  #endif
}

//-------------------------------------------------------------------------
// DriverACIA::TtyReceive(char* buff,int length)
/*! Routine to reveive a message through the ACIA
//  (Busy Waiting and Interrupt mode).
*/
//-------------------------------------------------------------------------

int DriverACIA::TtyReceive(char* buff,int lg)
{
  
	#ifndef ETUDIANTS_TP
   printf("**** Warning: method Tty_Receive of the ACIA driver not implemented yet\n");

  exit(-1);
  return 0;
  #endif

	#ifdef ETUDIANTS_TP
	DEBUG('d', (char *) "Initiating reception subroutine, checking ACIA mode\n");
	if (g_machine->acia->GetWorkingMode() == BUSY_WAITING) { // Check whether we are in BUSY WATING or not
		DEBUG('d', (char *) "ACIA mode found as BUSY_WAITING\n");
		ind_rec = 0;
		bool reachedSlashZero = false;
		while (ind_rec < lg && !reachedSlashZero) {
			DEBUG('d', (char *) "Checking whether InputStateReg ready or not\n");
			if(g_machine->acia->GetInputStateReg() == FULL) {
				receive_buffer[ind_rec] = g_machine->acia->GetChar();
				DEBUG('d', (char *) "Registry ready and char copied : %c", receive_buffer[ind_rec]);
				if (receive_buffer[ind_rec] == '\0') {
					reachedSlashZero = true;
				}
				ind_rec++;
			}
		}
		buff = &receive_buffer[0];
		return ind_rec;
	} else {
		printf("ERROR : Accessing ACIA outside of BUSY WAITING, exiting !");
		exit(-2);
	}
	#endif
}


//-------------------------------------------------------------------------
// DriverACIA::InterruptSend()
/*! Emission interrupt handler.
Used in the ACIA Interrupt mode only.
Detects when it's the end of the message (if so, releases the send_sema semaphore), else sends the next character according to index ind_send.
*/
//-------------------------------------------------------------------------

void DriverACIA::InterruptSend()
{
  printf("**** Warning: send interrupt handler not implemented yet\n");
  exit(-1);
}

//-------------------------------------------------------------------------
// DriverACIA::Interrupt_receive()
/*! Reception interrupt handler.
Used in the ACIA Interrupt mode only. Reveices a character through the ACIA.
Releases the receive_sema semaphore and disables reception
interrupts when the last character of the message is received
(character '\0').
*/
//-------------------------------------------------------------------------

void DriverACIA::InterruptReceive()
{
  printf("**** Warning: receive interrupt handler not implemented yet\n");
  exit(-1);
}
