#ifndef POP_H
#define POP_H

#include "Matrix.h"

class Pop : public Matrix
{
private:
  void m_fadePopIn(int x_center, int y_center, int radius, unsigned char (&color)[3], double period);

  void m_fadePopOut(int x_center, int y_center, int radius, unsigned char (&color)[3], double period, double maxOffset);

public:
  bool m_run();

};

#endif

