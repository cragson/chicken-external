#pragma once

#include <cstdint>

struct imageinfo_t
{
	std::uintptr_t image_base = std::uintptr_t();
	size_t		   image_size = size_t();
};
