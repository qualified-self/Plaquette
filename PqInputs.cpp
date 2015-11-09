/*
 * PqInputs.cpp
 *
 * (c) 2015 Sofian Audry        :: info(@)sofianaudry(.)com
 * (c) 2015 Thomas O Fredericks :: tof(@)t-o-f(.)info
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PqInputs.h"

AnalogIn::AnalogIn(uint8_t pin, uint8_t mode)
  : PqPinComponent(pin, mode), PqGetter(), _value(-1), _alpha(1)
{}

void AnalogIn::smooth(float factor) {
  factor = max(factor, 0); // make sure factor >= 0
  _alpha = (factor > 1 ?
      2 / (factor + 1) :
      factor);
}

float AnalogIn::get() {
  float v = read();
  // Update value.
  if (_value < 0) // value never initialized
    return (_value = v);
  else // moving average
    return (_value -= _alpha * (_value - v));
}

float AnalogIn::read() {
  // Convert
  int rawValue = analogRead(_pin);
  if (_mode == ANALOG_INVERTED)
    rawValue = 1023 - rawValue;
  return rawValue / 1023.0f;
}

DigitalIn::DigitalIn(uint8_t pin, uint8_t mode)
  : PqPinComponent(pin, mode), PqDigitalGetter()
{}

bool DigitalIn::isOn() {
  bool isHigh = digitalRead(_pin) == HIGH;
  if (_mode == EXTERNAL_PULLDOWN) // inverted
    isHigh = !isHigh;
  return isHigh;
}

void DigitalIn::setup() {
  pinMode(_pin, _mode == INTERNAL_PULLUP ? INPUT_PULLUP : INPUT);
}