#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "Components/AllComponents.hpp"

using namespace sf;

class Object
{
private:
	void initComponents();

protected:
	//Data
	Sprite sprite;
	Texture* texture;

	//Components
	std::vector<std::unique_ptr<Component>> components;
	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

public:
	//Constructors
	Object(Vector2f pos, Texture* texture);
	virtual ~Object();

	//Components
	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}
	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		//c->init();
		return *c;
	}
	template <typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
	//Accessors

	//Modifiers

	//Functions
	virtual void draw(RenderTarget* window) const = 0;
	virtual void update(const float& dt, const float& multiplier) = 0;
};

#endif //GAME_OBJECT