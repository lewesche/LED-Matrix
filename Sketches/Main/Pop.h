#ifndef POP_H
#define POP_H

#include "Matrix.h"

class Pop : public Matrix
{
private:

  double m_period = 1000.0;
  double m_radius = 2.0;
  double m_maxOffset = 0.0;
  unsigned char m_color[3]; 
  int m_soundThreshold = 25;
  
  void m_fadePopIn(int x_center, int y_center);

  void m_fadePopOut(int x_center, int y_center);

public:
  bool m_run();

};

#endif
