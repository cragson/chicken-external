#pragma once

#include <cstdint>

#include <Windows.h>

namespace utils
{

	inline bool is_key_pressed( const int32_t virtual_key_code )
	{
		return GetAsyncKeyState( virtual_key_code ) & 0x8000;
	}
}
