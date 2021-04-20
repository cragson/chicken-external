#pragma once
#include <Windows.h>
#include <string>
#include <vector>

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
		return WriteProcessMemory ( this->m_handle, reinterpret_cast < LPVOID > ( address ), &value, sizeof( value ), nullptr ) != 0;
	}

	bool setup_process ( const std::wstring& window_name );


	[[nodiscard]] uint32_t get_image_base ( const std::wstring& image_name ) const;

	bool patch_bytes( const std::vector < byte > & bytes, const std::uintptr_t address, const size_t size );


private:
	HANDLE m_handle;

	HWND m_hwnd;

	DWORD m_pid;
};
