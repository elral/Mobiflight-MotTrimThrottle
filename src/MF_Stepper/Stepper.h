#pragma once

namespace Stepper
{
void Add(int pin1, int pin2, int pin3, int pin4, int btnPin1);
void Clear();
void OnSet();
void OnSetRelative();
void OnReset();
void OnSetZero();
void update();
}
