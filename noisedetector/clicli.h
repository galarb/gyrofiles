#ifndef CLICLI_H
#define CLICLI_H
#include "ND.h"

 class clicli {
  private:
  ND &mynd;
  int number;
  
  public:
   clicli(ND &mynd);
   void begin();   //must be called from  void setup()
   void run();   //must be called from  void loop()

 };
#endif 