#pragma once

namespace Stepper
{
void Add(int pin1, int pin2, int pin3, int pin4, int btnPin1);
void Clear();
void OnSet();
void OnSetRelative();
void SetRelative(uint8_t _stepper, int16_t _pos);
void OnReset();
void OnSetZero();
void update();
}
