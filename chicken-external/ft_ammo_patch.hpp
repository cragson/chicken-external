#pragma once
#include "feature.hpp"
#include "process.hpp"
#include "offsets.hpp"

class ft_ammo_patch : public feature
{
public:
	void tick () override
	{
		this->m_patched = !this->m_patched;
		if ( this->m_status && m_patched )
			g_pGame->patch_bytes ( { 0xFF, 0x0D, 0x78, 0xD8, 0x40, 0x00 }
			                     , g_pGame->get_image_base ( L"MoorhuhnDeluxe.exe" ) + Offsets::OFFSET_AMMO_PATCH, 6
				);
		else
			g_pGame->nop_bytes ( g_pGame->get_image_base ( L"MoorhuhnDeluxe.exe" ) + Offsets::OFFSET_AMMO_PATCH, 6 );
	}


private:
	bool m_patched = false;
};
