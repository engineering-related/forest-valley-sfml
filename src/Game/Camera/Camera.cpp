#include "Camera.h"
#include "Components/MovementComponent.h"

Camera::Camera(RenderTarget* window)
{
	this->view = window->getView();
}

Camera::~Camera()
{

}

void Camera::zoomViewAt(const sf::Vector2i& pixel, sf::RenderWindow& window, const float& zoom)
{
	const sf::Vector2f beforeCoord { window.mapPixelToCoords(pixel) };
	sf::View view { window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord { window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords { roundf(beforeCoord.x - afterCoord.x), roundf(beforeCoord.y - afterCoord.y) };
	view.move(offsetCoords);
	window.setView(view);
}

void Camera::setView(const Vector2f& targetPos, RenderTarget* window)
{
	View view = window->getView();
	view.setCenter(Vector2f(targetPos.x, targetPos.y));
	window->setView(view);
}

float Camera::roundToNearestPixel(const float &units)
{
	float valueInPixels = units * this->pixelToUnits;
	valueInPixels = roundf(valueInPixels);
	float roundedUnits = valueInPixels * (1 / this->pixelToUnits);
	return roundedUnits;
}

void Camera::updateView(const Vector2f& targetPos, RenderTarget* window, const float& dt, const float& multiplier)
{
	this->view = window->getView();

	Vector2f dist = Vector2f(this->roundToNearestPixel(targetPos.x) - this->roundToNearestPixel(this->view.getCenter().x),
						this->roundToNearestPixel(targetPos.y) - this->roundToNearestPixel(this->view.getCenter().y));

    float roundedX = this->roundToNearestPixel(targetPos.x);
	float roundedY = this->roundToNearestPixel(targetPos.y);

	Vector2f movement(roundedX, roundedY);
	//this->view.setCenter(movement);
	//window->setView(this->view);
	//Vector2f movement = targetPos - view.getCenter();
	//std::cout << movement.x << " " << movement.y << std::endl;
	//view.move(Vector2f(dist.x * dt, dist.y * dt));
	//window->setView(this->view);
}