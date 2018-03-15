//A simple test program to check how work the sempahore implementation with a wait behavior

#include "userlib/syscall.h"
#include "userlib/libnachos.h"

SemId sema1;
SemId sema2;

VoidNoArgFunctionPtr testSema1(){
  P(sema1);
  n_printf("Test de wait sema1 \n");
  V(sema2);
  n_printf("Wait finis sema1 \n");
  return 0;
}

VoidNoArgFunctionPtr testSema2(){
  V(sema1);
  n_printf("Test de wait sema 2 \n");
  P(sema2);
  n_printf("Wait finis sema2 \n");
  return 0;
}

int main() {
  n_printf("Debut du programme de test");

 sema1 = SemCreate((char *)"sema1", 1);
 sema2 = SemCreate((char *)"sema2", 1);

  threadCreate((char *)"process1", (VoidNoArgFunctionPtr)&testSema1);
  threadCreate((char *)"process2", (VoidNoArgFunctionPtr)&testSema2);

  return 0;
}
