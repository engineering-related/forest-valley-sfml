#ifndef CAMERA
#define CAMERA

using namespace sf;

class Camera
{
private:
	float pixelToUnits = 32;
	View view;
	float zoom = 1/1.5f;

public:
	Camera(RenderTarget* window);
	~Camera();
	static void zoomViewAt(const sf::Vector2i& pixel, sf::RenderWindow& window, const float& zoom);
	void updateView(const Vector2f& targetPos, RenderTarget* window, const float& dt, const float& multiplier);
	//Ensure bounds
	float roundToNearestPixel(const float& units);
	void setView(const Vector2f& targetPos, RenderTarget* window);
};




#endif