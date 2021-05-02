#pragma once

#include "IGameSession.hpp"

#include "offsets.hpp"

namespace interfaces
{

	inline bool initialize()
	{
		g_pIGameSession->set_ptr ( g_pGame->get_image_base( L"MoorhuhnDeluxe.exe" ) + Offsets::OFFSET_GAME_SESSION );
	
		return true;
	}

}