#ifndef MATRIX_H
#define MATRIX_H
#include <FastLED.h>

#define NUM_LED 64

class Matrix
{
protected:
  unsigned char m_matrix[8][8][3];
  struct CRGB m_leds[NUM_LED];
  int m_soundLevels[8];
  
///////////////////////////////////////////////////////////////
////////////////////// General Functions //////////////////////
///////////////////////////////////////////////////////////////
public:
  Matrix();

  void m_clear();
  
protected:
  int cvtCoords(int x, int y);

  void setPercentBrightness(int x, int y, int percentColor);

  void m_interpColors(int x, int y, unsigned char (&oldColor)[3], unsigned char (&newColor)[3], int percent);

  void m_getRandomColor(unsigned char (&color)[3]);

  void m_FHT(int scale = 2);

  void m_fillSolidColor(unsigned char (&color)[3]);

public:
   virtual bool m_run() = 0;


};




#endif

