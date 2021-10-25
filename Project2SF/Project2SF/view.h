#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

sf::View view;

View getPlayerCoordinateforview(float x, float y)
{
	view.setCenter(x + 100, y);
	return view;
}