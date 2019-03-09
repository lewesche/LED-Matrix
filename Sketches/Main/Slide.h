#ifndef SLIDE_H
#define SLIDE_H

#include "Matrix.h"

class Slide : public Matrix
{
private:
  unsigned char m_currentColor[3] {0};
  unsigned char m_nextColor[3] {0};
  unsigned char m_decayColor[3] {0};
  int m_transitionTime = 350;
  int m_soundThreshold = 7;
  double m_decayPeriod = 500;
  double m_decayTimer = 0;

  void m_newColors();
  
  bool m_transitionColors();
  
  void m_decayColors();
  
public:

  void m_run();

};

#endif
