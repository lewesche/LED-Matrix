#include <Arduino.h>
#include "Matrix.h"
////LED Includes////
#include <FastLED.h>
////Audio Includes////
#define LIN_OUT8 1 // use the linear byte output function //For some reason these defines need to go before FTH.h library???
#define FHT_N 32 // set to 256 point fht
#include <FHT.h> // include the library


////Audio Defines////
//Macros - http://yaab-arduino.blogspot.co.il/2015/02/fast-sampling-from-analog-input.html
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define AmpMax (1024/2)
#define MicSamples (1024*2) // Not actually sure about these two variables...tweaked to get decent looking data


////LED Defines////
#define LED_DATA 3
#define BUTTON_PIN 5
#define COLOR_ORDER GRB
#define LED_TYPE WS2812B
#define NUM_LED 64
#define GRID_SIZE 8

/////// TURNING DEBUG MODES ON PRINTS DATA TO SERIAL PLOTTER ///////
////// MAKES MATRIX RUN MUCH WORSE, LEAVE OFF IN FINAL CONFIG //////
////////////////////// ONLY USE ONE AT A TIME //////////////////////
//#define DEBUG_SOUND_LEVELS
//#define DEBUG_MIC_INPUTS


///////////////////////////////////////////////////////////////
////////////////////// General Functions //////////////////////
///////////////////////////////////////////////////////////////

Matrix:: Matrix()
{
  ////Audio Setup////
  //Establish stable 3.3V reference voltage 
  analogReference(EXTERNAL); // 3.3V to AREF
  //Freeflow Mode - http://maxembedded.com/2011/06/the-adc-of-the-avr/
  ADCSRA = 0xe0+7; // "ADC Enable", "ADC Start Conversion", "ADC Auto Trigger Enable" and divider.
  ADMUX = 0x0; // use adc0 (hardcoded, doesn't use MicPin). Use ARef pin for analog reference (same as analogReference(EXTERNAL)).  
  
  ////LED Setup////
  LEDS.addLeds<LED_TYPE, LED_DATA, COLOR_ORDER>(m_leds, NUM_LED);
  void m_clear(); 
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  ////Button Setup////
  
}

bool Matrix::checkButton()
{
  if(digitalRead(BUTTON_PIN) == HIGH)
    return false;
  else
    return true;
}

int Matrix::cvtCoords(int x, int y)
{
  return 8*x + y;
}

void Matrix::setPercentBrightness(int x, int y, int percentColor)
{
  m_leds[cvtCoords(x, y)].setRGB(
    (m_matrix[x][y].getR()*percentColor)/100,
    (m_matrix[x][y].getG()*percentColor)/100, 
    (m_matrix[x][y].getB()*percentColor)/100);
}

void Matrix::m_interpColors(int x, int y, color_t &oldColor, color_t &newColor, int percent)
{
  m_leds[cvtCoords(x, y)].setRGB(
    (newColor.getR()*(100-percent) + oldColor.getR()*percent)/100,
    (newColor.getG()*(100-percent) + oldColor.getG()*percent)/100, 
    (newColor.getB()*(100-percent) + oldColor.getB()*percent)/100);
}

void Matrix::m_FHT()
{
  for (int i = 0; i < FHT_N; ++i) { // save FHT_N samples
    while (!(ADCSRA & /*0x10*/_BV(ADIF))); // wait for adc to be ready (ADIF)
    sbi(ADCSRA, ADIF); // restart adc
    byte m = ADCL; // fetch adc data
    byte j = ADCH;
    int k = ((int)j << 8) | m; // form into an int

    k -= 0x0200; // form into a signed int
    k <<= 6; // form into a 16b signed int
    //k <<= 0;
    fht_input[i] = k; // put real data into bins
  }

  //fht_window(); // window the data for better frequency response - seems to work better without?
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  fht_mag_lin8(); // take the output of the fht
  
  //Through experimenting with my microhpone (Adafruit MAX 9814 clone) I found 11 out of 16 frequency bins to be useful for normal sound, including
  //bins 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, and 13
  //To merge these 8 bins into 8 sound levels, I convolved an equally weighted 3 bin kernel across the set EXCEPT for the middle bin, bin 8, which I skipped.
  //This actually produced a nice subtle visual effect, helping to increase the visual division between low and high frequencies.

  m_soundLevelSum = 0;

  //Convolve across first half of bins
  for (int i = 4; i < 8; ++i)
  {
    m_soundLevels[i-4] = (fht_lin_out8[i-1] + fht_lin_out8[i] + fht_lin_out8[i+1])/(3*m_soundScale);
    if(m_soundLevels[i-4] > 7)
      m_soundLevels[i-4] = 7;
    m_soundLevelSum += m_soundLevels[i-4];
  }
  
  //Convolve across second half of bins
  for (int i = 9; i < 13; ++i)
  {
    m_soundLevels[i-5] = (fht_lin_out8[i-1] + fht_lin_out8[i] + fht_lin_out8[i+1])/(3*m_soundScale);
    if(m_soundLevels[i-5] > 7)
      m_soundLevels[i-5] = 7;
    m_soundLevelSum += m_soundLevels[i-5];
  }

//  Print statements for debugging and tweaking spectrum constants 

#ifdef DEBUG_SOUND_LEVELS

  for (int i = 0; i < 8; ++i)
  {
    Serial.print(m_soundLevels[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
#endif

#ifdef DEBUG_MIC_OUTPUTS
  for (int i = 3; i < 14; ++i)
  {
    Serial.print(fht_lin_out8[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
#endif

}

void Matrix::m_fillSolidColor(color_t &color)
{
  for(int x = 0; x < GRID_SIZE; ++x)       
    for(int y = 0; y < GRID_SIZE; ++y)
    {                      
      m_matrix[x][y] = color;
      m_leds[cvtCoords(x, y)].setRGB(color.getR(), color.getG(), color.getB());
    }     
}

void Matrix::m_clear()
{
  for(int x = 0; x < GRID_SIZE; ++x)
    for(int y = 0; y < GRID_SIZE; ++y)
    {
      m_matrix[x][y].zero();
      m_leds[cvtCoords(x, y)].setRGB(0, 0, 0);
    } 
  FastLED.show();
}

void Matrix::m_off()
{
  m_clear();
  //Serial.println("In off");
  while(checkButton()) // Wait for user to release the button
  {
  }
  
  while(true)
  {
    if(checkButton())
    {
      //Serial.println("Returning control");
      return;
    }
  }
}
