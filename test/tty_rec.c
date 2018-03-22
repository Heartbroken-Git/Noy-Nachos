/**
 * LEBRETON Mickaël (M1 SSR)
 * ESNAULT Jérémy (M1 IL)
 */

#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#define NB_MESSAGES 10
#define SIZE 2048

int main() {
  char buffer[SIZE];

  TtyReceive(buffer, SIZE);
  n_printf(buffer);
  n_printf("\n");

  TtyReceive(buffer, SIZE);
  n_printf(buffer);
  n_printf("\n");

  TtyReceive(buffer, SIZE);
  n_printf(buffer);
  n_printf("\n");

  TtyReceive(buffer, SIZE);
  n_printf(buffer);
  n_printf("\n");

  TtyReceive(buffer, SIZE);
  n_printf(buffer);
  n_printf("\n");

  TtyReceive(buffer, SIZE);
  n_printf(buffer);
  n_printf("\n");

  return 0;
}
