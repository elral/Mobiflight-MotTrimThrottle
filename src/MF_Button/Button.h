#pragma once

namespace Button
{
void Add(uint8_t pin, char const *name);
void Clear();
void read();
void press(uint8_t button);
void release(uint8_t button);
void OnTrigger();
}
