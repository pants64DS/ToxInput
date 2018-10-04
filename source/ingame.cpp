#include "ingame.h"
#include "checkbox.h"

HANDLE EmuHandle = NULL;

HANDLE GetProcessHandle(HWND wnd)
{
	long unsigned pid;
	GetWindowThreadProcessId(wnd, &pid);
	return OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
}

void PrepareGameForInput()
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
	
	/*unsigned mov_r0_1h = 0xe3a00001;
	unsigned mov_r1_1h = 0xe3a01001;
	unsigned mov_r3_1h = 0xe3a03001;

	WriteProcessMemory(EmuHandle, (unsigned*)(0x0d3bf8 + ndsRAMoffset), &mov_r3_1h, 4, 0);
	WriteProcessMemory(EmuHandle, (unsigned*)(0x0d3f3c + ndsRAMoffset), &mov_r0_1h, 4, 0);
	WriteProcessMemory(EmuHandle, (unsigned*)(0x0d43b4 + ndsRAMoffset), &mov_r1_1h, 4, 0);
	WriteProcessMemory(EmuHandle, (unsigned*)(0x0d4624 + ndsRAMoffset), &mov_r0_1h, 4, 0);
	WriteProcessMemory(EmuHandle, (unsigned*)(0x0d4684 + ndsRAMoffset), &mov_r0_1h, 4, 0);
	WriteProcessMemory(EmuHandle, (unsigned*)(0x0d4768 + ndsRAMoffset), &mov_r0_1h, 4, 0);
	WriteProcessMemory(EmuHandle, (unsigned*)(0x0d48c4 + ndsRAMoffset), &mov_r0_1h, 4, 0);*/

	if (y_checkbox.isChecked)
	{
		unsigned mov_r0_1h = 0xe3a00001;
		unsigned* pointer = (unsigned*)(0x02c68c + ndsRAMoffset);
		WriteProcessMemory(EmuHandle, pointer, &mov_r0_1h, 4, 0);
	}
}

bool IsGameCompatible(HANDLE pHandle)
{
	if (pHandle == NULL) return false;
	
	unsigned code1 [8] =
	{
		0xE3500000, // cmp     r0,0h
		0x059F0178, // ldreq   r0,=20A0E58h
		0x01A01108, // moveq   r1,r8,lsl 2h
		0x019000B1, // ldreqh  r0,[r0,r1]
		0x020010F0, // andeq   r1,r0,0F0h
		0xE1A00241, // mov     r0,r1,asr 4h
		0xE3510000, // cmp     r1,0h
	};

	unsigned code2 [7];

	ReadProcessMemory(pHandle, (void*)(0x02C698 + ndsRAMoffset), code2, 28, 0);

	bool b = true;
	for (int i = 0; i < 8; i++)
	{
		if (code1[i] != code2[i]) b = false;
	}
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
	unsigned* KuppaPointerPointer = (unsigned*)(KuppaPointer_addr);

	ReadProcessMemory(EmuHandle, KuppaPointerPointer, &KuppaPointer, 4, 0);

	return KuppaPointer != 0;
}
