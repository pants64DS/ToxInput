#include "controller.h"

sf::Texture Controller::texture;
sf::Sprite Controller::sprite;

sf::RectangleShape Controller::bg_rect(sf::Vector2f(Controller::total_width, Controller::total_height));

char Controller::nextID = 0;

Controller::Controller()
{
	rect = sf::RectangleShape(sf::Vector2f(width, height));

	id = nextID++;

	rect.setPosition(id * (width + outline) + border, border + starting_y);
}

Controller::~Controller(){}

void Controller::Update()
{
	Button::Update();
	sprite.setPosition(rect.getPosition() + sf::Vector2f(width/2, height/2));
	if (gfxChangeTimer > 0) window.draw(sprite);

	if (flags & click_1_frame)
		selectedController = (selectedController == id) ? -1 : id;
}

void Controller::OnMouseOn()
{
	rect.setFillColor((id == Controller::selectedController) ? color::dark_selected : color::dead);
	SetSpriteColor();
}

void Controller::OnMouseOff()
{
	rect.setFillColor(color::bg2);
	SetSpriteColor();
}

void Controller::OnBeingDead()
{
	sprite.setColor(color::dead);
	rect.setFillColor(color::bg1);
}

void Controller::SetSpriteColor()
{
	sf::Color color = (id == Controller::selectedController) ? color::bright_selected : sf::Color::White;
	sprite.setColor(color);
}