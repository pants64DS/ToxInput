#include "gui.h"
#include "emu.h"
#include "checkbox.h"

sf::Font main_font;

int mouse_x, mouse_y;
bool mouseOnWindow;

unsigned char gfxChangeTimer = 30;

sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "ToxInput 0.7", sf::Style::Titlebar | sf::Style::Close);

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
	}
	else if (MouseOnRect(rect) && mouseOnWindow)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			state = (state == ST_MOUSE_OFF || state == ST_INVALID) ? ST_INVALID : ST_CLICKED;
			gfxChangeTimer = 30;
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

	if (gfxChangeTimer > 0) window.draw(rect);
}

void UpdateEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
				window.close();
				break;
			
			case sf::Event::MouseWheelMoved:
				if (Emu::bg_rect.getGlobalBounds().contains(event.mouseWheel.x, event.mouseWheel.y))
				{
					Emu::scrollSpeed = event.mouseWheel.delta * 10;
					Emu::scrollTarget = event.mouseWheel.delta * 30 + Emu::scroll;
				}
				gfxChangeTimer = 30;

				break;
			
			case sf::Event::MouseEntered:
				mouseOnWindow = true;
				break;

			case sf::Event::MouseLeft:
				mouseOnWindow = false;
				break;

			case sf::Event::MouseMoved:
				sf::Vector2i vec = sf::Mouse::getPosition(window);
				mouse_x = vec.x;
				mouse_y = vec.y;
				gfxChangeTimer = 30;
				break;
		}
	}
}