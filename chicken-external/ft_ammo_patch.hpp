#pragma once
#include "feature.hpp"
#include "process.hpp"
#include "offsets.hpp"
#include <stdexcept>

class ft_ammo_patch : public feature
{
public:

	void tick() override {}

	void on_enable() override
	{
		printf( "[#] Activated ammo patch!\n" );

		g_pGame->nop_bytes( g_pGame->get_image_base(L"MoorhuhnDeluxe.exe") + Offsets::OFFSET_AMMO_PATCH, 6 );
			
	}

	void on_disable() override
	{
		printf( "[#] Deactivated ammo patch!\n" );

		g_pGame->patch_bytes( this->m_original, g_pGame->get_image_base(L"MoorhuhnDeluxe.exe") + Offsets::OFFSET_AMMO_PATCH, 6 );
	}

	void on_first_activation() override
	{
		printf( "[#] Trying to read original bytes..\n" );

		const auto image_base = g_pGame->get_image_base ( L"MoorhuhnDeluxe.exe" );

		if( !image_base )
			throw std::runtime_error( "Could not retrieve the image base of MoorhuhnDeluxe.exe!" );
		
		for( size_t i = 0; i < 6; i++ )
			this->m_original.push_back( g_pGame->read< std::byte > ( image_base + Offsets::OFFSET_AMMO_PATCH + i ) );

		printf( "[#] Read the following bytes: " );

		for( const auto& elem : this->m_original )
			printf( "%02X ", elem );

		printf( "\n" );
	}


private:
	std::vector< std::byte > m_original;
};
