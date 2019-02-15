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
  unsigned char m_peakColors[GRID_SIZE][3];
  bool m_newColors = false;
  bool m_animating = false;
  unsigned long m_setTime[GRID_SIZE] {0};

  
  //Background Variables
  unsigned char m_backgroundMatrix[GRID_SIZE][GRID_SIZE][3];
  long m_backgroundStepTime = 0; 
  const long m_period = 30000;
  const int m_steps = 120;
  int m_stepCount = 0;
  unsigned char m_lastBackgroundColor[3];
  unsigned char m_nextBackgroundColor[3];
  unsigned char m_fadeColor[3] {0};
  
  //Sleep variables
  unsigned long m_downTime;
  const unsigned long m_sleepTime = 20000; // Goes to sleep after 20 seconds of no sound
  
  void m_intro(int fadeTime = 1000);
  
  void m_newSpectrumColors();

  void m_drawBackground();

  void m_drawNewPeak(int x);

  void m_fadePeak(int x);

  void m_outro(int fadeTime = 1000);

  void m_sleep();
  
public:
  bool m_run();

};

#endif
