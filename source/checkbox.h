#ifndef CHECK_BOX_INCLUDED
#define CHECK_BOX_INCLUDED

#include "gui.h"
#include "controller.h"

struct CheckBox : public Button
{
	static constexpr float bg1_x = 2 * raw_border + Controller::total_width;
	static constexpr float bg1_y = raw_border + Controller::starting_y;
	static constexpr float bg1_width = screen_width - bg1_x - raw_border;
	static constexpr float bg1_height = screen_height - bg1_y - raw_border;

	static constexpr float box_size = 25;

	static sf::Texture texture;
	static sf::Sprite sprite;
	static sf::RectangleShape bg_rect1;

	static void init();
	static void update();

	sf::Text text;
	sf::RectangleShape box;

	bool isChecked;

	CheckBox(std::string _text, float x, float y, float width, float height, float text_x, float text_y, bool checked);

	virtual ~CheckBox();

	void OnMouseOn() override;
	void OnMouseOff() override;
	void OnBeingDead() override;

	void Update();
	void SetTextColor();
}
extern input_checkbox, y_checkbox;

#endif