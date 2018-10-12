#include "controller.h"
#include "ingame.h"
#include "checkbox.h"
#include "emu.h"

Controller controller_select[4] =
{
	Controller(),
	Controller(),
	Controller(),
	Controller()
};

char Controller::selectedController = -1;

void Controller::init()
{
	texture.loadFromFile("controller.png");
	sprite.setTexture(texture);

	sf::FloatRect bounds = sprite.getGlobalBounds();
	sprite.setOrigin(int(bounds.width / 2), int(bounds.height / 2));

	bg_rect.setPosition(raw_border, raw_border + starting_y);
	bg_rect.setFillColor(color::bg1);
}

void Controller::update()
{
	window.draw(bg_rect);

	if (timer < 13 && timer % 4 == 0)
		checkIfConnected(timer >> 2);
	
	for (int i = 0; i < 4; i++)
		controller_select[i].Update();
}

void Controller::checkIfConnected(unsigned char i)
{
	_XINPUT_STATE state;
	ZeroMemory(&state, sizeof(_XINPUT_STATE));
	
	if (XInputGetState(i, &state) != ERROR_SUCCESS)
		controller_select[i].flags |= Button::dead;
}
