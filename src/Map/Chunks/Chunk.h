#ifndef CHUNK
#define CHUNK

#include "Entity/Object.h"

using namespace sf;

class Chunk
{
private:
	void init();

public:
	Vector2f pos;
	RenderTexture renderTexture;
	Sprite sprite;
	std::vector<Object*> dynamicEntites;

	Chunk(Vector2f pos);
	~Chunk();

	void draw(RenderTarget* window);
	void update(const float &dt, const float &multiplier);
	void setTexture();

	inline static Vector2i* const size = new Vector2i(50, 50);
};



#endif