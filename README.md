# LED-Matrix

## https://www.youtube.com/watch?v=jVYRgS695s0&fbclid=IwAR1OD_sazcJwwZFTdqP10BqMdPewttWTMSlTIxPb6Re7gO50KjPQsa7xVBo

The LED Matrix is a arduino driven 8x8 array of lights with an integrated microphone that hangs up on the wall. I designed the matrix as a fun little test bed for music visualization software, and it came out pretty cool so I thought I'd share. It costs about $40 for all the parts that go into one of these. 

## 3D Printed Hardware

Three parts make up the structure of the Matrix, and they are all easy to 3D print. The housing is just a bit larger than a 8 inch square. On my printer it takes about a full day of printing to make one of these. No screws or hardware are required. 

The printed parts include a housing that the other parts slide into, a set of dividers to seperate the light coming from each LED, and a top to seal it off. I would suggest printing with black PLA, since dark colors do a good job of keeping light from bleeding through cells. If a soldered connection or LED ever fails, its super easy to remove and work on the PCB. The only passive hardware not 3D printed is the 8 inch square black accrylic screen, which is a pretty easy thing to find for a couple bucks. 

## Electrical Hardware

I designed an 8 inch square PCB with spots for an arduino nano, microphone, DC jack, 64 LEDs, and a button. I've included a full BOM and gerber file for the PCB, which can easily be uploaded to any PCB manufacturers site. 

This thing requires a bit of home assembly for sure. The LED's are surface mount components, but they really aren't too hard to solder at home with some practice and a nice pointy iron. I didn't use any solder paste or anything special. Make sure you pay attention to the orientation of the LED's, arduino, and microphone! I included some seperate sketches that can be uploaded to the arduino to test the red, green, and blue channels of each LED during assembly. 

Note: The LED spec sheet calls for a capacitor connection between the ground and 5v input of each LED, but I ommited this for simplicity. So far I haven't noticed any issues with this, I suspect this is just to enable the LED's to change color at a ridiculously high framerate. 

## Software

I set up a simple software framework for controlling LED's and measuring audio input. The arduino runs in "freeflow mode", which lets the audio sample at around 10khz and measure frequencies up to 5khz. This could be tweaked to run faster, but I figured that 5khz was probably a reasonable top range. The cheap mic I used seems to have some trouble picking up low end sounds too. I omited the lowest frequency bin from the animation since they were much noisier and less responsive than the others, so the device is less sensitive to low end heavy sounds like a kick. 

### Dependencies

I used two awesome libraries for the project:

http://fastled.io/ - the ubiquitous FastLED

http://wiki.openmusiclabs.com/wiki/ArduinoFHT - Awesome FHT implementation by OpenMusicLabs

## Animations

### Frequency Spectrum Analzyer

This is most built out animation so far. It displays a colorful frequency spectrum with lows on the left and highs on the right. The spectrum and background colors change over time, or when the signal dies down.  

### Pop
This simple animation creates a coloful pop when a sound threshold is reached. 
