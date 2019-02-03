#ifndef MATRIX_H
#define MATRIX_H
#include <FastLED.h>

#define NUM_LED 64

class Matrix
{
private:
  unsigned char m_matrix[8][8][3];
  struct CRGB m_leds[NUM_LED];
  int m_soundLevels[8];
  
///////////////////////////////////////////////////////////////
////////////////////// General Functions //////////////////////
///////////////////////////////////////////////////////////////
public:
  Matrix();

  void m_clear();
  
private:
  int cvtCoords(int x, int y);

  void setPercentBrightness(int x, int y, int percentColor);

  void m_interpColors(int x, int y, unsigned char (&oldColor)[3], unsigned char (&newColor)[3], int percent);

  void m_getRandomColor(unsigned char (&color)[3]);

  void m_FHT(int scale = 2);

  void m_fillSolidColor(unsigned char (&color)[3]);
  
///////////////////////////////////////////////////////////////
///////////////////////// Pop Effect //////////////////////////
///////////////////////////////////////////////////////////////

private:
  void m_fadePopIn(int x_center, int y_center, int radius, unsigned char (&color)[3], double period);

  void m_fadePopOut(int x_center, int y_center, int radius, unsigned char (&color)[3], double period, double maxOffset);

public:
  bool m_pop(double period = 3000.0, double radius = 2.0, int R = 0, int G = 0, int B = 0, double maxOffset = 0.0);

///////////////////////////////////////////////////////////////
///////////////////// Frequency Analyzer //////////////////////
///////////////////////////////////////////////////////////////
private:
  void m_newSpectrumColors(unsigned char (&peakColors)[8][3]);

  void m_drawBackground(unsigned char (&backgroundMatrix)[8][8][3], long &backgroundStepTime, long &period, int &steps, int &stepCount, unsigned char (&lastBackgroundColor)[3], unsigned char (&nextBackgroundColor)[3]);

  void m_drawNewPeak(int x, bool &newColors, bool &animating, int (&peaks)[8], unsigned char (&peakColors)[8][3], unsigned long (&setTime)[8]);

  void m_fadePeak(int x, bool &animating, int (&peaks)[8], unsigned char (&peakColors)[8][3], unsigned long (&setTime)[8], unsigned char (&backgroundMatrix)[8][8][3], int (&decayTime));

public:
  bool m_drawFrequencySpectrum(int decayTime = 250);

};

#endif

