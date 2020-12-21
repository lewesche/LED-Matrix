#ifndef MATRIX_H
#define MATRIX_H
#include <FastLED.h>
#include "Colors.h"

#define NUM_LED 64

class Matrix
{
protected:
  color_t m_matrix[8][8];
  struct CRGB m_leds[NUM_LED];
  int m_soundScale = 3;
  int m_soundLevels[8];
  int m_soundLevelSum = 0;
///////////////////////////////////////////////////////////////
////////////////////// General Functions //////////////////////
///////////////////////////////////////////////////////////////
public:
  Matrix();

  void m_clear();
  
protected:
  bool checkButton();

  // Converts X Y coords to 0 < coord < NUM_LED
  int cvtCoords(int x, int y);

  void setPercentBrightness(int x, int y, int percentColor);

  void m_interpColors(int x, int y, color_t &oldColor, color_t &newColor, int percent);

  void m_FHT();

  void m_fillSolidColor(color_t &color);

public:
   virtual void m_run() = 0;
   
   void m_off();
};




#endif
