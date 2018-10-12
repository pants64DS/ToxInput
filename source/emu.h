#ifndef EMU_INCLUDED
#define EMU_INCLUDED

#include "gui.h"
#include "controller.h"
#include <windows.h>
#include <vector>
#include <string>

struct Emu : public Button
{
	static constexpr int button_width = Controller::width * 4 + outline * 3;
	static constexpr int button_height = 40;
	static constexpr int starting_y = Controller::total_height + raw_border + border + Controller::starting_y;
	static constexpr int total_width = Controller::total_width - 2 * outline;
	static constexpr int total_height = screen_height - starting_y - border;

	static int scroll;
	static int scrollSpeed;
	static int scrollTarget;

	static short selectedEmu;

	static sf::RectangleShape bg_rect;
	static sf::RectangleShape scroll_cover_rect1;
	static sf::RectangleShape scroll_cover_rect2;
	static sf::RectangleShape scroll_cover_rect3;
	static sf::RectangleShape outline_rect;

	static void init();
	static void update();
	static void refresh();

	Emu(HWND _wnd, std::string _name);
	virtual ~Emu();

	void OnMouseOn() override;
	void OnMouseOff() override;
	void OnBeingDead() override;

	void Update();
	void SetTextColor();

	sf::Text name;
	HWND wnd;
	short id;

	static short nextID;
};

extern std::vector<Emu> Emus;
extern std::vector<HWND> newWindows;

extern unsigned char timer;

#endif