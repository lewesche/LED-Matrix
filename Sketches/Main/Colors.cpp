#include <Arduino.h>
#include "Colors.h"

// Truley randomly generating colors seems to lead to alot of brownish grey. 
// Picking from a pre-defined set of pretty ones and mutating a bit is easier
color_t COLORS[20] = {
  color_t(MAX_BRIGHTNESS, 0, 0),  // RED
  color_t(0, MAX_BRIGHTNESS, 0),  // BLUE
  color_t(0, 0, MAX_BRIGHTNESS),  // GREEN
  color_t(MAX_BRIGHTNESS, MAX_BRIGHTNESS, 0), // YELLOW
  color_t(MAX_BRIGHTNESS, MAX_BRIGHTNESS/1.15, 0), // GOLD
  color_t(MAX_BRIGHTNESS, MAX_BRIGHTNESS/1.65, 0), // YELLOW ORANGE
  color_t(MAX_BRIGHTNESS, MAX_BRIGHTNESS/2, 0), // ORANGE
  color_t(MAX_BRIGHTNESS, MAX_BRIGHTNESS/2.5, MAX_BRIGHTNESS/4), // SALMON
  color_t(MAX_BRIGHTNESS/1.15, MAX_BRIGHTNESS/12, MAX_BRIGHTNESS/4.25), // CRIMSON
  color_t(MAX_BRIGHTNESS/1.4, MAX_BRIGHTNESS/7/5, MAX_BRIGHTNESS/7.5),  // BRICK
  color_t(MAX_BRIGHTNESS, 0, MAX_BRIGHTNESS),  // PURPLE
  color_t(MAX_BRIGHTNESS, MAX_BRIGHTNESS/4, MAX_BRIGHTNESS/2.4),  // PINK
  color_t(MAX_BRIGHTNESS/2.2, 0, MAX_BRIGHTNESS), // INDIGO
  color_t(MAX_BRIGHTNESS/1.8, MAX_BRIGHTNESS/6, MAX_BRIGHTNESS/1.1),  // VIOLET
  color_t(0, MAX_BRIGHTNESS, MAX_BRIGHTNESS/2), // SPRING GREEN
  color_t(MAX_BRIGHTNESS/7.5, MAX_BRIGHTNESS/1.8, MAX_BRIGHTNESS/7.5),  // FOREST GREEN
  color_t(MAX_BRIGHTNESS/3, MAX_BRIGHTNESS/2.4, 0), // OLIVE
  color_t(MAX_BRIGHTNESS/1.6, MAX_BRIGHTNESS/2.7, MAX_BRIGHTNESS/8.7),  // BRONZE
  color_t(0, MAX_BRIGHTNESS/1.2, MAX_BRIGHTNESS), // TURQOIZE
  color_t(0, MAX_BRIGHTNESS/3.3, MAX_BRIGHTNESS/1.6)  // OCEAN BLUE
};

color_t getRandomColor()
{
  int r = random(20);
  color_t color = COLORS[r];

  for(int c = 0; c < 3; ++c)
  {
    int r_c = 0; // Random element to add to each channel
    
    if (color.val[c] == 0)
      continue;
      
    r_c = MAX_BRIGHTNESS/RANDOM_SCALE - random(MAX_BRIGHTNESS/(RANDOM_SCALE+1)); // Only gets brighter

    if ((int)color.val[c] + r_c >= MAX_BRIGHTNESS) {
      color.val[c] = MAX_BRIGHTNESS;
    } else {
      color.val[c] += r_c;
    }
  }
  return color;
}
