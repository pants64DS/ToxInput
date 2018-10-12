#include "emu.h"
#include "controller.h"
#include "checkbox.h"
#include "ingame.h"

void init()
{
	window.clear(color::bg3);
	window.display();
	window.setFramerateLimit(60);

	sf::Image icon;
	icon.loadFromFile("icon.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	char path [MAX_PATH];
	GetWindowsDirectory(path, MAX_PATH);
	main_font.loadFromFile(std::string(path) + std::string("\\Fonts\\seguisb.ttf"));

	Controller::init();
	Emu::init();
	CheckBox::init();

	for (int i = 0; i < 4; i++)
		Controller::checkIfConnected(i);
}

/*inline void text(float x, float y, long long int num)
{
	sf::Text text(std::to_string(num), main_font);
	text.setCharacterSize(15);
	text.setFillColor(sf::Color::Red);
	text.setPosition(x, y);
	window.draw(text);
}*/

int main()
{
	init();

	while (window.isOpen())
	{
		UpdateEvents();

		window.clear(color::bg3);

		Emu::update();
		Controller::update();
		CheckBox::update();
		
		if (IsGameCompatible(EmuHandle))
		{
			UpdateJoystickInput();
			UpdateRunWithoutY();
			Controller::UpdateRumble();
		}
		else if (EmuHandle != NULL)
		{
			CloseHandle(EmuHandle);
			EmuHandle = NULL;
			Emus[Emu::selectedEmu].flags |= Button::dead;
		}

		// text (100, 200, Emu::selectedEmu);
		// text (100, 220, (long long int)(EmuHandle));

		window.display();
	}

	return 0;
}
