#include "Utils.h"

struct Utils
{
	static float distance(sf::Vector2f p1, sf::Vector2f p2);
	static void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom);
	static std::string round2D(float f);
	static float map(float value,
		float start1, float stop1,
		float start2, float stop2);
	static float randFloatFromRange(float LO, float HI);
	static void constrain(float& value, const float& minimum, const float& maximum);
};

float Utils::distance(sf::Vector2f p1, sf::Vector2f p2)
{
	return static_cast<float>(sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)));
}

void Utils::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord { window.mapPixelToCoords(pixel) };
	sf::View view { window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord { window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords { beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}
std::string Utils::round2D(float f)
{
	std::string s(16, '\0');
	auto written = std::snprintf(&s[0], s.size(), "%.2f", f);
	s.resize(written);
	return s;
}
float Utils::map(float value,
	float start1, float stop1,
	float start2, float stop2)
{
	float outgoing =
		start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	return outgoing;
}
float Utils::randFloatFromRange(float LO, float HI)
{
	float r3 = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
	return r3;
}
void Utils::constrain(float& value, const float& minimum, const float& maximum)
{
	if (value < minimum)
		value = minimum;
	else if (value > maximum)
		value = maximum;
}