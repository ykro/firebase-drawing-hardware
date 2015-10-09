#include "application.h"
#include "neopixel/neopixel.h"

#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812B
#define LEDsW 32
#define LEDsH 16
#define matrixW 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDsW * LEDsH, PIXEL_PIN, PIXEL_TYPE);

void setup() {
  Serial.begin(9600);
  
  Particle.function("lightLED", lightLED);
  Particle.function("allOff", allOff);
  strip.begin();
  strip.setBrightness(80);
}

void loop() {
}

int unhex(char c) {
  return ((c >= '0') && (c <= '9')) ?      c - '0' :
         ((c >= 'a') && (c <= 'f')) ? 10 + c - 'a' :
         ((c >= 'A') && (c <= 'F')) ? 10 + c - 'A' : 0;
}

uint32_t getColor(String color) {
    char colorCharBuf[7];
    color.toCharArray(colorCharBuf, 7);
    return     strip.Color(
                            (unhex(colorCharBuf[1]) << 4) + unhex(colorCharBuf[2]),
                            (unhex(colorCharBuf[3]) << 4) + unhex(colorCharBuf[4]),
                            (unhex(colorCharBuf[5]) << 4) + unhex(colorCharBuf[6]));
}

void setLED(uint32_t x, uint32_t y, String color) {
  int offset = ((x/8) * 64) + ((y/8) * 256);
  x = (x%matrixW);
  y = (y%matrixW);
  int ledAddr = offset + x + (y * matrixW);  
  strip.setPixelColor(ledAddr, getColor(color));
}

int lightLED(String args) {
  int commaPosition = args.indexOf(",");
  int lastCommaPosition = args.lastIndexOf(",");
  
  int x = args.substring(0,commaPosition).toInt();
  int y = args.substring(commaPosition+1,lastCommaPosition).toInt();
  String color = args.substring(lastCommaPosition+1);
  setLED(x,y,color);
  strip.show();
  return 1;
}

int allOff(String args) { 
  strip.clear();
  strip.show();
  return 1;
}