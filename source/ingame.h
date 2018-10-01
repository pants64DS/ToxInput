#ifndef IN_GAME_INCLUDED
#define IN_GAME_INCLUDED

#include <windows.h>

#define ndsRAMoffset 0x1462C3840 // offset to 0x02000000 in nds address space

constexpr unsigned long long input0_addr = ndsRAMoffset + 0x09f49c;
constexpr unsigned long long input1_addr = ndsRAMoffset + 0x09f4b4;

constexpr unsigned long long KuppaPointer_addr = ndsRAMoffset + 0x09fc48;

struct Input
{
	enum Buttons : unsigned short
	{
		A = 1 << 0,
		B = 1 << 1,
		SELECT = 1 << 2,
		START = 1 << 3,
		RIGHT = 1 << 4,
		LEFT = 1 << 5,
		UP = 1 << 6,
		DOWN = 1 << 7,
		CAM_RIGHT = 1 << 8,
		CAM_LEFT = 1 << 9,
		R = 1 << 10,
		Y = 1 << 11,
		L = 1 << 14,
		X = 1 << 15
	};

	unsigned short buttonsHeld;
    unsigned short buttonsPressed;
	short magnitude;
    short dirX;
    short dirZ;
    short angle;
};

HANDLE GetProcessHandle(HWND wnd);
void PrepareGameForInput();
bool IsGameCompatible(HANDLE pHandle);
bool IsGameCompatible2(HWND wnd);
bool IsCutsceneRunning();

extern HANDLE EmuHandle;

#endif