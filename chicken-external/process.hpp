#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "image_info.hpp"

class process
{
public:
	process ()
	{
		this->m_handle = INVALID_HANDLE_VALUE;
		this->m_hwnd   = nullptr;
		this->m_pid    = 0;
	}


	~process ()
	{
		if ( this->m_handle )
			CloseHandle ( this->m_handle );
	}


	[[nodiscard]] HANDLE get_process_handle () const noexcept
	{
		return this->m_handle;
	}


	[[nodiscard]] HWND get_window_handle () const noexcept
	{
		return this->m_hwnd;
	}


	[[nodiscard]] DWORD get_process_id () const noexcept
	{
		return this->m_pid;
	}


	[[nodiscard]] bool is_process_ready () const noexcept
	{
		return this->m_handle != INVALID_HANDLE_VALUE && this->m_hwnd != nullptr && this->m_pid >= 0;
	}


	template < typename T >
	T read ( std::uintptr_t address , size_t size_of_read = sizeof ( T ) )
	{
		T buffer;
		ReadProcessMemory ( this->m_handle, reinterpret_cast < LPCVOID > ( address ), &buffer, size_of_read, nullptr );
		return buffer;
	}


	template < typename T >
	bool write ( std::uintptr_t address , T value )
	{
		return WriteProcessMemory ( this->m_handle, reinterpret_cast < LPVOID > ( address ), &value, sizeof( value )
		                          , nullptr
			) != 0;
	}


	bool setup_process ( const std::wstring& window_name );


	[[nodiscard]] std::uintptr_t get_image_base ( const std::wstring& image_name ) const
	{
		// if you pass here a name, which isn't inside the map... oh no no exception will hunt you.
		return this->m_images.at ( image_name ).get()->image_base;
	}


	[[nodiscard]] size_t get_image_size ( const std::wstring& image_name ) const
	{
		// if you pass here a name, which isn't inside the map... oh no no no no no no no no no no no no yes.
		return this->m_images.at ( image_name ).get()->image_size;
	}


	[[nodiscard]] size_t get_map_size () const noexcept
	{
		return this->m_images.size();
	}


	void print_images () const
	{
		for ( auto& image : this->m_images )
			printf ( "[+] %ls : 0x%08X - 0x%08X.\n", image.first.c_str(), image.second->image_base
			       , image.second->image_base + image.second->image_size
				);
		printf ( "\n\n" );
	}


	bool patch_bytes ( const std::vector < byte >& bytes , std::uintptr_t address , size_t size );


	bool patch_bytes ( const std::byte bytes[] , std::uintptr_t address , size_t size );


	bool nop_bytes ( std::uintptr_t address , size_t size );


private:
	HANDLE m_handle;

	HWND m_hwnd;

	DWORD m_pid;

	std::unordered_map < std::wstring , std::unique_ptr < imageinfo_t > > m_images;
};

extern std::unique_ptr < process > g_pGame;
