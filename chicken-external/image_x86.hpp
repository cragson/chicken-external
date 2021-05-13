#pragma once

#include "custom_data_types.hpp"

#include <cstdint>

class image_x86
{

public:

	image_x86()
	{
		this->m_base = std::uintptr_t();

		this->m_size = size_t();

		this->m_bytes = byte_vector();
	}

	image_x86( const std::uintptr_t image_base, const size_t image_size )
	{
		this->m_base = image_base;

		this->m_size = image_size;

		this->m_bytes = byte_vector();
		this->m_bytes.reserve( image_size );
	}

	[[nodiscard]] inline std::uintptr_t get_image_base() const noexcept
	{
		return this->m_base;
	}

	[[nodiscard]] inline size_t get_image_size() const noexcept
	{
		return this->m_size;
	}

	[[nodiscard]] inline bool is_byte_vector_empty() const noexcept
	{
		return this->m_bytes.empty();
	}

	[[nodiscard]] inline byte_vector* get_byte_vector_ptr() noexcept
	{
		return &this->m_bytes;
	}
	
	[[nodiscard]] inline std::byte* get_ref_to_first_byte() noexcept
	{
		// make sure that the vector isn't empty, so no exception can occur
		if( this->m_bytes.empty() )
			return nullptr;
		
		return &this->m_bytes.front();
	}

	[[nodiscard]] inline bool is_executable() const noexcept
	{
		if( this->m_bytes.empty() || this->m_bytes.size() < 2 )
			return false;
		
		return static_cast< int >(this->m_bytes.at( 0 )) == 0x4D && static_cast<int>(this->m_bytes.at( 1 )) == 0x5A;
	}

private:
	std::uintptr_t m_base;

	size_t m_size;

	byte_vector m_bytes;
};