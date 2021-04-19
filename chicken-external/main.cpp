#include <iostream>
#include "process.hpp"

constexpr std::ptrdiff_t OFFSET_AMMO = 0x7D878;

constexpr std::ptrdiff_t OFFSET_POINTS = 0x7D85C;


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
	
	printf ( "[+] Image base: 0x%llX.\n", image_base );
	printf ( "[+] Ammo:       0x%llX.\n", image_base + OFFSET_AMMO );
	printf ( "[+] Points:     0x%llX.\n\n", image_base + OFFSET_POINTS );
	printf ( "[=] Features [=]\n" );
	printf ( "[>] NUMPAD1 => Increase Ammo\n" );
	printf ( "[>] NUMPAD2 => Increase Points\n" );
	printf ( "[>] END     => Exit\n" );
	
	const auto is_key_pressed = [] ( const int32_t vk_code )
	{
		return GetAsyncKeyState ( vk_code ) & 0x8000;
	};
	
	const auto get_current_ammo = [ &game, &image_base ] ()
	{
		return game->read < int32_t > ( image_base + OFFSET_AMMO );
	};
	
	const auto get_current_points = [ &game, &image_base ] ()
	{
		return game->read < int32_t > ( image_base + OFFSET_POINTS );
	};
	
	while ( true )
	{
		if ( is_key_pressed ( VK_END ) )
			break;
		
		if ( is_key_pressed ( VK_NUMPAD1 ) )
		{
			game->write < int32_t > ( image_base + OFFSET_AMMO, get_current_ammo() + 5 );
			Sleep ( 250 );
		}
		
		if ( is_key_pressed ( VK_NUMPAD2 ) )
		{
			game->write < int32_t > ( image_base + OFFSET_POINTS, get_current_points() + 100 );
			Sleep ( 250 );
		}
		
		Sleep ( 10 );
	}
	
	printf ( "Goodbye! :)\n" );
	
	return EXIT_SUCCESS;
}
