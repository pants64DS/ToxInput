#include "textbutton.h"
#include "checkbox.h"
#include "emu.h"

namespace
{
	constexpr float width = 100;
	constexpr float height = 40;
	constexpr float x = CheckBox::bg1_x + CheckBox::bg1_width / 2 - width / 2;
	constexpr float y = CheckBox::bg1_y + CheckBox::bg1_height - 60;
}

TextButton apply_button("Apply", x, y, width, height, 27, 7, false);

void TextButton::update()
{
	if (Emu::selectedEmu == -1 || !input_checkbox.isChecked)
	{
		apply_button.flags |= dead;
	}
	else
	{
		apply_button.flags &= ~dead;
	}

	apply_button.Update();
}

TextButton::TextButton(std::string _text, float x, float y, float width, float height, float text_x, float text_y, bool dead)
{
	rect = sf::RectangleShape(sf::Vector2f(width, height));
	rect.setPosition(x, y);

	text = sf::Text(_text, main_font);
	text.setCharacterSize(18);
	text.setPosition(x + text_x, y + text_y);

	if (dead) flags |= Button::dead;
}

TextButton::~TextButton() {}

void TextButton::Update()
{
	Button::Update();
	window.draw(text);
}

void TextButton::OnMouseOn()
{
	SetTextColor();
	rect.setFillColor(state == ST_CLICKED ? color::dark_selected : color::dead);
}

void TextButton::OnMouseOff()
{
	SetTextColor();
	rect.setFillColor(color::bg2);
}

void TextButton::OnBeingDead()
{
	rect.setFillColor(color::bg1);
	text.setFillColor(color::dead);
}

void TextButton::SetTextColor()
{
	text.setFillColor(state == ST_CLICKED ? color::bright_selected : color::whiteish1);
}