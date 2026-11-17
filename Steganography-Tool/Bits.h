#pragma once
#include <iostream>
#include <bitset>
class Bits
{
	bool GetBit(std::uint8_t value = 0b01010101, unsigned int index = 3);

	std::uint8_t SetBit(std::uint8_t value, unsigned int index);

	std::uint8_t ClearBit(std::uint8_t value, unsigned int index);

	void PrintBits(std::uint8_t value);

	std::uint8_t ToggleBit(std::uint8_t value = 0b00101101, unsigned int index = 3);

	std::uint8_t ToggleBitFast(std::uint8_t value = 0b00101101, unsigned int index = 3);

};

