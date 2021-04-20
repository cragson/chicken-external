#include <iostream>
#include "process.hpp"
#include "offsets.hpp"

int main ()
{
	SetConsoleTitleW ( L"calb: Moorhuhn Deluxe" );
	
	const auto game = std::make_unique < process >();
	
	printf ( "[+] Waiting for the game.." );
	
	while ( !game->setup_process ( L"Moorhuhn Deluxe" ) )
	{
		printf ( "." );
		Sleep ( 500 );
	}
	printf ( "done!\n" );
	
	const auto image_base = game->get_image_base ( L"MoorhuhnDeluxe.exe" );
	
	if ( !image_base )
	{
		printf ( "[!] Unable to retrieve the image base of the game, exiting now.\n" );

		return EXIT_FAILURE;
	}

	const auto game_session = image_base + Offsets::OFFSET_GAME_SESSION;
	
	printf ( "[+] Image base: 0x%08X.\n", image_base );
	printf ( "[+] Game session: 0x%08X.\n", game_session );
	printf ( "[+] Ammo:       0x%08X.\n", image_base + Offsets::OFFSET_AMMO );
	printf ( "[+] Points:     0x%08X.\n\n", image_base + Offsets::OFFSET_POINTS );
	
	printf ( "[=] Features [=]\n" );
	printf ( "[>] NUMPAD1 => Increase Ammo\n" );
	printf ( "[>] NUMPAD2 => Increase Points\n" );
	printf ( "[>] NUMPAD3 => Increase Time\n" );
	printf ( "[>] NUMPAD4 => Toggle Points Patch\n" );
	printf ( "[>] NUMPAD5 => Toggle Ammo Patch\n" );
	printf ( "[>] END     => Exit\n" );
	
	const auto is_key_pressed = [] ( const int32_t vk_code )
	{
		return GetAsyncKeyState ( vk_code ) & 0x8000;
	};
	
	const auto get_current_ammo = [ &game, &image_base ] ()
	{
		return game->read < int32_t > ( image_base + Offsets::OFFSET_AMMO );
	};
	
	const auto get_current_points = [ &game, &image_base ] ()
	{
		return game->read < int32_t > ( image_base + Offsets::OFFSET_POINTS );
	};

	bool points_patched = false;
	const std::vector< byte > original_point_bytes = { 0x89, 0x0D };
	const std::vector< byte > modified_point_bytes = { 0xFF, 0x05 };
	
	const auto patch_points = [ &points_patched, &game, &image_base, &original_point_bytes, &modified_point_bytes ]()
	{
		if( points_patched )
			game->patch_bytes (  original_point_bytes, image_base + Offsets::OFFSET_POINTS_PATCH, original_point_bytes.size() );
		else
			game->patch_bytes ( modified_point_bytes, image_base + Offsets::OFFSET_POINTS_PATCH, modified_point_bytes.size() );

		points_patched = !points_patched;
	};

	bool ammo_patched = false;
	const std::vector< byte > original_ammo_bytes = { 0xFF, 0x0D, 0x78, 0xD8, 0xEB, 0x00 };
	const std::vector< byte > modified_ammo_bytes = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

	const auto patch_ammo = [ &ammo_patched, &game, &image_base, &original_ammo_bytes, &modified_ammo_bytes ]()
	{
		if( ammo_patched )
			game->patch_bytes ( original_ammo_bytes, image_base + Offsets::OFFSET_AMMO_PATCH, original_ammo_bytes.size() );
		else
			game->patch_bytes ( modified_ammo_bytes, image_base + Offsets::OFFSET_AMMO_PATCH, modified_ammo_bytes.size() );

		ammo_patched = !ammo_patched;
	};

	const auto increase_time = [ &game_session, &game ]()
	{
		// if player is in the menu, just return.
		if( game->read < bool > ( game_session + 0x34 ) )
			return;

		game->write < int32_t >( game_session + 0x20, game->read< int32_t > ( game_session + 0x20 ) + 20420 );
	};
	
	while ( true )
	{
		if ( is_key_pressed ( VK_END ) )
			break;
		
		if ( is_key_pressed ( VK_NUMPAD1 ) )
		{
			game->write < int32_t > ( image_base + Offsets::OFFSET_AMMO, get_current_ammo() + 5 );
			Sleep ( 250 );
		}
		
		if ( is_key_pressed ( VK_NUMPAD2 ) )
		{
			game->write < int32_t > ( image_base + Offsets::OFFSET_POINTS, get_current_points() + 100 );
			Sleep ( 250 );
		}

		if ( is_key_pressed ( VK_NUMPAD3 ) )
		{
			increase_time();

			Sleep ( 250 );
		}

		if ( is_key_pressed ( VK_NUMPAD4 ) )
		{
			patch_points();

			Sleep( 500 );
		}

		if ( is_key_pressed ( VK_NUMPAD5 ) )
		{
			patch_ammo();

			Sleep( 500 );
		}
		
		Sleep ( 10 );
	}
	
	printf ( "Goodbye! :)\n" );
	
	return EXIT_SUCCESS;
}
