#pragma once

namespace Analog
{
void Add(uint8_t pin, char const *name, uint8_t sensitivity);
void Clear();
void read();
int16_t getActualValue(uint8_t channel);
void readAverage();
}
