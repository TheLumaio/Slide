//
// Created by Lumaio on 7/17/2015.
//

#ifndef PONG_GRAPHICS_H
#define PONG_GRAPHICS_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Graphics {
private:
	sf::RenderWindow *window;
	sf::RectangleShape _rect;
	sf::CircleShape _circle;

	sf::Font _font;
	sf::Text _text;
public:
	Graphics(sf::RenderWindow &window);

	void ClearColor(sf::Color color);

	void Rectangle(float x, float y, float width, float height, sf::Color color = sf::Color::White);
	void Circle(float x, float y, float rad, sf::Color color = sf::Color::White);
	void Print(float x, float y, std::string text, int c_size = 18, sf::Color color = sf::Color::White);

};


#endif //PONG_GRAPHICS_H
