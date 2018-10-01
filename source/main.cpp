#include "controller.h"
#include "emu.h"
#include "checkbox.h"
#include "textbutton.h"
#include "ingame.h"

sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "ToxInput 0.5", sf::Style::Titlebar | sf::Style::Close);

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
}

int main()
{
	init();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return 0;
			}

			if(event.type == sf::Event::MouseWheelMoved)
			{
				Emu::updateScroll(event.mouseWheel);
			}
		}
		UpdateMouse();

		window.clear(color::bg3);

		Emu::update();
		Controller::update();
		CheckBox::update();
		TextButton::update();

		window.display();
		
		if (input_checkbox.isChecked)
		{
			if (apply_button.flags & Button::click_1_frame && IsGameCompatible(EmuHandle))
			{
				PrepareGameForInput();
			}
			if (!IsCutsceneRunning()) Controller::SendInput();
		}
	}
	return 0;
}