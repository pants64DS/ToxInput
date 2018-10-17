#include "emu.h"
#include "ingame.h"

short Emu::nextID = 0;

int Emu::scroll = 0;
int Emu::scrollSpeed = 0;
int Emu::scrollTarget = 0;

sf::RectangleShape Emu::bg_rect(sf::Vector2f(total_width, total_height));
sf::RectangleShape Emu::scroll_cover_rect1(sf::Vector2f(screen_width - border, starting_y - outline));
sf::RectangleShape Emu::scroll_cover_rect2(sf::Vector2f(screen_width - (border + total_width + outline), total_height + 2 * outline));
sf::RectangleShape Emu::scroll_cover_rect3(sf::Vector2f(screen_width - border, screen_height - (starting_y + total_height + outline)));
sf::RectangleShape Emu::outline_rect(sf::Vector2f(total_width, total_height));

Emu::Emu(HWND _wnd, std::string _name)
{
	id = nextID++;
	rect = sf::RectangleShape(sf::Vector2f(button_width, button_height));

	name = sf::Text(_name, main_font);
	name.setCharacterSize(18);

	wnd = _wnd;
}

Emu::~Emu(){}

void Emu::Update()
{
	Button::Update();

	float y = starting_y + scroll + id * (button_height + outline);
	rect.setPosition(border, y);
	name.setPosition(border + 10, y + 7);

	window.draw(name);

	if (flags & click_1_frame)
	{
		CloseEmu();
		
		selectedEmu = (selectedEmu == id) ? -1 : (EmuHandle = GetProcessHandle(wnd), id);
	}

	if (id == selectedEmu && !EmuHandle)
	{
		EmuHandle = GetProcessHandle(wnd);
		if (!IsGameCompatible(EmuHandle))
		{
			CloseHandle(EmuHandle);
			EmuHandle = NULL;
		}
	}

}

void Emu::OnMouseOn()
{
	if (!MouseOnRect(bg_rect))
	{
		OnMouseOff();
		state = ST_MOUSE_OFF;
		return;
	}

	rect.setFillColor((id == selectedEmu) ? color::dark_selected : color::dead);
	SetTextColor();
}

void Emu::OnMouseOff()
{
	rect.setFillColor(color::bg2);
	SetTextColor();
}

void Emu::OnBeingDead()
{
	rect.setFillColor(color::bg1);
	name.setFillColor(color::dead);
}

void Emu::SetTextColor()
{
	sf::Color color = (id == selectedEmu) ? color::bright_selected : color::whiteish1;
	name.setFillColor(color);
}
