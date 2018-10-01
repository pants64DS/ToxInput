#ifndef TEXT_BUTTON_INCLUDED
#define TEXT_BUTTON_INCLUDED

#include "gui.h"

struct TextButton : public Button
{
	static void update();

	sf::Text text;

	TextButton(std::string _text, float x, float y, float width, float height, float text_x, float text_y, bool dead);

	virtual ~TextButton();

	void OnMouseOn() override;
	void OnMouseOff() override;
	void OnBeingDead() override;

	void Update();
	void SetTextColor();
}
extern apply_button;

#endif