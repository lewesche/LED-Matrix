#ifndef COLORS_H
#define COLORS_H

#define MAX_BRIGHTNESS 151  // Don't wanna waist juice
#define RANDOM_SCALE 7      // Larger values modify default values less

struct color_t {
  unsigned char val[3]; // Keeping channel values together in an array makes it easy to work with iteration
  
  color_t(unsigned char r, unsigned char g, unsigned char b)
  {
    val[0]=r;
    val[1]=g;
    val[2]=b;
  }

  color_t()
  {
    val[0]=0;
    val[1]=0;
    val[2]=0;
  }

  inline unsigned char getR() { return val[0]; }
  inline unsigned char getG() { return val[1]; }
  inline unsigned char getB() { return val[2]; }

  inline void setR(unsigned char c) { val[0]=c; }
  inline void setG(unsigned char c) { val[1]=c; }
  inline void setB(unsigned char c) { val[2]=c; }

  inline void set(color_t c) {
    val[0] = c.val[0];
    val[1] = c.val[1];
    val[2] = c.val[2];
  }

  inline void zero() {
    val[0] = 0;
    val[1] = 0;
    val[2] = 0;
  }
};

color_t getRandomColor();

#endif
