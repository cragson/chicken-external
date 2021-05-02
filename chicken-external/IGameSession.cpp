#include "IGameSession.hpp"

std::unique_ptr< IGameSession > g_pIGameSession = std::make_unique< IGameSession > ();