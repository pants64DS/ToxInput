#ifndef CONTROLLER_INCLUDED
#define CONTROLLER_INCLUDED

#include "gui.h"
#include <windows.h>
#include <xinput.h>

struct Controller : public Button
{
	static constexpr float width = 90;
	static constexpr float height = 62;

	static constexpr float total_width = Controller::width * 4 + outline * 5;
	static constexpr float total_height = Controller::height + outline * 2;
	static constexpr float starting_y = 2;

	static sf::Texture texture;
	static sf::Sprite sprite;

	static sf::RectangleShape bg_rect;

	static char selectedController;

	static void init();
	static void update();
	static void checkForConnectedOnes();

	static void SendInput();
	static void UpdateRumble();

	Controller();
	virtual ~Controller();

	void OnMouseOn() override;
	void OnMouseOff() override;
	void OnBeingDead() override;

	void Update();
	void SetSpriteColor();

	char id;
	static char nextID;
}
extern controller_select[4];

#endif