#pragma once

#include "feature.hpp"
#include "IGameSession.hpp"

class ft_time_adder : public feature
{

public:

	void tick() override
	{
		const auto current_tick_count = g_pIGameSession->get_tick_count();

		if( current_tick_count > 400000 )
			g_pIGameSession->set_tick_count ( 20000 );
		else
			g_pIGameSession->increase_tick_count( 3000 );
	}

	void on_enable() override
	{
		printf( "[#] Activated time adder!\n" );
	}

	void on_disable() override
	{
		printf( "[#] Deactivated time adder!\n" );
	}

	void on_first_activation() override {}

};