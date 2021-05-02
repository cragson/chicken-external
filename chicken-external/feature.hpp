#pragma once
#include <cstdint>

class feature
{
public:
	feature ()
	{
		this->m_status          = false;
		this->m_virtualkey_code = int32_t();
	}


	feature ( const bool _status , const int32_t _vk_code ) : m_status ( _status ), m_virtualkey_code ( _vk_code ) {}


	[[nodiscard]] bool is_active () const noexcept
	{
		return this->m_status;
	}


	void enable () noexcept
	{
		this->m_status = true;
	}


	void disable () noexcept
	{
		this->m_status = false;
	}


	void toggle () noexcept
	{
		this->m_status = !this->m_status;
	}


	void set_status ( const bool new_status ) noexcept
	{
		this->m_status = new_status;
	}


	[[nodiscard]] int32_t get_virtual_key_code () const noexcept
	{
		return this->m_virtualkey_code;
	}


	void set_virtual_key_code ( const int32_t vk_code ) noexcept
	{
		this->m_virtualkey_code = vk_code;
	}


	virtual void tick () = 0;


protected:
	bool m_status;

	int32_t m_virtualkey_code;
};
