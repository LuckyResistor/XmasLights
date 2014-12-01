//
// Xmas Lights
// ---------------------------------------------------------------------------
// (c)2014 by Lucky Resistor. See LICENSE for details.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//


#include <Adafruit_NeoPixel.h>
#include "Color.h"

// The global variable to access the NeoPixel.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, 7, NEO_GRB + NEO_KHZ800);

// Setup everything.
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  // Initialize the random number generator to start
  // with another number for each start. Using unused analog
  // inputs which will read more or less random values.
  randomSeed(analogRead(0)|
    (((uint32_t)analogRead(1))<<8)|
    (((uint32_t)analogRead(2))<<16));
    
  // Set pin 13 to output.
  // The LED on the Arduino UNO board will blink while waiting for
  // the next color.
  pinMode(13, OUTPUT);
}


// The current displayed colors (start with black).
Color c1;
Color c2;
Color c3;


void loop() {
  // Create some new colors.
  Color new1;
  Color new2;
  Color new3;
  getRandomColor(&new1, &new2, &new3);
  
  // Blend the current colors to the new colors.
  for (float i = 0.0f; i < 1.0f; i += 0.001) {
    strip.setPixelColor(0, c1.blendWith(new1, i).getColor());
    strip.setPixelColor(1, c2.blendWith(new2, i).getColor());
    strip.setPixelColor(2, c3.blendWith(new3, i).getColor());
    strip.show();
    delay(20);
  }

  // Assign the new colors and set the final colors.
  c1 = new1;
  c2 = new2;
  c3 = new3;
  strip.setPixelColor(0, c1.getColor());
  strip.setPixelColor(1, c2.getColor());
  strip.setPixelColor(2, c3.getColor());
  strip.show();
  
  // Show the colors for 2 minutes.
  // Using multiple delays, because the delay function has some
  // problems with large values.
  for (int i = 0; i < 120; ++i) {
    delay(1000);
    // blink the LED on the Arduino board to make clear the
    // board is still running and just waiting.
    digitalWrite(13, ((i&1)==0)?HIGH:LOW);
  }
}


/// Get three colors with a given hue shift.
///
/// @param c1 The 1st color to set.
/// @param c2 The 2nd color to set.
/// @param c3 The 3rd color to set. 
/// @param s1 The shift for the 2nd color. 0.0-1.0
/// @param s2 The shift for the 3rd color. 0.0-1.0
///
void getHueShift(Color *c1, Color *c2, Color *c3, float s1, float s2)
{
  // Create a random initial color value.
  float h1 = (float)(random(1024))/1024.0f;
  // Calculate the shifted values.
  float h2 = h1 + s1;
  float h3 = h1 + s2;
  // Limit the hue for the shifted values to 0.0-1.0;
  while (h2 > 1.0) h2 -= 1.0;
  while (h3 > 1.0) h3 -= 1.0;
  while (h2 < 0.0) h2 += 1.0;
  while (h3 < 0.0) h3 += 1.0;
  // Assign the colors.
  *c1 = Color(h1);
  *c2 = Color(h2);
  *c3 = Color(h3);
}


/// Create a new set of random colors.
///
/// @param c1 The 1st color to set.
/// @param c2 The 2nd color to set.
/// @param c3 The 3rd color to set. 
///
void getRandomColor(Color *c1, Color *c2, Color *c3)
{
  // Select one of 12 modes.
  uint8_t mode = random(12);
  // Assign colors according to the mode.
  switch (mode) {
    case 0:
    default:
      // Some white shades.
      *c1 = Color(1.0, 1.0, 1.0);
      *c2 = Color(0.8, 0.8, 0.8);
      *c3 = Color(0.6, 0.6, 0.6);
      break;
    case 1: // unicolor
      getHueShift(c1, c2, c3, 0.0, 0.0);
      break;
    case 2: // tri-color
      getHueShift(c1, c2, c3, 0.333, 0.666);
      break;
    case 3: // middle = contrast color
      getHueShift(c1, c2, c3, 0.5, 0.0);
      break;
    case 4: // strong variation
      getHueShift(c1, c2, c3, 0.2, 0.4);
      break;
    case 5: // little variantion
      getHueShift(c1, c2, c3, 0.1, 0.2);
      break;
    case 6: // strong variation (negative)
      getHueShift(c1, c2, c3, -0.2, -0.4);
      break;
    case 7: // little variation (negative)
      getHueShift(c1, c2, c3, -0.1, -0.2);
      break;
    case 8: // middle little variation.
      getHueShift(c1, c2, c3, 0.2, 0.0);
      break;
    case 9: // middle strong variation.
      getHueShift(c1, c2, c3, 0.4, 0.0);
      break;
    case 10: // middle little negative variation.
      getHueShift(c1, c2, c3, -0.2, 0.0);
      break;
    case 11: // middle strong negative variation.
      getHueShift(c1, c2, c3, -0.4, 0.0);
      break;
  }
}




