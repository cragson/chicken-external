#include "process.hpp"
#include <TlHelp32.h>


bool process::setup_process ( const std::wstring& window_name )
{
	// First try to retrieve a window handle, the process id and a handle to the process with specific rights.
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

	// Before I set the retrieved data, I want to safe information about every image in the process
	// So I iterate over every image loaded into the certain process and store it information in a fresh struct :)
	MODULEENTRY32 me32            = { sizeof ( MODULEENTRY32 ) };
	const auto    snapshot_handle = CreateToolhelp32Snapshot ( TH32CS_SNAPALL, buffer );
	if ( !snapshot_handle )
		return false;
	if ( Module32First ( snapshot_handle, &me32 ) )
	{
		do
		{
			// check first if the image name already exists, as a key, in the map
			// if so, just skip the certain image
			if ( this->m_images.find ( me32.szModule ) != this->m_images.end() )
				continue;

			// create a new struct for the image name, which is the key for the map
			this->m_images[ me32.szModule ] = std::make_unique < imageinfo_t >();

			// now set the correct information
			this->m_images[ me32.szModule ]->image_base = reinterpret_cast < std::uintptr_t > ( me32.modBaseAddr );
			this->m_images[ me32.szModule ]->image_size = me32.modBaseSize;
		} while ( Module32Next ( snapshot_handle, &me32 ) );
	}

	// make sure to close the handle 
	CloseHandle ( snapshot_handle );

	// finally set the new data about the process
	this->m_hwnd   = window_handle;
	this->m_pid    = buffer;
	this->m_handle = proc_handle;
	return true;
}


bool process::patch_bytes ( const std::vector < std::byte >& bytes , const std::uintptr_t address , const size_t size )
{
	if ( bytes.empty() || !address || !size || bytes.size() > size )
		return false;
	DWORD buffer = 0;
	if ( !VirtualProtectEx ( this->m_handle, reinterpret_cast < LPVOID > ( address ), size, PAGE_EXECUTE_READWRITE
	                       , &buffer
		) )
		return false;
	for ( size_t i = 0; i < size; i++ )
		this->write < byte > ( address + i, 0x90 );
	for ( size_t i = 0; i < bytes.size(); i++ )
		this->write < std::byte > ( address + i, bytes.at ( i ) );
	if ( !VirtualProtectEx ( this->m_handle, reinterpret_cast < LPVOID > ( address ), size, buffer, &buffer ) )
		return false;
	return true;
}


bool process::patch_bytes ( const std::byte bytes[] , const std::uintptr_t address , const size_t size )
{
	if ( !address || !size )
		return false;
	DWORD buffer = 0;
	if ( !VirtualProtectEx ( this->m_handle, reinterpret_cast < LPVOID > ( address ), size, PAGE_EXECUTE_READWRITE
	                       , &buffer
		) )
		return false;
	for ( size_t i = 0; i < size; i++ )
		this->write < std::byte > ( address + i, bytes[ i ] );
	if ( !VirtualProtectEx ( this->m_handle, reinterpret_cast < LPVOID > ( address ), size, buffer, &buffer ) )
		return false;
	return true;
}


bool process::nop_bytes ( const std::uintptr_t address , const size_t size )
{
	if ( !address || !size )
		return false;
	DWORD buffer = 0;
	if ( !VirtualProtectEx ( this->m_handle, reinterpret_cast < LPVOID > ( address ), size, PAGE_EXECUTE_READWRITE
	                       , &buffer
		) )
		return false;
	for ( size_t i = 0; i < size; i++ )
		this->write < byte > ( address + i, 0x90 );
	if ( !VirtualProtectEx ( this->m_handle, reinterpret_cast < LPVOID > ( address ), size, buffer, &buffer ) )
		return false;
	return true;
}


std::unique_ptr < process > g_pGame = std::make_unique < process >();
