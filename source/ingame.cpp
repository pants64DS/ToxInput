#include "controller.h"
#include "ingame.h"
#include "checkbox.h"
#include "emu.h"
#include <math.h>

#define pi 3.1415926535897932384626433832795

HANDLE EmuHandle = NULL;

unsigned player0 = 0;
unsigned oldPlayer0 = 0;

HANDLE GetProcessHandle(HWND wnd)
{
	long unsigned pid;
	GetWindowThreadProcessId(wnd, &pid);
	return OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
}

inline void EnableRunWithoutY()
{
	unsigned instruction = 0xe1500000; // cmp r0, r0
	WriteProcessMemory(EmuHandle, (void*)(0x0fa7c4 + ndsRAMoffset), &instruction, 4, 0);

	instruction = 0xe3a00001; // mov r0, #1
	WriteProcessMemory(EmuHandle, (unsigned*)(0x02c68c + ndsRAMoffset), &instruction, 4, 0);
}

void DisableRunWithoutY()
{
	unsigned instruction = 0xe3520000; // cmp r2, #0x0
	WriteProcessMemory(EmuHandle, (void*)(0x0fa7c4 + ndsRAMoffset), &instruction, 4, 0);

	instruction = 0xe5c90014; // strb r0, [r9, #0x14]
	WriteProcessMemory(EmuHandle, (unsigned*)(0x02c68c + ndsRAMoffset), &instruction, 4, 0);
}

void UpdateRunWithoutY()
{
	if (!player0) return;

	unsigned instruction;
	ReadProcessMemory(EmuHandle, (void*)(0x0fa7c4 + ndsRAMoffset), &instruction, 4, 0);

	if (instruction == 0xe3520000 || instruction == 0xe1500000)
	{
		bool b;
		if (y_checkbox.flags & CheckBox::checked)
		{
			EnableRunWithoutY();
			b = true;
		}
		else
		{
			DisableRunWithoutY();
			b = false;
		}
		WriteProcessMemory(EmuHandle, (void*)(0x09f4ac + ndsRAMoffset), &b, 1, 0);
	}
}

void EnableJoystickInput()
{
	void* instructionsToNop [] =
	{
		(void*)(0x02c510 + ndsRAMoffset),
		(void*)(0x02c6dc + ndsRAMoffset),
		(void*)(0x02c618 + ndsRAMoffset),
		(void*)(0x02c6f4 + ndsRAMoffset),
		(void*)(0x02c514 + ndsRAMoffset),
		(void*)(0x02c62c + ndsRAMoffset),
		(void*)(0x02c714 + ndsRAMoffset),
		(void*)(0x02c518 + ndsRAMoffset),
		(void*)(0x02c63c + ndsRAMoffset),
		(void*)(0x02c51c + ndsRAMoffset),
		(void*)(0x02c648 + ndsRAMoffset)
	};
	unsigned nop = 0xE1A00000;
	
	for (int i = 0; i < 11; i++)
	{
		WriteProcessMemory(EmuHandle, instructionsToNop[i], &nop, 4, 0);
	}

	struct
	{
		unsigned instruction_00 = 0xE5990020; // ldr r0,[r9,#0x20]
		unsigned instruction_04 = 0xE5890008; // str r0,[r9,#0x8]
		unsigned instruction_08 = 0xE5990024; // ldr r0,[r9,#0x24]
		unsigned instruction_0c = 0xE589000C; // str r0,[r9,#0xC]
		unsigned instruction_10 = 0xE5990004; // ldr r0,[r9,#0x4]
		unsigned instruction_14 = 0xE599101C; // ldr r1,[r9,#0x1C]
		unsigned instruction_18 = 0xE1811000; // orr r1,r1,r0
		unsigned instruction_1c = 0xE5891004; // str r1,[r9,#0x4]
	} code;

	WriteProcessMemory(EmuHandle, (void*)(0x02C6B4 + ndsRAMoffset), &code, sizeof(code), 0);
}

void DisableJoystickInput()
{
	void* noppedInstructions [] =
	{
		(void*)(0x02c510 + ndsRAMoffset),
		(void*)(0x02c6dc + ndsRAMoffset),
		(void*)(0x02c618 + ndsRAMoffset),
		(void*)(0x02c6f4 + ndsRAMoffset),
		(void*)(0x02c514 + ndsRAMoffset),
		(void*)(0x02c62c + ndsRAMoffset),
		(void*)(0x02c714 + ndsRAMoffset),
		(void*)(0x02c518 + ndsRAMoffset),
		(void*)(0x02c63c + ndsRAMoffset),
		(void*)(0x02c51c + ndsRAMoffset),
		(void*)(0x02c648 + ndsRAMoffset)
	};
	
	/*unsigned newInstructions [] =
	{
		0xe1c900b8, // strh r0, [r9, #0x8]
		0xe1c900b8, // strh r0, [r9, #0x8]
		0xe1c900b8, // strh r0, [r9, #0x8]
		0xe1c900ba, // strh r0, [r9, #0xa]
		0xe1c900ba, // strh r0, [r9, #0xa]
		0xe1c900ba, // strh r0, [r9, #0xa]
		0xe1c900bc, // strh r0, [r9, #0xc]
		0xe1c900bc, // strh r0, [r9, #0xc]
		0xe1c900bc, // strh r0, [r9, #0xc]
		0xe1c900be, // strh r0, [r9, #0xe]
		0xe1c900be, // strh r0, [r9, #0xe]
	}*/

	for (int i = 0; i < 11; i++)
	{
		unsigned strh_r0_r9_x = 0xe1c900b8 + (i / 3 << 1);
		WriteProcessMemory(EmuHandle, noppedInstructions[i], &strh_r0_r9_x, 4, 0);
	}

	struct
	{
		unsigned instruction_00 = 0xE1A01080; // mov     r1,r0,lsl #0x1
		unsigned instruction_04 = 0xE59F0164; // ldr     r0,=#0x20756B0
		unsigned instruction_08 = 0xE19000F1; // ldrsh   r0,[r0,r1]
		unsigned instruction_0c = 0xE1C900BE; // strh    r0,[r9,#0xE]
		unsigned instruction_10 = 0x059D0050; // ldreq   r0,[r13,#0x50]
		unsigned instruction_14 = 0x01C900B8; // streqh  r0,[r9,#0x8]
		unsigned instruction_18 = 0x01C900BA; // streqh  r0,[r9,#0xA]
		unsigned instruction_1c = 0x01C900BC; // streqh  r0,[r9,#0xC]
	} code;

	WriteProcessMemory(EmuHandle, (void*)(0x02C6B4 + ndsRAMoffset), &code, sizeof(code), 0);
}

void UpdateJoystickInput()
{
	if (((Emus[Emu::selectedEmu].flags | input_checkbox.flags) & Button::click_1_frame) || timer == 16)
	{
		if (input_checkbox.flags & CheckBox::checked)
			EnableJoystickInput();
		else
			DisableJoystickInput();
	}

	if (Controller::selectedController == -1 || !(input_checkbox.flags & CheckBox::checked) || IsCutsceneRunning() || !player0) return;

	Input myInput;
	ZeroMemory(&myInput, sizeof(Input));

	_XINPUT_STATE state;
	ZeroMemory(&state, sizeof(_XINPUT_STATE));

	if (XInputGetState(Controller::selectedController, &state) != ERROR_SUCCESS) return;

	float x = state.Gamepad.sThumbLX;
	float y = state.Gamepad.sThumbLY;

	if (abs(x) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || abs(y) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		x /= 0x8000;
		y /= 0x8000;

		float mag = sqrt(x*x + y*y);

		if (mag > 1)
		{
			x /= mag;
			y /= mag;
			mag = 1;
		}

		x *= 0x1000;
		y *= 0x1000;

		myInput.magnitude = mag * 0x1000;
		myInput.angle = -(short)(atan2(x, y) / pi * 0x8000) - 0x8000;
		myInput.dirX = x;
		myInput.dirZ = -y;
	}
	else
	{
		myInput.magnitude = myInput.dirX = myInput.dirZ = 0;
	}

	if (state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		myInput.buttonsHeld = (camera_checkbox.flags & Button::checked) ? Input::CAM_LEFT : Input::CAM_RIGHT;
	}
	else if (state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		myInput.buttonsHeld = (camera_checkbox.flags & Button::checked) ? Input::CAM_RIGHT : Input::CAM_LEFT;
	}

	WriteProcessMemory(EmuHandle, (Input*)input1, &myInput, sizeof(Input), 0);
}

bool IsGameCompatible(HANDLE pHandle)
{
	if (pHandle == NULL) return false;
	
	unsigned code1[7] =
	{
		0xE3500000,	// cmp     r0,0h
		0x059F0178,	// ldreq   r0,=20A0E58h
		0x01A01108,	// moveq   r1,r8,lsl 2h
		0x019000B1,	// ldreqh  r0,[r0,r1]
		0x020010F0,	// andeq   r1,r0,0F0h
		0xE1A00241,	// mov     r0,r1,asr 4h
		0xE3510000	// cmp     r1,0h
	};

	unsigned code2 [7];

	ReadProcessMemory(pHandle, (void*)(0x02C698 + ndsRAMoffset), code2, 28, 0);

	bool b = true;
	for (int i = 0; i < 7; i++)
		if (code1[i] != code2[i]) b = false;

	return b;
}

bool IsGameCompatible2(HWND wnd)
{
	HANDLE pHandle = GetProcessHandle(wnd);

	bool b = IsGameCompatible(pHandle);

	CloseHandle(pHandle);

	return b;
}

bool IsCutsceneRunning()
{
	if (EmuHandle == NULL) return false;

	unsigned KuppaPointer;
	ReadProcessMemory(EmuHandle, (unsigned*)(KuppaPointer_addr), &KuppaPointer, 4, 0);

	return KuppaPointer != 0;
}

char IsGamePaused() // 0 - not paused, 1 - paused, 2 - exiting pause
{
	char pause;
	ReadProcessMemory(EmuHandle, (unsigned*)(ndsRAMoffset + 0x09f2c4), &pause, 1, 0);

	return pause;
}

unsigned GetFrameCounter()
{
	unsigned frame_counter;
	ReadProcessMemory(EmuHandle, (void*)(ndsRAMoffset + 0x0a1040), &frame_counter, 4, 0);

	return frame_counter;
}

inline void InitJitModeCheck()
{
	struct // Inject this just before overlay 0
	{
		unsigned str_r0_r15_mCh = 0xe50f000c;
		unsigned ldrb_r0_r0 = 0xe5d00000;
		unsigned bx_lr = 0xe12fff1e;
	} code;

	WriteProcessMemory(EmuHandle, (void*)(0x0aa414 + ndsRAMoffset), &code, sizeof(code), 0);

	unsigned bl_0x020aa414 = 0xeb01f855;
	WriteProcessMemory(EmuHandle, (void*)(0x02c2b8 + ndsRAMoffset), &bl_0x020aa414, 4, 0);
}

unsigned prevFrameCounter = 0xffffffff;
bool jit_on = false;

bool IsJITModeCertainlyOn()
{
	unsigned frame_counter;
	frame_counter = GetFrameCounter();

	if (player0 == 0)
		return false;
	
	else if (oldPlayer0 == 0)
		prevFrameCounter = GetFrameCounter();

	unsigned a;
	ReadProcessMemory(EmuHandle, (void*)(0x0aa410 + ndsRAMoffset), &a, 4, 0);

	if (frame_counter > prevFrameCounter + 1) InitJitModeCheck();

	return a != 0x0209f2d8 && frame_counter > prevFrameCounter + 2;
}

unsigned GetPlayer0()
{
	unsigned player0;
	ReadProcessMemory(EmuHandle, (void*)(PLAYER_ARR), &player0, 4, 0);
	return player0;
}

void CloseEmu()
{
	if (!EmuHandle) return;

	DisableJoystickInput();
	DisableRunWithoutY();
	bool b = false;
	WriteProcessMemory(EmuHandle, (void*)(0x09f4ac + ndsRAMoffset), &b, 1, 0);
	
	CloseHandle(EmuHandle);
	EmuHandle = NULL;
}