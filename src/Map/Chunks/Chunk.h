#ifndef CHUNK
#define CHUNK

#include "Entity/Object.h"

using namespace sf;

class Chunk
{
private:

public:
	Chunk();
	~Chunk();
	RenderTexture texture;
	Sprite sprite;
	std::vector<Object*> dynamicEntites;

	void draw(RenderTarget* window);
	void update(const float &dt, const float &multiplier);

	inline static Vector2i* const size = new Vector2i(50, 50);
};



#endif