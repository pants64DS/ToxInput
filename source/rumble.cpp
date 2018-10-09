#include "controller.h"
#include "ingame.h"

/*inline void text(float x, float y, int num)
{
	sf::Text text(std::to_string(num), main_font);
	text.setCharacterSize(15);
	text.setFillColor(sf::Color::Red);
	text.setPosition(x, y);
	window.draw(text);
}*/

unsigned char GetStateState(unsigned player)
{
	unsigned char stateState;
	ReadProcessMemory(EmuHandle, (void*)(player + (ndsRAMoffset - 0x02000000 + 0x6e3)), &stateState, 1, 0);

	return stateState;
}

void Controller::UpdateRumble()
{
	if (IsGamePaused())
	{
		XINPUT_VIBRATION no_rumble {0, 0};
		XInputSetState(selectedController, &no_rumble);

		return;
	}

	unsigned player0;
	ReadProcessMemory(EmuHandle, (void*)(PLAYER_ARR), &player0, 4, 0);

	unsigned currState;
	ReadProcessMemory(EmuHandle, (void*)(player0 + (ndsRAMoffset - 0x02000000 + 0x370)), &currState, 4, 0);

	static unsigned lastState;

	struct
	{
		unsigned short intensity;
		unsigned short fadeout;
	}
	static rumble;

	unsigned char currJumpNumber = 0xff;
	static unsigned char lastJumpNumber = 0xff;

	int y_speed = 0;
	static int last_y_speed = 0;

	static bool rumbled = false;

	unsigned char health;
	static unsigned char prev_health = 8;
	ReadProcessMemory(EmuHandle, (void*)(ndsRAMoffset + 0x092145), &health, 1, 0);

	switch (currState)
	{
		case ST_CANNON:
		case ST_PUNCH_KICK:

			unsigned short buttonsPressed;
			ReadProcessMemory(EmuHandle, (Input*)(input0 + 2), &buttonsPressed, 2, 0);

			if (buttonsPressed & Input::A)
			{
				if (currState == ST_CANNON)
					rumble = {65535, 1000};
				else
					rumble = {49151, 4000};
			}

			break;

		case ST_CRAZED_CRATE:

			ReadProcessMemory(EmuHandle, (void*)(player0 + (ndsRAMoffset - 0x02000000 + 0x6e1)), &currJumpNumber, 1, 0);

			if (currJumpNumber != lastJumpNumber)
				rumble = {65535, 2500};

			break;

		case ST_GROUND_POUND:

			if (GetStateState(player0) == 0xff && !rumbled)
			{
				rumble = {65535, 3000};
				rumbled = true;
			}

			break;

		case ST_BURN_LAVA:

			ReadProcessMemory(EmuHandle, (void*)(player0 + (ndsRAMoffset - 0x02000000 + 0xa8)), &y_speed, 4, 0);
			
			if (y_speed > 0 && last_y_speed <= 0)
				rumble = {65535, 2700};

			break;

		case ST_STUCK_IN_GROUND:

			static unsigned prev_frame_counter = 0xffffffff;

			unsigned frame_counter;
			ReadProcessMemory(EmuHandle, (void*)(ndsRAMoffset + 0x0a1040), &frame_counter, 4, 0);

			if (currState != lastState)
			{
				prev_frame_counter = frame_counter;
				rumble = {65535, 3000};
			}
			else if (!rumbled)
			{
				unsigned char delay;

				switch (GetStateState(player0))
				{
					case 0:
						delay = 120;
						break;
					case 1:
						delay = 80;
						break;
					case 2:
						delay = 110;
				}

				if (frame_counter > prev_frame_counter + delay)
				{
					rumble = {65535, 3000};
					prev_frame_counter = 0xffffffff;
					rumbled = true;
				}
			}

			break;
			
		default:

			rumbled = false;

			if (currState == lastState) break;

			switch (currState)
			{
				case ST_HURT:
				case ST_HURT_WATER:
				case ST_DEAD_HIT:
				case ST_SQUISH:
				case ST_THROWN:
					rumble = {65535, 3000};
					break;

				case ST_BURN_FIRE:
					rumble = {35000, 0};
					break;

				case ST_DEAD_PIT:
					rumble = {35000, 1000};
					break;

				case ST_ELECTROCUTE:
				case 0x021102bc:
					rumble = {65535, 0};
					break;

				case ST_SLIDE_KICK_RECOVER:
				case ST_THROW:
				case ST_YOSHI_POWER:
				case ST_SWALLOW:
				case 0x0211061c:
					rumble = {49151, 4000};
					break;

				case ST_DIVE:
				case ST_SLIDE_KICK:
				case ST_LONG_JUMP:
				case ST_TURN_AROUND:
					rumble = {32767, 2000};
					break;

				case ST_LAND:
					if (prev_health > health)
					{
						rumble = {65535, 3000};
					}
					break;

				default:
					rumble.intensity = rumble.fadeout = 0;
			}
	}
	
	lastState = currState;
	lastJumpNumber = currJumpNumber;
	last_y_speed = y_speed;
	prev_health = health;

	if (rumble.intensity >= rumble.fadeout)
		rumble.intensity -= rumble.fadeout;
	else
		rumble.intensity = 0;

	XINPUT_VIBRATION xrumble {rumble.intensity, rumble.intensity};

	XInputSetState(selectedController, &xrumble);
}
