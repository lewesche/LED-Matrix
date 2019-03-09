#include <FastLED.h>

#define LED_DATA 3
#define BUTTON_PIN 5
#define COLOR_ORDER GRB
#define LED_TYPE WS2812
#define NUM_LED 64

int maxBrightness = 100; 
int wait_t = 15;

struct CRGB leds[NUM_LED];


void setup() {
  LEDS.addLeds<LED_TYPE, LED_DATA, COLOR_ORDER>(leds, NUM_LED); 
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {

while(digitalRead(BUTTON_PIN) == HIGH)
{
  Serial.println("off");
}

Serial.println("on");

for(int c = 0; c < maxBrightness; c++)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(c, 0, 0);  
  }
  FastLED.show();
  delay(wait_t);
}

for(int c = maxBrightness; c >=0; c--)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(c, 0, 0);  
  }
  FastLED.show();
  delay(wait_t);
}


for(int c = 0; c < maxBrightness; c++)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(0, 0, c);  
  }
  FastLED.show();
  delay(wait_t);
}

for(int c = maxBrightness; c >=0; c--)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(0, 0, c);  
  }
  FastLED.show();
  delay(wait_t);
}



for(int c = 0; c < maxBrightness; c++)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(0, c, 0);  
  }
  FastLED.show();
  delay(wait_t);
}

for(int c = maxBrightness; c >=0; c--)
{
  FastLED.clear();
  for(int i = 0; i < NUM_LED; i++)
  {
    leds[i].setRGB(0, c, 0);  
  }
  FastLED.show();
  delay(wait_t);
}







}
