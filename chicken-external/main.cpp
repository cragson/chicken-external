#include <iostream>
#include "process.hpp"
#include "cheat.hpp"
#include "utils.hpp"
#include "interfaces.hpp"

int main ()
{
	SetConsoleTitleW ( L"calb: Moorhuhn Deluxe" );

	// process setup
	printf ( "[+] Waiting for the game.." );
	while ( !g_pGame->setup_process ( L"Moorhuhn Deluxe" ) )
	{
		printf ( "." );
		Sleep ( 500 );
	}
	printf ( "done!\n" );
	
	printf ( "[+] Retrieved information about %d images!\n", g_pGame->get_map_size() );

	// interface setup
	printf ( "[+] Preparing game interfaces.." );
	while ( !interfaces::initialize() )
	{
		printf ( "." );
		Sleep ( 500 );
	}
	printf ( "done!\n" );

	// features setup
	printf ( "[+] Preparing cheat features.." );
	while ( !g_pCheat->setup_features() )
	{
		printf ( "." );
		Sleep ( 500 );
	}
	printf ( "done!\n" );
	
	printf ( "[+] %d cheat features are ready for usage!\n\n", g_pCheat->get_features_size() );

	
	// main loop
	while ( true )
	{
		g_pCheat->run();
		
		if ( utils::is_key_pressed ( VK_END ) )
			break;
		
		Sleep ( 10 );
	}

	g_pCheat->shutdown();

	printf( "Goodbye! :)\n" );
	
	return EXIT_SUCCESS;
}
