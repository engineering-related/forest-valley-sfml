#ifndef COMPONENT
#define COMPONENT

class Component;
class Object;

//Using ComponentID as size_t
using ComponentID = std::size_t;

inline ComponentID getComponentTypeID()
{
	static ComponentID lastID = 0;
	return lastID++;
}

//noexcept will thorw some types but not others
template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;


class Component
{
public:
	Object* entity;
	virtual void update(const float& dt, const float& multiplier) {};
	virtual void draw(sf::RenderTarget* window) {}
	Component(){
	};
	virtual ~Component(){};
};

#endif