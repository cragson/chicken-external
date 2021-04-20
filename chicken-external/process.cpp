#include "process.hpp"
#include <TlHelp32.h>


bool process::setup_process ( const std::wstring& window_name )
{
	if ( window_name.empty() )
		return false;
	
	const auto window_handle = FindWindowW ( nullptr, window_name.c_str() );
	
	if ( !window_handle )
		return false;
	
	DWORD buffer = 0;
	
	if ( !GetWindowThreadProcessId ( window_handle, &buffer ) )
		return false;
	
	const auto proc_handle = OpenProcess ( PROCESS_ALL_ACCESS, FALSE, buffer );
	
	if ( !proc_handle )
		return false;
	
	this->m_hwnd   = window_handle;
	this->m_pid    = buffer;
	this->m_handle = proc_handle;
	
	return true;
}


uint32_t process::get_image_base ( const std::wstring& image_name ) const
{
	uint32_t      ret             = 0;
	
	MODULEENTRY32 me32            = { sizeof ( MODULEENTRY32 ) };
	
	const auto    snapshot_handle = CreateToolhelp32Snapshot ( TH32CS_SNAPALL, this->m_pid );
	
	if ( !snapshot_handle )
		return false;
	
	if ( Module32First ( snapshot_handle, &me32 ) )
	{
		do
		{
			if ( image_name == me32.szModule )
			{
				ret = reinterpret_cast < uint32_t > ( me32.modBaseAddr );
				break;
			}
		} while ( Module32Next ( snapshot_handle, &me32 ) );
	}
	
	CloseHandle ( snapshot_handle );
	
	return ret;
}

bool process::patch_bytes( const std::vector< byte >& bytes, const std::uintptr_t address, const size_t size )
{
	if( bytes.empty() || !address || !size || bytes.size() > size )
		return false;

	DWORD buffer = 0;
	
	if( !VirtualProtectEx ( this->m_handle, reinterpret_cast< LPVOID >( address ), size, PAGE_EXECUTE_READWRITE, &buffer ) )
		return false;

	for( size_t i = 0; i < size; i++ )
		this->write< byte >( address + i, 0x90 );

	for( size_t i = 0; i < bytes.size(); i++ )
		this->write< byte >( address + i, bytes.at( i ) );

	if( !VirtualProtectEx ( this->m_handle, reinterpret_cast< LPVOID >( address ), size, buffer, &buffer ) )
		return false;
	
	return true;
}
