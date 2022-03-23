#pragma once

namespace MotAxis
{
void Add(uint8_t analogPin, uint8_t syncButton, uint8_t stepper, uint8_t enablePin, uint8_t temp);
void startPosition();
void Clear();
void update();
void OnSetModule();
}
