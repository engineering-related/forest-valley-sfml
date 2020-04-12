
#ifndef TILE
#define TILE

#include "Entity/Object.h"

using namespace sf;

class Tile: public Object
{
private:
	bool solid;
	bool walkable;

protected:

public:
struct Parts
	{
		//OUTER PARTS
		Vector2i OTL, //OUTER TOP LEFT
				 OTM, //OUTER TOP MID
				 OTR, //OUTER TOP RIGHT

				 OML, //OUTER MID LEFT
				 OMM, //OUTER MID MID
				 OMR, //OUTER MID RIGHT

				 OBL, //OUTER BOTTOM LEFT
				 OBM, //OUTER BOTTOM MID
				 OBR; //OUTER BOTTOM RIGHT
		//INNER PARTS
		Vector2i ITL, //INNER TOP LEFT
				 ITR, //INNER TOP RIGHT

				 IML, //INNER MID LEFT
				 IMR; //INNER MID RIGHT
	};
	Tile(Vector2f pos, Texture* texture, bool solid, bool walkable);
	virtual ~Tile();

	//Modifiers
	void changeType(const Vector2i& type);

	virtual void draw(RenderTarget* window) const;
	virtual void update(const float& dt, const float& multiplier);
};


#endif