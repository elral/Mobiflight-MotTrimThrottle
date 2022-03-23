#pragma once

namespace MotAxis
{
void Add(uint8_t analogPin, uint8_t syncButton, uint8_t stepper);
void Clear();
void update();
void OnSetModule();
int16_t GetSetpoint(uint8_t stepper);
}
