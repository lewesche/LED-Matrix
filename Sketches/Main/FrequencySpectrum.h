#ifndef FS_H
#define FS_H

#include "Matrix.h"
#define GRID_SIZE 8

class FrequencySpectrum : public Matrix
{
private:
  
  //Animation Variables
  const int m_decayTime = 250; //Time it takes for one pixel in a peak to decay;
  int m_peaks[GRID_SIZE] {0};
  color_t m_lastPeakEndColors[2];
  color_t m_nextPeakEndColors[2];
  color_t m_peakColors[GRID_SIZE];
  const long m_peakPeriod = 5000;
  unsigned long m_peakCycleTime = 0;
  long m_peakStepTime = 0;
  const int m_peakSteps = 120;
  int m_peakStepCount = 0;
  bool m_newColors = false;
  bool m_animating = false;
  bool m_transitioning = false;
  unsigned long m_setTime[GRID_SIZE] {0};

  
  //Background Variables
  color_t m_backgroundMatrix[GRID_SIZE][GRID_SIZE];
  long m_backgroundStepTime = 0; 
  const long m_backgroundPeriod = 30000;
  const int m_backgroundSteps = 120;
  int m_backgroundStepCount = 0;
  color_t m_lastBackgroundColor;
  color_t m_nextBackgroundColor;
  color_t m_backgroundColor;
  
  //Sleep variables
  unsigned long m_downTime = 0;
  const unsigned long m_sleepTime = 3000; // Goes to sleep after 3 seconds of no sound
  
  void m_intro(int fadeTime = 1000);
  
  void m_newSpectrumColors();

  void m_drawBackground();

  void m_drawNewPeak(int x);

  void m_fadePeak(int x);

  void m_transitionPeakColors();

  void m_outro(int fadeTime = 2000);

  void m_sleep();
  
public:
  void m_run();

};

#endif
