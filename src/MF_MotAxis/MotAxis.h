#pragma once

namespace MotAxis
{
void Add(uint8_t analogPin, const char *syncName, uint8_t stepper, uint8_t enablePin, uint8_t startPosition, uint16_t movingTime, uint16_t maxSteps, uint16_t maxSpeed, uint16_t maxAccel);
void startPosition();
void Clear();
void update();
void OnSetModule();
}
