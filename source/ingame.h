#ifndef IN_GAME_INCLUDED
#define IN_GAME_INCLUDED

#include <windows.h>

#define ndsRAMoffset 0x1462C3840 // offset to 0x02000000 in nds address space

constexpr unsigned long long input0 = ndsRAMoffset + 0x09f49c;
constexpr unsigned long long input1 = ndsRAMoffset + 0x09f4b4;

constexpr unsigned long long KuppaPointer_addr = ndsRAMoffset + 0x09fc48;
constexpr unsigned long long PLAYER_ARR = ndsRAMoffset + 0x09f394;

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

void UpdateRunWithoutY();
void DisableRunWithoutY();

void EnableJoystickInput();
void DisableJoystickInput();
void UpdateJoystickInput();

bool IsGameCompatible(HANDLE pHandle);
bool IsGameCompatible2(HWND wnd);

bool IsCutsceneRunning();
char IsGamePaused();
unsigned GetFrameCounter();

bool IsJITModeCertainlyOn();

unsigned GetPlayer0();

void CloseEmu();

extern HANDLE EmuHandle;

extern unsigned player0;
extern unsigned oldPlayer0;

extern unsigned prevFrameCounter;
extern bool jit_on;

enum States // Credits for these go to Josh65536
{
	ST_LEDGE_GRAB         = 0x02110004,
	ST_CEILING_GRATE      = 0x0211001c,
	ST_YOSHI_POWER        = 0x02110034, //tongue, spitting, throwing egg, breathing fire
    ST_SWALLOW            = 0x0211004c,
	
	
	ST_HURT               = 0x02110094,
    ST_HURT_WATER         = 0x021100ac,
	ST_ELECTROCUTE        = 0x021100c4,
	ST_BURN_FIRE          = 0x021100dc,
	ST_BURN_LAVA          = 0x021100f4,
	ST_DEAD_HIT           = 0x0211010c,
	ST_DEAD_PIT           = 0x02110124,
    ST_WALK               = 0x0211013c,
    ST_WAIT               = 0x02110154,
	ST_GRABBED            = 0x0211016c,
	ST_TURN_AROUND        = 0x02110184,
	ST_JUMP               = 0x0211019c,
    ST_FALL               = 0x021101b4,
	ST_THROWN             = 0x021101cc,
	ST_SIDE_FLIP          = 0x021101e4,
	ST_SLIDE_KICK_RECOVER = 0x021101fc,
	ST_FLY                = 0x02110214,
	ST_NO_CONTROL         = 0x0211022c, //includes caps
	ST_OWL                = 0x02110244,
	
	ST_WIND_CARRY         = 0x02110274,
	ST_BALLOON            = 0x0211028c,
	ST_TELEPORT           = 0x021102a4,
	
	ST_CANNON             = 0x021102d4,
	ST_SQUISH             = 0x021102ec,
	ST_SHELL              = 0x02110304,
	ST_STOMACH_SLIDE      = 0x0211031c,
	ST_BUTT_SLIDE         = 0x02110334,
	ST_DIZZY_STARS        = 0x0211034c,
	ST_HOLD_LIGHT         = 0x02110364,
	ST_BONK               = 0x0211037c,
	ST_HOLD_HEAVY         = 0x02110394,
	ST_WALL_SLIDE         = 0x021103ac,
	
	ST_WALL_JUMP          = 0x021103dc,
	ST_SLOPE_JUMP         = 0x021103f4,
	ST_STUCK_IN_GROUND    = 0x0211040c,
    ST_LAND               = 0x02110424,
	ST_ON_WALL            = 0x0211043c,
	ST_SPIN               = 0x02110454,
	ST_TALK		          = 0x0211046c,
	ST_CRAZED_CRATE       = 0x02110484,
	
	ST_LEVEL_ENTER        = 0x021104b4,
	
	ST_CROUCH             = 0x021104e4,
	
	ST_CRAWL              = 0x02110514,
	ST_BACK_FLIP          = 0x0211052c,
	
	ST_LONG_JUMP          = 0x0211055c,
	ST_PUNCH_KICK         = 0x02110574,
	
	ST_GROUND_POUND       = 0x021105a4,
	ST_DIVE               = 0x021105bc,
	ST_THROW              = 0x021105d4,
	
	
	
	ST_SLIDE_KICK         = 0x02110634,
	
	
    ST_SWIM               = 0x0211067c,
	ST_WATER_JUMP         = 0x02110694,
	ST_METAL_WATER_GROUND = 0x021106ac,
	ST_METAL_WATER_WATER  = 0x021106c4,
	ST_CLIMB              = 0x021106dc,
	ST_HEADSTAND          = 0x021106f4,
	ST_POLE_JUMP          = 0x0211070c,
	ST_HEADSTAND_JUMP     = 0x02110724,
	
	
	
	
	ST_LAUNCH_STAR        = 0x0211079c
};

#endif