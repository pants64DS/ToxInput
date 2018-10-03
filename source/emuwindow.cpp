#include "emu.h"
#include "ingame.h"

std::vector<Emu> Emus;
std::vector<HWND> newWindows;

short Emu::selectedEmu = -1;

std::string GetWindowNameAsString(HWND wnd)
{
	if (!IsWindowVisible(wnd)) return "Error :p";
	int length = GetWindowTextLength(wnd);

	if (!length) return "Error :p";
	length++;

	char* buffer = new char[length];
	GetWindowText(wnd, buffer, length);

	std::string bufferString(buffer);
	delete[] buffer;
	return bufferString;
}

BOOL CALLBACK GetWindowHandles(HWND wnd, LPARAM lParam)
{
	std::string name = GetWindowNameAsString(wnd);

	if (name.substr (0, 7) == "DeSmuME" || name.substr (0, 6) == "Paused")
	{	
		newWindows.push_back(wnd);
	}

	return true;
}

void Emu::init()
{
	bg_rect.setFillColor(color::bg1);
	bg_rect.setPosition(border, starting_y);

	outline_rect.setOutlineThickness(outline);
	outline_rect.setOutlineColor(color::bg1);
	outline_rect.setFillColor(sf::Color::Transparent);
	outline_rect.setPosition(border, starting_y);

	scroll_cover_rect1.setPosition(border, 0);
	scroll_cover_rect1.setFillColor(color::bg3);

	scroll_cover_rect2.setPosition(border + total_width + outline, starting_y - outline);
	scroll_cover_rect2.setFillColor(color::bg3);

	scroll_cover_rect3.setPosition(border, starting_y + total_height + outline);
	scroll_cover_rect3.setFillColor(color::bg3);
}

void Emu::refresh()
{
	EnumDesktopWindows(GetThreadDesktop(GetCurrentThreadId()), GetWindowHandles, 0);

	std::vector<Emu> Emus2;

	// add new windows to Emus2
	for (std::vector<HWND>::iterator i = newWindows.begin(); i != newWindows.end(); i++)
	{
		bool isWindowNew = true;
		for (std::vector<Emu>::iterator j = Emus.begin(); j != Emus.end(); j++)
		{
			if (j->wnd == *i) isWindowNew = false;
		}
		if (isWindowNew)
		{
			Emus2.push_back (Emu(*i, GetWindowNameAsString(*i)));
		}
	}

	// copy old windows to Emus2 if they are still open
	for (std::vector<Emu>::iterator i = Emus.begin(); i != Emus.end(); i++)
	{
		bool windowExits = false;
		for (std::vector<HWND>::iterator j = newWindows.begin(); j != newWindows.end(); j++)
		{
			if (i->wnd == *j) windowExits = true;
		}
		if (windowExits)
		{
			Emus2.push_back(*i);
		}
		else if (i->id == selectedEmu)
		{
			selectedEmu = -1;
		}
	}

	Emus = Emus2;

	// rearrange ids and stuff
	for (std::vector<Emu>::iterator i = Emus.begin(); i != Emus.end(); i++)
	{
		short newid = std::distance(Emus.begin(), i);

		if (i->id == selectedEmu) selectedEmu = newid;
		i->id = newid;

		if (!IsGameCompatible2(i->wnd)) i->flags |= dead;
	}

	nextID = Emus.size();

	newWindows.clear();
}

void Emu::update()
{
	static unsigned char timer = 0;
	if (!(++timer &= 127)) refresh();

	scroll = std::min (std::max (total_height - Emus.size() * (button_height + outline) + outline, scrollSpeed + scroll), float(0));

	if (scroll == scrollTarget) scrollSpeed = 0;

	window.draw(bg_rect);

	for (int i = 0; i < Emus.size(); i++)
	{
		Emus[i].Update();
	}

	window.draw(outline_rect);
	window.draw(scroll_cover_rect1);
	window.draw(scroll_cover_rect2);
	window.draw(scroll_cover_rect3);
}