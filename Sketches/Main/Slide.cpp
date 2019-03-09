#include <Arduino.h>
#include "Slide.h"

#define GRID_SIZE 8

void Slide::m_newColors()
{
  for(int c = 0; c < 3; c++)
    m_currentColor[c] = m_nextColor[c];

  int colorDifference = 0;
  while(colorDifference < 100)
  {
    m_getRandomColor(m_nextColor);
    
    colorDifference = 0;
    
    for(int c = 0; c < 3; c++)
      colorDifference += abs(m_currentColor[c]-m_nextColor[c]);
  }
  return;
}

bool Slide::m_transitionColors()
{
  m_decayTimer = 0;
  
  int m_direction = random(0, 4);
  
  long percentFade = 0;
  
  unsigned long startTime = millis();
  unsigned long t = 0;

  while(true)
  {
    for(int y = GRID_SIZE-1; y >= 0; y--)
    {
      percentFade = pow(static_cast<double>(t)/m_transitionTime * GRID_SIZE/(y+1), 2) * 100;
      if (percentFade > 100)
        percentFade = 100;

      if (m_direction == 0)
      {
        for(int x = 0; x < GRID_SIZE; x++)
        {
          m_interpColors(x, y, m_nextColor, m_currentColor, percentFade);
        }
      }

      if (m_direction == 1)
      {
        for(int x = 0; x < GRID_SIZE; x++)
        {
          m_interpColors(x, 7-y, m_nextColor, m_currentColor, percentFade);
        }
      }

      if (m_direction == 2)
      {
        for(int x = 0; x < GRID_SIZE; x++)
        {
          m_interpColors(y, x, m_nextColor, m_currentColor, percentFade);
        }
      }

      if (m_direction == 3)
      {
        for(int x = 0; x < GRID_SIZE; x++)
        {
          m_interpColors(7-y, x, m_nextColor, m_currentColor, percentFade);
        }
      }
      
    } 
    FastLED.show();
    t = millis() - startTime; 

    if(checkButton())
        return false;
    
    if (t > m_transitionTime)
    {
      m_fillSolidColor(m_currentColor);
      m_newColors();
      return true;
    }
  }
  
}

void Slide::m_decayColors()
{
  if(m_currentColor[0] == 0 && m_currentColor[1] == 0 && m_currentColor[2] == 0)
    return;
  
  double percentFade = 100 * (1.0 - (m_decayTimer)/(m_decayPeriod));

  //Serial.println("In Decay");

  
  if (percentFade < 0)
    percentFade = 0;

  for(int c = 0; c < 3; c++)
  {
//    Serial.print(m_currentColor[c]);
//    Serial.print(" -> ");
    m_currentColor[c] = (m_decayColor[c] * percentFade) / 100;
//    Serial.print(m_currentColor[c]);
//    Serial.println("    ");
  }
  
  m_FHT();  

  m_fillSolidColor(m_currentColor);
  FastLED.show();
}

void Slide::m_run()
{
  while(checkButton()) // Wait for user to release the button
  {
  }
  
  m_clear();
  
  while(m_transitionColors())
  {
    m_FHT();
    m_decayTimer= 0;
    
    for(int c = 0; c < 3; c++)
      m_decayColor[c] = m_currentColor[c];
    Serial.println(m_soundLevelSum);  
    while (m_soundLevelSum < m_soundThreshold)
    {
      m_decayColors();
      m_FHT(); //Sample audio

      m_decayTimer += 10;
      
      if(checkButton())
        return;
    }
  }

  return;
}
