#include "cheat.hpp"
#include "ft_ammo_patch.hpp"
#include "utils.hpp"
#include "ft_score_patch.hpp"
#include "ft_time_adder.hpp"
#include "process.hpp"


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

bool cheat::setup_offsets()
{
	if( !g_pGame )
		return false;
	
	const auto image = g_pGame->get_image_ptr_by_name( L"MoorhuhnDeluxe.exe" );

	if( !image )
		return false;

	const auto game_session_pattern = image->find_pattern( L"68 ? ? ? ? E8 ? ? ? ? 8B 7D 0C", false );

	if( !game_session_pattern )
		return false;

	// need here the +1 to skip the 68 byte (push offset)
	// also this offset isn't relative
	const auto game_session_offset = image->deref_address< uint32_t >( game_session_pattern + 1 );

	if( !game_session_offset )
		return false;

	Offsets::OFFSET_GAME_SESSION = game_session_offset;


	const auto points_patch_offset = image->find_pattern( L"89 0D ? ? ? ? E8" );

	if( !points_patch_offset )
		return false;
	
	Offsets::OFFSET_POINTS_PATCH = points_patch_offset;

	const auto ammo_patch_offset = image->find_pattern( L"FF 0D ? ? ? ? 33 D2" );

	if( !ammo_patch_offset )
		return false;
	
	Offsets::OFFSET_AMMO_PATCH = ammo_patch_offset;
	
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

void cheat::print_offsets()
{
	{
		printf( "\n" );

		const auto msg = [](const std::wstring& name, const std::uintptr_t value)
		{
			printf( "[>] %-20ls -> 0x%08X\n", name.c_str(), value );
		};

		msg( L"GameSession", Offsets::OFFSET_GAME_SESSION );
		msg( L"Ammo Patch", Offsets::OFFSET_AMMO_PATCH );
		msg( L"Points Patch", Offsets::OFFSET_POINTS_PATCH );

		printf( "\n" );
	}
}


std::unique_ptr < cheat > g_pCheat = std::make_unique < cheat >();
