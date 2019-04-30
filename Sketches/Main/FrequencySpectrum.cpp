#include <Arduino.h>
#include "FrequencySpectrum.h"

#define GRID_SIZE 8

void FrequencySpectrum::m_intro(int fadeTime)
{
  int percentBrightness = 0;
  m_fillSolidColor(m_backgroundColor);

  unsigned long setTime = millis();
  unsigned long t = 0;
  
  while(true)
  {
    for(int y = GRID_SIZE-1; y >= 0; y--)
    {
      percentBrightness = pow(static_cast<double>(t)/fadeTime, 4) * (50*(y+1))/(GRID_SIZE);
      for(int x = 0; x < GRID_SIZE; x++)
      {
        setPercentBrightness(x, y, percentBrightness);
      }
    } 
    FastLED.show();
    t = millis() - setTime; 
    if (t > fadeTime)
      return;
  }
}

void FrequencySpectrum::m_newSpectrumColors()
{
  
  for(int i = 0; i < 2; i++)
  {
    m_getRandomColor(m_lastPeakEndColors[i]);
    m_getRandomColor(m_nextPeakEndColors[i]);
  }

  for(int i = 0; i < 3; i++)
  {
    m_peakColors[0][i] = m_lastPeakEndColors[0][i];
    m_peakColors[GRID_SIZE-1][i] = m_lastPeakEndColors[1][i];
  }

  // Fill in interior colors
  for(int i = 1; i < GRID_SIZE-1; i++)
  {
    for(int c = 0; c < 3; c++)
    {
      m_peakColors[i][c] = (m_peakColors[0][c] * (GRID_SIZE-1-i) + m_peakColors[GRID_SIZE-1][c] * i) / 7;
    }
  }    

  m_peakStepCount = 0;
}

void FrequencySpectrum::m_drawBackground()
{
  int percentBrightness = 0;
  int percentFade = 0;
  
  if((millis()-m_backgroundStepTime) > m_backgroundPeriod/m_backgroundSteps)
  {
    m_backgroundStepTime = millis();  
    for(int c = 0; c < 3; c++)
      m_backgroundColor[c] = (m_lastBackgroundColor[c]*(m_backgroundSteps-m_backgroundStepCount) + m_nextBackgroundColor[c]*m_backgroundStepCount)/m_backgroundSteps;

    for(int y = 0; y < GRID_SIZE; y++)
    {
      percentBrightness = (50*(y+1))/(GRID_SIZE);
      for(int x = 0; x < GRID_SIZE; x++)
      {
        m_leds[cvtCoords(x, y)].setRGB(
          (m_backgroundColor[0]*percentBrightness)/100,
          (m_backgroundColor[1]*percentBrightness)/100, 
          (m_backgroundColor[2]*percentBrightness)/100);
        for(int c = 0; c < 3; c++)
          m_backgroundMatrix[x][y][c] = (m_backgroundColor[c]*percentBrightness)/100;
      }
    }
    m_backgroundStepCount+=1;
  }

  
  if(m_backgroundStepCount > m_backgroundSteps)
  {
    for(int c = 0; c < 3; c++)
      m_lastBackgroundColor[c] = m_nextBackgroundColor[c];
    m_getRandomColor(m_nextBackgroundColor);
    m_backgroundStepCount = 0;
  }
}

void FrequencySpectrum::m_drawNewPeak(int x)
{
  m_peaks[x] = m_soundLevels[x];
  m_setTime[x] = millis();
  for (int y = 0; y <= m_peaks[x]; y++)
  {
    for(int c = 0; c < 3; c++)
      m_matrix[x][y][c] = m_peakColors[x][c];
    setPercentBrightness(x, y, 100);
  }
}

void FrequencySpectrum::m_fadePeak(int x)
{  
//
//  if(m_peakCycleTime > m_peakPeriod)
//  {
//    //Cycle end colors
//    if((millis()-m_peakStepTime) > m_peakPeriod/m_peakSteps)
//    {
//      m_peakStepTime = millis();
//      for(int c = 0; c < 3; c++)
//      {
//        m_peakColors[0][c] = (m_lastPeakEndColors[0][c]*(m_peakSteps-m_peakStepCount) + m_nextPeakEndColors[0][c]*m_peakStepCount)/m_peakSteps;
//        m_peakColors[GRID_SIZE-1][c] = (m_lastPeakEndColors[1][c]*(m_peakSteps-m_peakStepCount) + m_nextPeakEndColors[1][c]*m_peakStepCount)/m_peakSteps;
//      }
//      m_peakStepCount++;
//      Serial.println(m_peakStepCount);
//    }
//  
//    // Fill in interior colors
//    for(int i = 1; i < GRID_SIZE-1; i++)
//    {
//      for(int c = 0; c < 3; c++)
//      {
//        m_peakColors[i][c] = (m_peakColors[0][c] * (GRID_SIZE-1-i) + m_peakColors[GRID_SIZE-1][c] * i) / 7;
//      }
//    }
//    
//    if(m_peakStepCount > m_peakSteps)
//    {
//      for(int c = 0; c < 3; c++)
//      {
//        m_lastPeakEndColors[0][c] = m_nextPeakEndColors[0][c];
//        m_lastPeakEndColors[1][c] = m_nextPeakEndColors[1][c];
//      }
//      m_getRandomColor(m_nextPeakEndColors[0]);
//      m_getRandomColor(m_nextPeakEndColors[1]);
//      m_peakStepCount = 0;
//      m_peakCycleTime = 0;
//    }
//  }
// 
//  m_peakCycleTime += 10;
  
  //Draw new peaks, drop existing peaks
  for (int y = 0; y < m_peaks[x]; y++)
  {
    for(int c = 0; c < 3; c++)
      m_matrix[x][y][c] = m_peakColors[x][c];
    setPercentBrightness(x, y, 100);
  }
  
  int percentBrightness = 100 - static_cast<int>((100 * (millis() - m_setTime[x]))/m_decayTime);
  if (percentBrightness > 0)
  {  
    m_interpColors(x, m_peaks[x], m_peakColors[x], m_backgroundMatrix[x][m_peaks[x]], percentBrightness);
  }
  else
  {
    m_interpColors(x, m_peaks[x], m_peakColors[x], m_backgroundMatrix[x][m_peaks[x]], 0);     //Fill in pixel with background color
    m_peaks[x]--;                                                                       //Peak height = -1 once lowest pixel fades                                          
    m_setTime[x] = millis();                                                            //Reset duration counter for fading purposes
    if(m_peaks[x] >= 0)                                                  
      setPercentBrightness(x, m_peaks[x], 100);                                         //If the new peak is greater than -1, aka exists, set it's brightness to 100
  }
} 

void FrequencySpectrum::m_transitionPeakColors()
{
  
  if(m_peakCycleTime > 2*m_peakPeriod)
  {
    //Cycle end colors
    if((millis()-m_peakStepTime) > m_peakPeriod/m_peakSteps)
    {
      m_peakStepTime = millis();
      for(int c = 0; c < 3; c++)
      {
        m_peakColors[0][c] = (m_lastPeakEndColors[0][c]*(m_peakSteps-m_peakStepCount) + m_nextPeakEndColors[0][c]*m_peakStepCount)/m_peakSteps;
        m_peakColors[GRID_SIZE-1][c] = (m_lastPeakEndColors[1][c]*(m_peakSteps-m_peakStepCount) + m_nextPeakEndColors[1][c]*m_peakStepCount)/m_peakSteps;
      }
      m_peakStepCount++;
      Serial.println(m_peakStepCount);
    }
  
    // Fill in interior colors
    for(int i = 1; i < GRID_SIZE-1; i++)
    {
      for(int c = 0; c < 3; c++)
      {
        m_peakColors[i][c] = (m_peakColors[0][c] * (GRID_SIZE-1-i) + m_peakColors[GRID_SIZE-1][c] * i) / 7;
      }
    }
    
    if(m_peakStepCount > m_peakSteps)
    {
      for(int c = 0; c < 3; c++)
      {
        m_lastPeakEndColors[0][c] = m_nextPeakEndColors[0][c];
        m_lastPeakEndColors[1][c] = m_nextPeakEndColors[1][c];
      }
      m_getRandomColor(m_nextPeakEndColors[0]);
      m_getRandomColor(m_nextPeakEndColors[1]);
      m_peakStepCount = 0;
      m_peakCycleTime = 0;
    }
  }
 
  m_peakCycleTime += 10;

  return;
}

void FrequencySpectrum::m_outro(int fadeTime)
{
  int percentBrightness;
  unsigned long setTime = millis();
  unsigned long t = 0;

  m_fillSolidColor(m_backgroundColor);
  
  while(true)
  {
    for(int y = GRID_SIZE-1; y >= 0; y--)
    {
      percentBrightness = pow((fadeTime-static_cast<double>(t))/fadeTime, 4) * (50*(y+1))/(GRID_SIZE);
      for(int x = 0; x < GRID_SIZE; x++)
      {
        setPercentBrightness(x, y, percentBrightness);
      }
    } 
    FastLED.show();
    t = millis() - setTime; 
    
    if (t > fadeTime)
    {
      m_clear();
      return;
    }
  }
}

void FrequencySpectrum::m_sleep()
{
  while(true)
  {
    m_FHT();
    if(m_soundLevelSum > 8)
      return;
  }
}

void FrequencySpectrum::m_run()
{
  Serial.println("In Frequency Spectrum");
  
  m_getRandomColor(m_lastBackgroundColor);
  m_getRandomColor(m_nextBackgroundColor);
  
  m_clear(); 

  //Intro requires a known fade status
  for(int c = 0; c < 3; c++)
    m_backgroundColor[c] = m_lastBackgroundColor[c];
  
  m_intro();

  
  while(true)
    {
    //Sample sound, run FHT, update audio levels
    m_FHT();
  
    //Fill in the background
    m_drawBackground();

    //Terminate if button is pushed
    if(checkButton())
    {
      Serial.println("Returning Control");
      m_outro(1500);
      return;
    }
    
    //Logic for generating new colors and sleeping
    if(!m_animating) // If the screen isn't currently plotting frequencies
    {
      if(!m_newColors) // If the current color scheme has already been displayed
      {
        m_newSpectrumColors();
        m_newColors = true;
      }

      m_downTime += 10; //Time counter is an approximation, ¯\_(ツ)_/¯
      if(m_downTime > m_sleepTime)
      {
        m_outro(1500); //Slow fade out
        m_sleep();
        m_intro(500); //Fast fade in
        m_downTime = 0;
      }
    }
  
    
    m_animating = false; //Assume were not animating
    //Cycle through columns
    for(int x = 0; x < GRID_SIZE; x ++)
    {
      //If a new level exceeds a current peak update it and fill in collors below it
      if(m_peaks[x] <= m_soundLevels[x] && m_soundLevels[x] > 0)
      { 
        m_drawNewPeak(x);
        m_animating = true; m_newColors = false; m_downTime = 0; 
      }
  
      //Animate peak fading. Fade top pixel first, the drop peak and move on to next pixels
      else if(m_peaks[x] >= 0)
      {
        m_fadePeak(x);
        m_animating = true; m_downTime = 0;
      }   
    }   
    if(m_animating)
      m_transitionPeakColors();
    FastLED.show();
  } 
     
  return true;
}
