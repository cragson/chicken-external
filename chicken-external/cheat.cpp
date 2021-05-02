#include "cheat.hpp"
#include "ft_ammo_patch.hpp"
#include "utils.hpp"


bool cheat::setup_features ()
{
	// Holy shit, better hope nobody will notice this piece of garbage code
	// Ammo Patch
	this->m_features.push_back ( std::make_unique < ft_ammo_patch >() );
	this->m_features.at ( 0 )->set_virtual_key_code ( VK_NUMPAD0 );
	return true;
}


void cheat::run ()
{

	// If sky is blue enough, I will implement some proper logic here lol
	for ( const auto& feature : this->m_features )
	{
		if ( utils::is_key_pressed ( feature->get_virtual_key_code() ) )
		{
			feature->toggle();
			if ( feature->is_active() )
				feature->tick();
		}
	}
}


std::unique_ptr < cheat > g_pCheat = std::make_unique < cheat >();
