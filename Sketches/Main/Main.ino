#include <Arduino.h>
#include "FrequencySpectrum.h"
#include "Pop.h" 
#include "Matrix.h"

//Create animation object

//FrequencySpectrum matrix;
Pop matrix;

void setup() 
{   
  Serial.begin(9600);
}

 
void loop() 
{
  
  matrix.m_run();
  
}
