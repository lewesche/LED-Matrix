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
#define MAX_BRIGHTNESS 151
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
    (m_matrix[x][y][0]*percentColor)/100,
    (m_matrix[x][y][1]*percentColor)/100, 
    (m_matrix[x][y][2]*percentColor)/100);
}

void Matrix::m_interpColors(int x, int y, unsigned char (&oldColor)[3], unsigned char (&newColor)[3], int percent)
{
  m_leds[cvtCoords(x, y)].setRGB(
    (newColor[0]*(100-percent) + oldColor[0]*percent)/100,
    (newColor[1]*(100-percent) + oldColor[1]*percent)/100, 
    (newColor[2]*(100-percent) + oldColor[2]*percent)/100);
}

void Matrix::m_getRandomColor(unsigned char (&color)[3])
{
  static unsigned char *COLOR_BANK[20];
  
  static unsigned char RED[3] = {MAX_BRIGHTNESS, 0, 0}; 
  COLOR_BANK[0] = RED;
  static unsigned char GREEN[3] {0, MAX_BRIGHTNESS, 0};
  COLOR_BANK[1] = GREEN;
  static unsigned char BLUE[3] {0, 0, MAX_BRIGHTNESS};
  COLOR_BANK[2] = BLUE;
  static unsigned char YELLOW[3] {MAX_BRIGHTNESS, MAX_BRIGHTNESS, 0};
  COLOR_BANK[3] = YELLOW;
  static unsigned char GOLD[3] {MAX_BRIGHTNESS, MAX_BRIGHTNESS/1.15, 0};
  COLOR_BANK[4] = GOLD;
  static unsigned char YELLOW_ORANGE[3] {MAX_BRIGHTNESS, MAX_BRIGHTNESS/1.65, 0};
  COLOR_BANK[5] = YELLOW_ORANGE;
  static unsigned char ORANGE[3] {MAX_BRIGHTNESS, MAX_BRIGHTNESS/2, 0};
  COLOR_BANK[6] = ORANGE;
  static unsigned char SALMON[3] {MAX_BRIGHTNESS, MAX_BRIGHTNESS/2.5, MAX_BRIGHTNESS/4};
  COLOR_BANK[7] = SALMON;
  static unsigned char CRIMSON[3] {MAX_BRIGHTNESS/1.15, MAX_BRIGHTNESS/12, MAX_BRIGHTNESS/4.25};
  COLOR_BANK[8] = CRIMSON;
  static unsigned char BRICK[3] {MAX_BRIGHTNESS/1.4, MAX_BRIGHTNESS/7/5, MAX_BRIGHTNESS/7.5};
  COLOR_BANK[9] = BRICK;
  static unsigned char PURPLE[3] {MAX_BRIGHTNESS, 0, MAX_BRIGHTNESS};
  COLOR_BANK[10] = PURPLE;
  static unsigned char PINK[3] {MAX_BRIGHTNESS, MAX_BRIGHTNESS/4, MAX_BRIGHTNESS/2.4};
  COLOR_BANK[11] = PINK;
  static unsigned char INDIGO[3] {MAX_BRIGHTNESS/2.2, 0, MAX_BRIGHTNESS};
  COLOR_BANK[12] = INDIGO;
  static unsigned char VIOLET[3] {MAX_BRIGHTNESS/1.8, MAX_BRIGHTNESS/6, MAX_BRIGHTNESS/1.1};
  COLOR_BANK[13] = VIOLET;
  static unsigned char SPRING_GREEN[3] {0, MAX_BRIGHTNESS, MAX_BRIGHTNESS/2};
  COLOR_BANK[14] = SPRING_GREEN;
  static unsigned char FOREST_GREEN[3] {MAX_BRIGHTNESS/7.5, MAX_BRIGHTNESS/1.8, MAX_BRIGHTNESS/7.5};
  COLOR_BANK[15] = FOREST_GREEN;
  static unsigned char OLIVE[3] {MAX_BRIGHTNESS/3, MAX_BRIGHTNESS/2.4, 0};
  COLOR_BANK[16] = OLIVE;
  static unsigned char BRONZE[3] {MAX_BRIGHTNESS/1.6, MAX_BRIGHTNESS/2.7, MAX_BRIGHTNESS/8.7};
  COLOR_BANK[17] = BRONZE;
  static unsigned char TURQOIZE[3] {0, MAX_BRIGHTNESS/1.2, MAX_BRIGHTNESS};
  COLOR_BANK[18] = TURQOIZE;
  static unsigned char OCEAN_BLUE[3] {0, MAX_BRIGHTNESS/3.3, MAX_BRIGHTNESS/1.6};
  COLOR_BANK[19] = OCEAN_BLUE;

  int r = random(20);
  
  for(int c = 0; c < 3; c++)
    {
    int r_c;
    if (COLOR_BANK[r][c] > MAX_BRIGHTNESS/10)
      r_c = random(MAX_BRIGHTNESS/5) - MAX_BRIGHTNESS/10;
    else
      r_c = 0;
    color[c] = COLOR_BANK[r][c] + r_c;
    if(color[c] < 0 || color[c] > MAX_BRIGHTNESS)
      color[c] = COLOR_BANK[r][c];
    }
}

void Matrix::m_FHT()
{
  for (int i = 0; i < FHT_N; i++) { // save FHT_N samples
    while (!(ADCSRA & /*0x10*/_BV(ADIF))); // wait for adc to be ready (ADIF)
    sbi(ADCSRA, ADIF); // restart adc
    byte m = ADCL; // fetch adc data
    byte j = ADCH;
    int k = ((int)j << 8) | m; // form into an int

    k -= 0x0200; // form into a signed int
    k <<= 6; // form into a 16b signed int
    k <<= 0;
    fht_input[i] = k; // put real data into bins
  }

  fht_window(); // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run(); // process the data in the fht
  fht_mag_lin8(); // take the output of the fht
  
  //Through experimenting with my microhpone (Adafruit MAX 9814 clone) I found 11 out of 16 frequency bins to be useful for every day sound, including
  //bins 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, and 13
  //To merge these 8 bins into 8 sound levels, I convolved an equally weighted 3 bin kernel across the set EXCEPT for the middle bin, bin 8, which I skipped.
  //This actually produced a nice subtle visual effect, helping to increase the visual division between low and high frequencies.

  //Convolve across first half of bins
  for (int i = 4; i < 8; i++)
  {
    m_soundLevels[i-4] = (fht_lin_out8[i-1] + fht_lin_out8[i] + fht_lin_out8[i+1])/(3*m_soundScale);
  }
  
  //Convolve across second half of bins
  for (int i = 9; i < 13; i++)
  {
    m_soundLevels[i-5] = (fht_lin_out8[i-1] + fht_lin_out8[i] + fht_lin_out8[i+1])/(3*m_soundScale);
  }

  m_soundLevelSum = 0;
  
  for(int i = 0; i < GRID_SIZE; i++)
  {
    if(m_soundLevels[i] > 7)
      m_soundLevels[i] = 7;

    m_soundLevelSum += m_soundLevels[i];
  }


  
//  Print statements for debugging and tweaking spectrum constants 

#ifdef DEBUG_SOUND_LEVELS

  for (int i = 0; i < 8; i++)
  {
    Serial.print(m_soundLevels[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
#endif

#ifdef DEBUG_MIC_OUTPUTS
  for (int i = 3; i < 14; i++)
  {
    Serial.print(fht_lin_out8[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
#endif

}

void Matrix::m_fillSolidColor(unsigned char (&color)[3])
{
  for(int x = 0; x < GRID_SIZE; x++)       
    for(int y = 0; y < GRID_SIZE; y++)
    {
      for(int c = 0; c < 3; c++)                        
        m_matrix[x][y][c] = color[c];
      m_leds[cvtCoords(x, y)].setRGB(color[0], color[1], color[2]);
    }     
}

void Matrix::m_clear()
{
  for(int x = 0; x < GRID_SIZE; x++)
    for(int y = 0; y < GRID_SIZE; y++)
    {
      for(int c = 0; c < 3; c++)
        m_matrix[x][y][c] = 0;
      m_leds[cvtCoords(x, y)].setRGB(0, 0, 0);
    }
  
  FastLED.show();
}

void Matrix::m_off()
{
  m_clear();

  while(checkButton()) // Wait for user to release the button
  {
  }
  
  while(true)
  {
    if(checkButton())
      return;
  }
}
