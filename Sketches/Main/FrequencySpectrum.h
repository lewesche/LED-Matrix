#ifndef FS_H
#define FS_H

#include "Matrix.h"

#define GRID_SIZE 8

class FrequencySpectrum : public Matrix
{
private:
  
  //Animation Variables
  int m_decayTime = 250;
  int m_peaks[GRID_SIZE] {0};
  unsigned char m_peakColors[GRID_SIZE][3];
  bool m_newColors = false;
  bool m_animating = false;
  unsigned long m_setTime[GRID_SIZE] {0};
  
  //Background Variables
  unsigned char m_backgroundMatrix[GRID_SIZE][GRID_SIZE][3];
  long m_backgroundStepTime = 0; 
  long m_period = 30000;
  int m_steps = 120;
  int m_stepCount = 0;
  unsigned char m_lastBackgroundColor[3];
  unsigned char m_nextBackgroundColor[3];
  
  
  void m_newSpectrumColors();

  void m_drawBackground();

  void m_drawNewPeak(int x);

  void m_fadePeak(int x);

public:
  bool m_run();

};

#endif

