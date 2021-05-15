#pragma once

#include "IGameSession.hpp"

#include "offsets.hpp"

namespace interfaces
{

	inline bool initialize()
	{
		g_pIGameSession->set_ptr ( Offsets::OFFSET_GAME_SESSION );
	
		return true;
	}

}