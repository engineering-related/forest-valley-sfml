#ifndef UTILITES
#define UTILITIES

namespace util
{
	float distance(sf::Vector2f p1, sf::Vector2f p2);

	void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom);

	std::string round2D(float f);

	float map(float value,
		float start1, float stop1,
		float start2, float stop2);

	float randFloatFromRange(float LO, float HI);

	void constrain(float &value, const float& minimum, const float &maximum);
}

#endif