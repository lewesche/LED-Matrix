#include <Arduino.h>
#include "Pop.h"

#define GRID_SIZE 8

void Pop::m_fadePopIn(int x_center, int y_center)
{
  double percentColor;
  double distance;
  unsigned long setTime;
  unsigned long t;
  
  setTime = millis();
  t = 0;

  while(true)
  {
    for(double x = 0; x < GRID_SIZE; x++)       
    {
      for(double y = 0; y < GRID_SIZE; y++)
      {
        distance = sqrt((x_center-x)*(x_center-x) + (y_center-y)*(y_center-y));
        percentColor = ((100.0*static_cast<double>(t))/m_period) * (m_radius/(distance + m_radius)) * (m_radius/(distance + m_radius));
        setPercentBrightness(x, y, static_cast<int>(percentColor)); 
      }
    }
    FastLED.show();    
    t = (millis() - setTime);  

    if(t > m_period/2)
      return;  
  }  
}

void Pop::m_fadePopOut(int x_center, int y_center)
{
  double percentColor;
  double distance;
  double offset;
  unsigned long setTime;
  unsigned long t;

  
  setTime = millis();
  t = 0;

  while(true)                    
  {
    offset = m_maxOffset * (1 - (m_period/2 -static_cast<double>(t))/(m_period/2));
    for(double x = 0; x < GRID_SIZE; x++)       
    {
      for(double y = 0; y < GRID_SIZE; y++)
      {
        distance = sqrt((x_center-x)*(x_center-x) + (y_center-y)*(y_center-y));
        percentColor = ((100.0*(m_period/2 - static_cast<double>(t)))/m_period) * (m_radius/(distance + m_radius)) * (m_radius/(abs(distance-offset) + m_radius));
        setPercentBrightness(x, y, static_cast<int>(percentColor)); 
      }
    }
    FastLED.show();
    t = (millis() - setTime);  
    
    if(t > m_period/2)
      return; 
  }  
}

bool Pop::m_run()
{
  m_clear();
  
  //Generate center
  double x_center = random(1, 7); 
  double y_center = random(1, 7);

  //Assign color to whole grid. Before color is written to LED's, pixels with low intensity will be faded out. 
  m_getRandomColor(m_color);
  m_fillSolidColor(m_color);

  //Calculate sum of sound levels, once they exceed a threshold procede with the animation
  int soundSum = 0;
  while (soundSum < m_soundThreshold)
  {
    soundSum = 0;
    m_FHT(); //Sample audio
    for (int i = 0; i < GRID_SIZE; i++)
      soundSum += m_soundLevels[i];
  }

  m_fadePopIn(x_center, y_center);

  //If none is provided, generate a max offset (pop 'intensity') based on the period
  if(m_maxOffset == 0)
    m_maxOffset = sqrt(m_period/2);
    
  m_fadePopOut(x_center, y_center);

  m_clear(); 

  return true;
}
