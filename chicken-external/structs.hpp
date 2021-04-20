#pragma once

#include <cstdint>

class GameSession
{
public:
	class SessionInstance* m_instance; //0x0000
	char pad_0004[28]; //0x0004
	int32_t m_tick; //0x0020
	char pad_0024[8]; //0x0024
	bool m_leftmouse_pressed; //0x002C
	char pad_002D[7]; //0x002D
	bool m_is_in_menu; //0x0034
	char pad_0035[3]; //0x0035
	int32_t m_current_ammo; //0x0038
	char pad_003C[252]; //0x003C
}; //Size: 0x0138


class SessionInstance
{
public:
	char pad_0000[212]; //0x0000
	class ShotCatcher* m_shotcatcher; //0x00D4
	class Cursor* m_cursor; //0x00D8
	class Game* m_game; //0x00DC
}; //Size: 0x00E0


class ShotCatcher
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044


class Cursor
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044


class Game
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044
