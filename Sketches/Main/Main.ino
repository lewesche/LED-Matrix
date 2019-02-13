#include <Arduino.h>
#include "FrequencySpectrum.h"
#include "Pop.h" 
#include "Matrix.h"


//Create a pointer to a matrix base class
Matrix* matrix;

void setup() 
{   
  Serial.begin(9600);
}


void loop() 
{
  for(int i = 0; i < 5; i++)
  {
    matrix = new Pop;
    matrix->m_run();
    delete matrix;
  }

  matrix = new FrequencySpectrum();
  matrix->m_run();
}
