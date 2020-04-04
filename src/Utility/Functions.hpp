#ifndef UTIL_FUNCTIONS
#define UTIL_FUNCTIONS

namespace util
{
	struct fn
	{
		static float distance2i(const sf::Vector2i& a, const sf::Vector2i& b)
		{
			return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
		}
		static float distance2f(const sf::Vector2f &p1, const sf::Vector2f &p2)
		{
			return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
		}

		static void zoomViewAt(const sf::Vector2i &pixel, sf::RenderWindow& window, const float &zoom)
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
		static std::string round2D(float f)
		{
			std::string s(16, '\0');
			auto written = std::snprintf(&s[0], s.size(), "%.2f", f);
			s.resize(written);
			return s;
		}
		static float map(float value,
			float start1, float stop1,
			float start2, float stop2)
		{
			float outgoing =
				start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
			return outgoing;
		}
		static float randFloatFromRange(float LO, float HI)
		{
			float r3 = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI - LO)));
			return r3;
		}
		static void constrain(float& value, const float& minimum, const float& maximum)
		{
			if (value < minimum)
				value = minimum;
			else if (value > maximum)
				value = maximum;
		}
		static bool AABBColision(const sf::RectangleShape& r1, const sf::RectangleShape& r2)
		{
			sf::Vector2f r1Pos = r1.getPosition();
			sf::Vector2f r1Size(r1.getGlobalBounds().width, r1.getGlobalBounds().height);

			sf::Vector2f r2Pos = r2.getPosition();
			sf::Vector2f r2Size(r2.getGlobalBounds().width, r2.getGlobalBounds().height);

			return (r1Pos.x < r2Pos.x + r2Size.x && r1Pos.x + r1Size.x > r2Pos.x && r1Pos.y < r2Pos.y + r2Size.y && r1Pos.y + r1Size.y > r2Pos.y);
		}

		template <class T>
		static void deleteObjInVector(T* obj, std::vector<T*> &vector)
		{
			auto firstToRemove = std::stable_partition(vector.begin(), vector.end(), [obj](T*objPtr) { return objPtr!=obj; });
			std::for_each(firstToRemove, vector.end(), [](T* objPtr) { delete objPtr; });
			vector.erase(firstToRemove, vector.end());
			return;
		}
	};
}

#endif //UTIL_FUNCTIONS