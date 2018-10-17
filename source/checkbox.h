#ifndef CHECK_BOX_INCLUDED
#define CHECK_BOX_INCLUDED

#include "gui.h"
#include "controller.h"

constexpr int screen_width = 620;

void assign_bit (unsigned char* number, unsigned source_bit, unsigned target_bit);

struct CheckBox : public Button
{
	static constexpr float bg1_x = 2 * raw_border + Controller::total_width;
	static constexpr float bg1_y = raw_border + Controller::starting_y;
	static constexpr float bg1_width = screen_width - bg1_x - raw_border;
	static constexpr float bg1_height = 120 + 3 * outline;

	static constexpr float box_size = 25;

	static sf::Texture texture;
	static sf::Sprite sprite;
	static sf::RectangleShape bg_rect1;

	static void init();
	static void update();

	sf::Text text;
	sf::RectangleShape box;

	CheckBox(std::string _text, float x, float y, float width, float height, float text_x, float text_y, unsigned char _flags);

	virtual ~CheckBox();

	void OnMouseOn() override;
	void OnMouseOff() override;
	void OnBeingDead() override;

	void Update();
	void SetTextColor();
}
extern input_checkbox, y_checkbox, camera_checkbox, rumble_checkbox;

constexpr int screen_height = CheckBox::bg1_y + CheckBox::bg1_height + 3 * border + 120 + outline;

#endif