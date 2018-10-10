#include "emu.h"
#include "controller.h"
#include "checkbox.h"
#include "textbutton.h"

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
		UpdateEvents();

		window.clear(color::bg3);

		Emu::update();
		Controller::update();
		CheckBox::update();
		TextButton::update();
		
		window.display();
	}

	return 0;
}