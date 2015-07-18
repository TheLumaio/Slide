//
// Created by Lumaio on 7/17/2015.
//

#include "Graphics.h"

Graphics::Graphics(sf::RenderWindow &w)
{
	this->window = &w;

	if (_font.loadFromFile("arial.ttf"))
		_text.setFont(_font);
	else
		std::cout << "Error loading \"arial.ttf\"" << std::endl;

	_rect.setSize(sf::Vector2f(0, 0));
	_circle.setRadius(0);
}

void Graphics::ClearColor(sf::Color color)
{
	window->clear(color);
}

void Graphics::Rectangle(float x, float y, float width, float height, sf::Color color)
{
	_rect.setSize(sf::Vector2f(width, height));
	_rect.setPosition(x, y);
	_rect.setFillColor(color);
	window->draw(_rect);
}

void Graphics::Circle(float x, float y, float rad, sf::Color color)
{
	_circle.setRadius(rad);
	_circle.setPosition(x, y);
	_circle.setFillColor(color);
	window->draw(_circle);
}

void Graphics::Print(float x, float y, std::string text, int c_size, sf::Color color)
{
	_text.setString(text);
	_text.setPosition(x, y);
	_text.setCharacterSize(c_size);
	_text.setColor(color);
	window->draw(_text);
}
