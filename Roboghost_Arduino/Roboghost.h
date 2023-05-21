#ifndef ROBOGHOST_H
#define ROBOGHOST_H


  void blink (char side);
  void stripled(int lednum, int red, int green, int blue);
  void neopixels(int red, int green, int blue);
  

 class Roboghost {
  
  private:
   // byte pin;
    
  public:
    Roboghost(); 
    void begin();
    void neopixels(int red, int green, int blue);   
};


#endif 