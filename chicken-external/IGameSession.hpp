#pragma once
#include "process.hpp"

class IGameSession
{
public:
	IGameSession ()
	{
		this->m_ptr = std::uintptr_t();
	}


	IGameSession ( const std::uintptr_t _ptr ) : m_ptr { _ptr } { }


	void set_ptr ( const std::uintptr_t new_ptr )
	{
		this->m_ptr = new_ptr;
	}


	[[nodiscard]] std::uintptr_t get_session_ptr () const noexcept
	{
		return this->m_ptr;
	}


	[[nodiscard]] bool is_ptr_empty () const noexcept
	{
		return this->m_ptr == 0;
	}

	// TODO: Implement correct class for "SessionInstance"
	[[nodiscard]] LPVOID get_session_instance () const
	{
		return g_pGame->read < LPVOID > ( this->m_ptr );
	}


	[[nodiscard]] int32_t get_tick_count () const
	{
		return g_pGame->read < int32_t > ( this->m_ptr + 0x20 );
	}

	bool increase_tick_count( const int32_t ticks ) const 
	{
		// using abs() here to make sure the tick count is not decreased
		// undefined behaviour could happen if the tick count is overflowed..
		return g_pGame->write< int32_t >( this->m_ptr + 0x20, this->get_tick_count() + abs( ticks ) );
	}


	[[nodiscard]] bool is_left_mouse_pressed () const
	{
		return g_pGame->read < bool > ( this->m_ptr + 0x2C );
	}


	[[nodiscard]] bool is_menu_open () const
	{
		return g_pGame->read < bool > ( this->m_ptr + 0x34 );
	}


	[[nodiscard]] int32_t get_current_ammo () const
	{
		return g_pGame->read < int32_t > ( this->m_ptr + 0x38 );
	}


	bool set_tick_count ( const int32_t ticks ) const
	{
		return g_pGame->write < int32_t > ( this->m_ptr + 0x20, ticks );
	}


	bool set_current_ammo ( const int32_t ammo ) const
	{
		return g_pGame->write < int32_t > ( this->m_ptr + 0x38, ammo );
	}


private:
	std::uintptr_t m_ptr;
};

extern std::unique_ptr < IGameSession > g_pIGameSession;
