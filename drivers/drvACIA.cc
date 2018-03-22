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
  send_sema = new Semaphore((char *)"send_sema",0);
  receive_sema = new Semaphore((char * )"receive_sema",0);
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
    DEBUG('d', (char *)"TtySend : ACIA_BUSY_WAITING\n");

    do{
      while(g_machine->acia->GetOutputStateReg() == FULL){}// can't send a character
      g_machine->acia->PutChar(buff[ind_send]); // send one character
      ind_send++;
      DEBUG('d', (char *)"buffer[%d] = %c\n",ind_send, buff[ind_send]);

    }while(buff[ind_send] != 0);

    g_machine->acia->SetWorkingMode(mode);
    DEBUG('d', (char *)"TtySend exit\n");

    return ind_send;


  }else if (g_cfg->ACIA == ACIA_INTERRUPT){// INTERRUPTION MODE
	
	DEBUG('d', (char *) "ACIA mode found as either INTERRUPT\n");
	DEBUG('d', (char *) "Filling emission buffer\n");
	ind_send = 0;
	
	while (buff[ind_send] != '\0' && ind_send < BUFFER_SIZE) {
		send_buffer[ind_send] = buff[ind_send];
		ind_send++;
	}
	DEBUG('d', (char *) "Buffer filled with %i characters\n", ind_send);
	ind_send = 0;
	
	DEBUG('d', (char *) "Giving way to other threads while buffer emptying\n");
	g_machine -> acia -> SetWorkingMode(SEND_INTERRUPT);
	send_sema->P();
	// Sending stuff
	
	g_machine -> acia -> SetWorkingMode(BUSY_WAITING);
	return ind_send;	

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
  switch (g_cfg->ACIA) {

    case ACIA_BUSY_WAITING: {
      DEBUG('d', (char *) "ACIA mode found as BUSY_WAITING\n");
      ind_rec = 0;
      bool reachedSlashZero = false;
      while (ind_rec < lg && !reachedSlashZero) {
        DEBUG('d', (char *) "Checking whether InputStateReg ready or not\n");
        while (g_machine->acia->GetInputStateReg() != FULL) {}
        receive_buffer[ind_rec] = g_machine->acia->GetChar();
        DEBUG('d', (char *) "Registry ready and char copied : %c \n", receive_buffer[ind_rec]);
        if (receive_buffer[ind_rec] == '\0') {
          reachedSlashZero = true;
        }
        ind_rec++;
      }

      buff = &receive_buffer[0];
      return ind_rec;
      // break;
    }

    case ACIA_INTERRUPT: {
      DEBUG('d', (char *) "ACIA mode found as either INTERRUPT\n");
      ind_rec = 0;
      DEBUG('d', (char *) "Giving way to other threads while waiting for buffer\n");
      receive_sema->P();
      g_machine -> acia -> SetWorkingMode(REC_INTERRUPT);
      // wait for buffer to get filled in

      buff = &receive_buffer[0];
      g_machine -> acia -> SetWorkingMode(BUSY_WAITING);
      return ind_rec;
      //break;
    }

    default: {
      printf("ERROR : Unexpected ACIA mode, exiting !");
      exit(-2);
    }
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
  #ifdef ETUDIANTS_TP
  if (send_buffer[ind_send] != 0) {
  	DEBUG('d', (char *) "Registry ready and char copied : %c \n", send_buffer[ind_send]);
    g_machine -> acia -> PutChar(send_buffer[ind_send]);
    ind_send++;
  }else{
    g_machine->acia->SetWorkingMode(REC_INTERRUPT);
    send_sema->V();
  }
  #endif
  #ifndef ETUDIANTS_TP
  printf("**** Warning: send interrupt handler not implemented yet\n");
  exit(-1);
  #endif
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
  #ifndef ETUDIANTS_TP
  printf("**** Warning: receive interrupt handler not implemented yet\n");
  exit(-1);
  #endif

  #ifdef ETUDIANTS_TP
  receive_buffer[ind_rec] = g_machine->acia->GetChar();
  DEBUG('d', (char *) "Registry ready and char copied : %c \n", receive_buffer[ind_rec]);
  if ((receive_buffer[ind_rec] == '\0') || (ind_rec+1 == BUFFER_SIZE)) {
    g_machine -> acia -> SetWorkingMode(SEND_INTERRUPT);

    receive_sema->V();
  }else{
    ind_rec++;
  }
  #endif

}
