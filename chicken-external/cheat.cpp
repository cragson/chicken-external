#include "cheat.hpp"
#include "ft_ammo_patch.hpp"
#include "utils.hpp"
#include "ft_score_patch.hpp"
#include "ft_time_adder.hpp"


bool cheat::setup_features ()
{
	// Holy shit, better hope nobody will notice this piece of garbage code

	// Ammo Patch
	this->m_features.push_back ( std::make_unique < ft_ammo_patch >() );
	this->m_features.at ( 0 )->set_virtual_key_code ( VK_NUMPAD0 );
	this->m_features.at ( 0 )->set_activation_delay ( 420 );

	// Score patch
	this->m_features.push_back ( std::make_unique< ft_score_patch > () );
	this->m_features.at ( 1 )->set_virtual_key_code ( VK_NUMPAD1 );
	this->m_features.at ( 1 )->set_activation_delay ( 420 );

	// Time adder
	this->m_features.push_back ( std::make_unique< ft_time_adder > () );
	this->m_features.at( 2 )->set_virtual_key_code ( VK_NUMPAD2 );
	this->m_features.at( 2 )->set_activation_delay ( 420 );

	
	return true;
}


void cheat::run ()
{

	// If the sky is blue enough, I will implement some proper logic here lol
	for ( const auto& feature : this->m_features )
	{
		// before tick'ing the feature, check first if the state will eventually change
		if ( utils::is_key_pressed( feature->get_virtual_key_code() ) )
			feature->toggle();
		
		// let the feature tick() when active
		if ( feature->is_active() )
			feature->tick();	
	}
}

void cheat::shutdown ()
{
	// disable every feature here
	for( const auto& feature : this->m_features )
		if( feature->is_active() )
			feature->disable();
}


std::unique_ptr < cheat > g_pCheat = std::make_unique < cheat >();
