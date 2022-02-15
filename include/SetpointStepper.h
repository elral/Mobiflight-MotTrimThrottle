#pragma once

namespace SetpointStepper
{
void Add();
void Clear();
void OnSetModule();
int16_t GetSetpoint(uint8_t _module);
}
