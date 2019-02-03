#include <FastLED.h>

#define LED_DATA 3
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define NUM_LED 64

int maxBrightness = 100; 

struct CRGB leds[NUM_LED];


void setup() {
  LEDS.addLeds<LED_TYPE, LED_DATA, COLOR_ORDER>(leds, NUM_LED); 
  //FastLED.setBrightness(maxBrightness);
}

void loop() {
//int t = 30;
//
//for(int i = 0; i < NUM_LED; i++)
//{
//  FastLED.clear();
//  leds[i].setRGB(50, 0, 0);
//  FastLED.show();
//  delay(t);
//}
//
//for(int i = 0; i < NUM_LED; i++)
//{
//  FastLED.clear();
//  leds[i].setRGB(0, 50, 0);
//  FastLED.show();
//  delay(t);
//}
//
//for(int i = 0; i < NUM_LED; i++)
//{
//  FastLED.clear();
//  leds[i].setRGB(0, 0, 50);
//  FastLED.show();
//  delay(t);
//}
//
//FastLED.clear();
//FastLED.show();
//delay(1000);


for(int c = 0; c < maxBrightness; c++)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(c, 0, 0);  
  }
  FastLED.show();
  delay(25);
}

for(int c = maxBrightness; c >=0; c--)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(c, 0, 0);  
  }
  FastLED.show();
  delay(25);
}


for(int c = 0; c < maxBrightness; c++)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(0, 0, c);  
  }
  FastLED.show();
  delay(25);
}

for(int c = maxBrightness; c >=0; c--)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(0, 0, c);  
  }
  FastLED.show();
  delay(25);
}



for(int c = 0; c < maxBrightness; c++)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(0, c, 0);  
  }
  FastLED.show();
  delay(25);
}

for(int c = maxBrightness; c >=0; c--)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(0, c, 0);  
  }
  FastLED.show();
  delay(25);
}







}




