#pragma once
#include "feature.hpp"
#include "IGameSession.hpp"
#include "offsets.hpp"

class ft_score_patch : public feature
{
	
public:

	void tick() override {}

	void on_enable() override
	{
		printf( "[#] Activated score patch!\n" );

		g_pGame->write< short > ( g_pGame->get_image_base ( L"MoorhuhnDeluxe.exe" ) + Offsets::OFFSET_POINTS_PATCH, 0x05FF );
	}

	void on_disable() override
	{
		printf( "[#] Deactivating score patch!\n" );

		g_pGame->write< short > ( g_pGame->get_image_base ( L"MoorhuhnDeluxe.exe" ) + Offsets::OFFSET_POINTS_PATCH, 0x0D89 );
		
	}

	void on_first_activation() override {}
	
};
