#ifndef UTILS
#define UTILS
#define PI 3.14159265359

//SFML
#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
//#include "SFML/Audio.hpp"
//#include "SFML/Network.hpp"
//Others
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <typeinfo>

struct Utils
{
	static float distance(sf::Vector2f p1, sf::Vector2f p2)
	{
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	}

	static void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
	{
		const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
		sf::View view{ window.getView() };
		view.zoom(zoom);
		window.setView(view);
		const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
		const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
		view.move(offsetCoords);
		window.setView(view);
	}
	static std::string round2D(float f)
	{
		std::string s(16, '\0');
		auto written = std::snprintf(&s[0], s.size(), "%.2f", f);
		s.resize(written);
		return s;
	}
	static float map(float value,
		float start1, float stop1,
		float start2, float stop2) {
		float outgoing =
			start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
		return outgoing;
	}
};

#endif