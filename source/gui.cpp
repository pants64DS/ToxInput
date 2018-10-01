#include "gui.h"

sf::Font arial;
sf::Font main_font;

int mouse_x, mouse_y;

void UpdateMouse()
{
	sf::Vector2i vec = sf::Mouse::getPosition(window);
	mouse_x = vec.x;
	mouse_y = vec.y;
}

bool MouseOnRect(sf::RectangleShape rect)
{
	return rect.getGlobalBounds().contains(mouse_x, mouse_y);
}

namespace color
{
	sf::Color bg1(15, 15, 15);
	sf::Color bg2(29, 29, 29);
	sf::Color bg3(38, 38, 38);
	sf::Color whiteish1(227, 227, 227);
	sf::Color dead(82, 82, 82);
	sf::Color bright_selected(255, 196, 9);
	sf::Color dark_selected(102, 78, 4);
}

Button::Button()
{
	flags = 0;
}

Button::~Button() {}

void Button::OnMouseOn() {}
void Button::OnMouseOff() {}
void Button::OnBeingDead() {}

void Button::Update()
{
	flags &= ~click_1_frame;
	
	if (flags & dead)
	{
		OnBeingDead();
		return;
	}

	if (MouseOnRect(rect))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{

			state = (state == ST_MOUSE_OFF || state == ST_INVALID) ? ST_INVALID : ST_CLICKED;
		}
		else
		{
			if (state == ST_CLICKED) flags |= click_1_frame;

			state = ST_MOUSE_ON;
		}
		OnMouseOn();
	}
	else
	{
		OnMouseOff();
		state = ST_MOUSE_OFF;
	}

	window.draw(rect);
}