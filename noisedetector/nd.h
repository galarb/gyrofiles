/*                 ******************
              Class for managing my noise detector
              Featuring: ESP32
                         7789 OLED screen
                        
                By: Gal Arbel 2023
                **********************                                   */
#ifndef ND_H
#define ND_H

class ND { 
  public:
    ND(int sensorPin); 
    void begin(double bdrate);    
    int measurenoise();
    void checklevel();
  private:
    int _sensorPin;
    
    
};


#endif 