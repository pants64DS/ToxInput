#include "controller.h"
#include "ingame.h"
#include "checkbox.h"
#include <math.h>

#define pi 3.1415926535897932384626433832795

void Controller::SendInput()
{
	if (selectedController == -1 || !EmuHandle) return;

	// Input* input0 = (Input*)(input0_addr);
	Input* input1 = (Input*)(input1_addr);

	Input myInput;

	_XINPUT_STATE state;
	ZeroMemory(&state, sizeof(_XINPUT_STATE));

	if (XInputGetState(selectedController, &state) != ERROR_SUCCESS) return;

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

	myInput.buttonsHeld = 0;

	if (camera_checkbox.isChecked)
	{
		if (state.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			myInput.buttonsHeld = Input::CAM_LEFT;
		}
		else if (state.Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
		{
			myInput.buttonsHeld = Input::CAM_RIGHT;
		}
	}

	WriteProcessMemory(EmuHandle, input1, &myInput, sizeof(Input), 0);
}