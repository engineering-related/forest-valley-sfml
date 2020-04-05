#ifndef GROUND
#define GROUND

#include "Entity/Tiles/Static/StaticTile.h"

using namespace sf;

class Ground : public StaticTile
{
private:
	Vector2i type;
	float friction;

protected:

public:

	Ground(Vector2f pos, Vector2i type);
	virtual ~Ground();

	virtual void draw(RenderTarget* window) const;
	virtual void update(const float& dt, const float& multiplier);

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

		//Get the correct type of a given ground
		static Parts* getParts(const Vector2i& gridPos);
	};

	//Static defintions
	inline static Parts* const GRASS_FOREST = Parts::getParts(Vector2i(0, 0));
	inline static Parts* const GRASS_FLAT = Parts::getParts(Vector2i(0, 3));
	inline static Parts* const SAND = Parts::getParts(Vector2i(1, 4));
	inline static Parts* const GRAVEL = Parts::getParts(Vector2i(2, 4));
	inline static Parts* const FIELD = Parts::getParts(Vector2i(2, 3));

	//Temp
	inline static Parts* const STONE = Parts::getParts(Vector2i(0, 4));
	inline static Parts* const WATER = Parts::getParts(Vector2i(0, 1));
};


#endif