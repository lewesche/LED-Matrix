#ifndef POP_H
#define POP_H

#include "Matrix.h"

class Pop : public Matrix
{
private:

  double m_period = 1000.0;
  double m_radius = 2.0;
  double m_maxOffset = sqrt(m_period/2);
  color_t m_color; 
  int m_soundThreshold = 7;
  int m_soundSum = 0;
  
  bool m_fadePopIn(int x_center, int y_center);

  bool m_fadePopOut(int x_center, int y_center);

public:
  bool m_singlePop();

  void m_run();

};

#endif
