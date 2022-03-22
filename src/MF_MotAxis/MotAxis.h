#pragma once

namespace MotAxis
{
void Add(uint8_t analogPin);
void Clear();
void OnSetModule();
int16_t GetSetpoint(uint8_t stepper);
}
