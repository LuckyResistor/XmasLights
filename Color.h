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
#pragma once


#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


/// A class to represent a single color.
///
/// Note the following things you should not do in your project:
/// - This class is using float which is a bad idea for microcontrollers
///   unless you have CPU time to waste. We have CPU time to waste in
///   this project.
/// - This class has no implementation file. This is working well, because
///   the project is very small and this class is only used in one module.
///
class Color
{
public:
  /// Create the default color black.
  ///
  Color()
    : _r(0.0f), _g(0.0f), _b(0.0f)
  {
  } 

  /// Create a color from red, green and blue values.
  ///
  /// Each value should be between 0.0 and 1.0.
  ///
  /// @param r The red value.
  /// @param g The green value.
  /// @param b The blue value.
  ///
  Color(float r, float g, float b)
    : _r(r), _g(g), _b(b)
  {
    checkLimits();
  }
  
  /// Create a color from a single hue value.
  ///
  /// The hue value should be between 0.0 and 1.0. The hue
  /// value represents the value on the "color wheel" and
  /// is starting with 0.0 red to 0.33 green and 0.66 blue
  /// and from there back to 1.0/0.0 red. 
  /// 
  /// @param h The hue value 0.0-1.0.
  ///
  Color(float h)
  {
    // Check the phase 
    const int p = h*3.0f;
    // Check the mix in the phase.
    const float m = (h*3.0f)-((float)(p));
    switch (p) {
      case 0: // phase red-green
        _r = 1.0f-m;
        _g = m;
        _b = 0.0f;
        break;
      case 1: // phase green-blue
        _r = 0.0f;
        _g = 1.0f-m;
        _b = m;
        break;
      case 2: // phase blue-red
        _r = m;
        _g = 0.0f;
        _b = 1.0f-m;
        break;
    }
    checkLimits();
  }
  
  /// Blend this color with another.
  ///
  /// @param other The other color.
  /// @param factor The blend factor from 0.0-1.0.
  ///
  Color blendWith(const Color &other, float factor)
  {
    return Color(
      ((_r * (1.0f-factor)) + (other._r * factor)),
      ((_g * (1.0f-factor)) + (other._g * factor)),
      ((_b * (1.0f-factor)) + (other._b * factor)));
  }
  
  /// Get the color value as 32bit integer for the NeoPixel.
  ///
  /// @return The 32bit value to send to the NeoPixel library.
  ///
  uint32_t getColor() const 
  {
    const uint8_t r = (uint8_t)(255.0f * _r);
    const uint8_t g = (uint8_t)(255.0f * _g);
    const uint8_t b = (uint8_t)(255.0f * _b);
    return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
  }
 
private:
  /// Check the limits of all color values.
  ///
  void checkLimits() {
    if (_r < 0.0f) _r = 0.0f;
    if (_r > 1.0f) _r = 1.0f;
    if (_g < 0.0f) _g = 0.0f;
    if (_g > 1.0f) _g = 1.0f;
    if (_b < 0.0f) _b = 0.0f;
    if (_b > 1.0f) _b = 1.0f;    
  }
  
private:
  float _r; // red
  float _g; // green
  float _b; // blue
};


