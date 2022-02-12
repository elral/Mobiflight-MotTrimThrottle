#pragma once

namespace LedSegment
{
void Add(int dataPin, int csPin, int clkPin, int numDevices, int brightness);
void Clear();
void OnInitModule();
void OnSetModule();
uint16_t GetSetpoint(uint8_t _module);
}
