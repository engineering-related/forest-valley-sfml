#ifndef UTIL_FUNCTIONS
#define UTIL_FUNCTIONS

#define ENUM_TO_STR(ENUM) std::string(#ENUM)

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

		static void mapSpriteToWindowCoords(sf::RenderTarget* window, sf::Sprite* sprite, const sf::Vector2f& constPos, const sf::Vector2f& constSize)
		{
			//Sprite
			sprite->setPosition(sf::Vector2f(window->mapPixelToCoords(sf::Vector2i(constPos))));
			sprite->setScale(constSize.x / window->getDefaultView().getSize().x * window->getView().getSize().x,
				constSize.y / window->getDefaultView().getSize().y * window->getView().getSize().y);
		}

		static std::chrono::milliseconds getTimeInMsSinceEpoch()
		{
			std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
    			std::chrono::system_clock::now().time_since_epoch()
			);
			return ms;
		}

		static std::vector<std::string> stringSplitSpace(std::string s)
		{
			std::stringstream ss(s);
			std::istream_iterator<std::string> begin(ss);
			std::istream_iterator<std::string> end;
			std::vector<std::string> vstrings(begin, end);
			return vstrings;
		}

		static std::string random_string(std::size_t length)
		{
			const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

			std::random_device random_device;
			std::mt19937 generator(random_device());
			std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

			std::string random_string;

			for (std::size_t i = 0; i < length; ++i)
			{
				random_string += CHARACTERS[distribution(generator)];
			}

			return random_string;
		}

		template <class InputIt, class T = typename std::iterator_traits<InputIt>::value_type>
		static T most_common(InputIt begin, InputIt end)
		{
			std::map<T, int> counts;
			for (InputIt it = begin; it != end; ++it)
			{
				if (counts.find(*it) != counts.end())
				{
					++counts[*it];
				}
				else
				{
					counts[*it] = 1;
				}
			}
			return std::max_element(counts.begin(), counts.end(), [](const std::pair<T, int>& pair1, const std::pair<T, int>& pair2) { return pair1.second < pair2.second; })->first;
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