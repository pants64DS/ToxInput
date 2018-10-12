#include "checkbox.h"
#include "emu.h"
#include "ingame.h"

CheckBox input_checkbox
(
	"Joystick input",
	CheckBox::bg1_x + outline,
	CheckBox::bg1_y + outline,
	CheckBox::bg1_width - 2 * outline, 60,
	62, 16,
	true
);

CheckBox y_checkbox
(
	"Run without\nholding Y",
	CheckBox::bg1_x + outline,
	CheckBox::bg1_y + 2 * outline + 60,
	CheckBox::bg1_width - 2 * outline, 60,
	65, 4,
	true
);

CheckBox rumble_checkbox
(
	"Rumble",
	CheckBox::bg1_x + outline,
	screen_height - 60 - border,
	CheckBox::bg1_width - 2 * outline, 60,
	62, 16,
	true
);

sf::RectangleShape CheckBox::bg_rect1(sf::Vector2f(CheckBox::bg1_width, CheckBox::bg1_height));

sf::Texture CheckBox::texture;
sf::Sprite CheckBox::sprite;

void CheckBox::init()
{
	bg_rect1.setPosition(bg1_x, bg1_y);
	bg_rect1.setFillColor(color::bg1);

	sf::RectangleShape check1(sf::Vector2f(11, 3));
	sf::RectangleShape check2(sf::Vector2f(3, 20));
	sf::Vector2f pos(7, 19);
	check1.setPosition(pos);
	check2.setPosition(pos);
	check1.rotate(-125);
	check2.rotate(-125);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 9;
	sf::RenderTexture render_texture;
	render_texture.create(25, 25, settings);
	render_texture.draw(check1);
	render_texture.draw(check2);
	render_texture.display();

	texture = render_texture.getTexture();
	sprite.setTexture(texture);

	rumble_checkbox.rect.setOutlineThickness(outline);
	rumble_checkbox.rect.setOutlineColor(color::bg1);
}

void CheckBox::update()
{
	if (EmuHandle == NULL)
	{
		input_checkbox.flags |= Button::dead;
		rumble_checkbox.flags |= Button::dead;
	}
	else
	{
		input_checkbox.flags &= ~Button::dead;
		rumble_checkbox.flags &= ~Button::dead;
	}

	static bool b = false;
	if (input_checkbox.flags & Button::dead || !input_checkbox.isChecked)
	{
		y_checkbox.flags |= Button::dead;
		if (y_checkbox.isChecked) b = true;
		y_checkbox.isChecked = false;
	}
	else
	{
		y_checkbox.flags &= ~Button::dead;

		if (b)
		{
			y_checkbox.isChecked = true;
			b = false;
		}
	}

	window.draw(bg_rect1);

	input_checkbox.Update();
	rumble_checkbox.Update();
	y_checkbox.Update();
}

CheckBox::CheckBox(std::string _text, float x, float y, float width, float height, float text_x, float text_y, bool checked)
{
	rect = sf::RectangleShape(sf::Vector2f(width, height));
	rect.setPosition(x, y);
	rect.setFillColor(color::bg2);

	box = sf::RectangleShape(sf::Vector2f(box_size, box_size));
	float box_y = (height - box_size) / 2;
	box.setPosition((int)(x + box_y), (int)(y + box_y));
	box.setOutlineThickness(3);

	text = sf::Text(_text, main_font);
	text.setCharacterSize(18);
	text.setPosition(x + text_x, y + text_y);

	isChecked = checked;
}

CheckBox::~CheckBox(){}

void CheckBox::Update()
{
	if (flags & click_1_frame) isChecked = !isChecked;

	Button::Update();
	window.draw(box);
	window.draw(text);

	if (isChecked)
	{
		sprite.setPosition(box.getPosition());
		window.draw(sprite);
	}
}

void CheckBox::OnMouseOn()
{
	sf::Color color = isChecked ? color::dark_selected : color::dead;
	box.setFillColor(color);
	rect.setFillColor(color);
	SetTextColor();
}

void CheckBox::OnMouseOff()
{
	box.setFillColor(color::bg2);
	rect.setFillColor(color::bg2);
	SetTextColor();
}

void CheckBox::OnBeingDead()
{
	text.setFillColor(color::dead);
	box.setFillColor(color::bg1);
	rect.setFillColor(color::bg1);
	box.setOutlineColor(color::dead);
	sprite.setColor(color::dead);
}

void CheckBox::SetTextColor()
{
	sf::Color color = isChecked ? color::bright_selected : color::whiteish1;
	text.setFillColor(color);
	box.setOutlineColor(color);
	sprite.setColor(color::bright_selected);
}