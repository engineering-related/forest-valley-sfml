#ifdef UTILS
#define UTILS

class Utils
{
public:
	virtual void show() = 0;

	static float distance(sf::Vector2f p1, sf::Vector2f p2);

	static void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom);

	static std::string round2D(float f);
	static float map(float value,
		float start1, float stop1,
		float start2, float stop2);

	static float randFloatFromRange(float LO, float HI);

	static void constrain(float& value, const float& minimum, const float& maximum);
};


#endif