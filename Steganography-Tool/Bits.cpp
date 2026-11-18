#include "Bits.h"

bool Bits::GetBit(std::uint8_t value, unsigned int index)
{
	std::uint8_t shift = value >> index & 1;
	return shift;
}

std::uint8_t Bits::SetBit(std::uint8_t value, unsigned int index)
{

	std::uint8_t take = 1u << index;
	std::uint8_t result = value | take;
	return result;
}

std::uint8_t Bits::ClearBit(std::uint8_t value, unsigned int index)
{
	std::uint8_t take = 1u << index;
	std::uint8_t result = value & ~take;
	return result;
}

void Bits::PrintBits(std::uint8_t value)
{
	for (int i = 7; i >= 0; --i)
	{
		if (GetBit(value, i))
		{
			std::cout << "1";
		}
		else
		{
			std::cout << "0";
		}
		//char result = GetBit(value, i) ? '1' : '0';
		//std::cout << (GetBit(value, i) ? '1' : '0');
	}


}

std::uint8_t Bits::ToggleBit(std::uint8_t value, unsigned int index)
{
	std::uint8_t mask = 1u << index; //     0b00001000
	std::uint8_t result = value ^ mask; //  0b00100101
	return result;
}

std::uint8_t Bits::ToggleBitFast(std::uint8_t value, unsigned int index)
{
	return static_cast<std::uint8_t>(value ^ (1u << index));

}
