#include <Arduino.h>
#include "Pop.h"

#define GRID_SIZE 8

void Pop::m_fadePopIn(int x_center, int y_center, int radius, unsigned char (&color)[3], double period)
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
        percentColor = ((100.0*static_cast<double>(t))/period) * (radius/(distance + radius)) * (radius/(distance + radius));
        setPercentBrightness(x, y, static_cast<int>(percentColor)); 
      }
    }
    FastLED.show();    
    t = (millis() - setTime);  

    if(t > period/2)
      return;  
  }  
}

void Pop::m_fadePopOut(int x_center, int y_center, int radius, unsigned char (&color)[3], double period, double maxOffset)
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
    offset = maxOffset * (1 - (period/2 -static_cast<double>(t))/(period/2));
    for(double x = 0; x < GRID_SIZE; x++)       
    {
      for(double y = 0; y < GRID_SIZE; y++)
      {
        distance = sqrt((x_center-x)*(x_center-x) + (y_center-y)*(y_center-y));
        percentColor = ((100.0*(period/2 - static_cast<double>(t)))/period) * (radius/(distance + radius)) * (radius/(abs(distance-offset) + radius));
        setPercentBrightness(x, y, static_cast<int>(percentColor)); 
      }
    }
    FastLED.show();
    t = (millis() - setTime);  
    
    if(t > period/2)
      return; 
  }  
}

bool Pop::m_run()
{
  double period = 3000.0;
  double radius = 2.0;
  double maxOffset = 0.0;
  
  unsigned char color[3]; 
  m_getRandomColor(color);

    
  //Generate center
  double x_center = random(1, 7); 
  double y_center = random(1, 7);

  //Assign color to whole grid. Before color is written to LED's, pixels with low intensity will be faded out. 
  m_fillSolidColor(color);

  m_fadePopIn(x_center, y_center, radius, color, period);

  //If none is provided, generate a max offset (pop 'intensity') based on the period
  if(maxOffset == 0)
    maxOffset = sqrt(period/2);
    
  m_fadePopOut(x_center, y_center, radius, color, period, maxOffset);

  m_clear(); 

  return true;
}

