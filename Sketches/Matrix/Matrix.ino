#include <Arduino.h>
#include "Matrix.h"

//Create matrix object
Matrix matrix;

void setup() 
{   
  Serial.begin(9600);
}
 
void loop() 
{
  matrix.m_drawFrequencySpectrum();
  //matrix.m_pop();
  
}

