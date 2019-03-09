#include <Arduino.h>
#include "FrequencySpectrum.h"
#include "Pop.h" 
#include "Slide.h"
#include "Matrix.h"

//Create a pointer to a matrix base class
Matrix* matrix;

void setup() 
{   
  Serial.begin(9600);
  randomSeed(analogRead(2));
}


void loop() 
{
  matrix = new FrequencySpectrum;
  matrix->m_run();
  delete matrix;

  matrix = new Pop;
  matrix->m_run();
  delete matrix;

  matrix->m_off();
  delete matrix;
}
