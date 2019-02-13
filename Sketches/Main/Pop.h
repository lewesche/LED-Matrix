#ifndef POP_H
#define POP_H

#include "Matrix.h"

class Pop : public Matrix
{
private:

  double m_period = 1000.0;
  double m_radius = 2.0;
  double m_maxOffset = sqrt(m_period/2);
  unsigned char m_color[3]; 
  int m_soundThreshold = 10;
  int m_soundSum = 0;

  void m_sumLevels();
  
  void m_fadePopIn(int x_center, int y_center);

  void m_fadePopOut(int x_center, int y_center);

public:
  bool m_run();

};

#endif
