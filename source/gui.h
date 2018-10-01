#ifndef GUI_INCLUDED
#define GUI_INCLUDED

#include "SFML/Graphics.hpp"

extern sf::RenderWindow window;
extern sf::Font main_font;

extern int mouse_x, mouse_y;

void UpdateMouse();
bool MouseOnRect(sf::RectangleShape rect);

constexpr float outline = 5;
constexpr float raw_border = 8;
constexpr float border = raw_border + outline;

constexpr float screen_width = 620;
constexpr float screen_height = 310;

struct Button
{
	sf::RectangleShape rect;

	enum State : unsigned char
	{
		ST_MOUSE_ON,
		ST_MOUSE_OFF,
		ST_CLICKED,
		ST_INVALID,
	};
	unsigned char state;

	enum Flags : unsigned char
	{
		dead = 1 << 0,
		click_1_frame = 1 << 1
	};
	unsigned char flags;

	Button();
	virtual ~Button();

	virtual void OnMouseOn();
	virtual void OnMouseOff();
	virtual void OnBeingDead();

	void Update();
};

namespace color
{
	extern sf::Color bg1;
	extern sf::Color bg2;
	extern sf::Color bg3;
	extern sf::Color whiteish1;
	extern sf::Color dead;
	extern sf::Color bright_selected;
	extern sf::Color dark_selected;
}

#endif