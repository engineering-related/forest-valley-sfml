#ifndef MAP_GENERATOR
#define MAP_GENERATOR

#include "PerlinNoise.h"

using namespace sf;

class MapGenerator
{
private:
	bool constDraw;
	Vector2f constDrawPos;
	Vector2f constDrawScale;
	void initTexture();
	void initTerrainTypes();
	void initOctaves();

public:						//THE TERRAIN TYPES IN PRIO DRAWING ORDER
	enum class TerrainType {SAND, MINERALS,
							GRASS_LIGHT, FOREST_SHALLOW, FOREST_DEEP, WHEAT,
							WATER_DEEP, WATER_SHALLOW,
							ROCK_DARK, ROCK_LIGHT,
							ROCK_LIGHT_2, ROCK_LIGHT_3, ROCK_LIGHT_4, ROCK_LIGHT_5,
							ROCK_LIGHT_6, ROCK_LIGHT_7, ROCK_LIGHT_8, ROCK_LIGHT_9,
							ROCK_LIGHT_10, ROCK_LIGHT_11, ROCK_LIGHT_12, ROCK_LIGHT_13,
							ROCK_LIGHT_14, ROCK_LIGHT_15, ROCK_LIGHT_16, ROCK_LIGHT_17,
							ROCK_LIGHT_18, ROCK_LIGHT_19, ROCK_LIGHT_20, ROCK_LIGHT_21,
							ROCK_LIGHT_22, ROCK_LIGHT_23, ROCK_LIGHT_24, ROCK_LIGHT_25,
							SNOW
							};

							//THE TERRAIN TYPES IN HEIGHT ORDER
		 enum class TOrder {WATER_DEEP, WATER_SHALLOW, SAND,
							GRASS_LIGHT, MINERALS, ROCK_DARK, ROCK_LIGHT,
							ROCK_LIGHT_2, ROCK_LIGHT_3, ROCK_LIGHT_4, ROCK_LIGHT_5,
							ROCK_LIGHT_6, ROCK_LIGHT_7, ROCK_LIGHT_8, ROCK_LIGHT_9,
							ROCK_LIGHT_10, ROCK_LIGHT_11, ROCK_LIGHT_12, ROCK_LIGHT_13,
							ROCK_LIGHT_14, ROCK_LIGHT_15, ROCK_LIGHT_16, ROCK_LIGHT_17,
							ROCK_LIGHT_18, ROCK_LIGHT_19, ROCK_LIGHT_20, ROCK_LIGHT_21,
							ROCK_LIGHT_22, ROCK_LIGHT_23, ROCK_LIGHT_24, ROCK_LIGHT_25,
							SNOW,
							FOREST_SHALLOW, FOREST_DEEP,
							WHEAT
							};
	struct Terrain {
		TerrainType type;
		float value;
		Color color;
		float* startRange = nullptr;
		float * endRange = nullptr;
		Terrain(TerrainType type, float value, Color color)
		{
			this->type = type;
			this->value = value;
			this->color = color;
		}
		inline void setRange(float * start, float * end) { this->startRange = start, this->endRange = end; }
	};

	std::vector<Terrain> heightRegions;
	std::vector<Terrain> forestRegions;
	std::vector<Terrain> wheatRegions;

	int seed;
	Vector2i mapDimensions;
	float noiseScale;
	int octaves;
	float persistance;
	float lacunarity;
	Vector2f offset;
	float elevation;

	RenderTexture texture;
	Sprite sprite;

	MapGenerator(unsigned int seed, Vector2i mapDimensions, float noiseScale, int octaves, float persistance, float lacunarity, Vector2f offset, float elevation);
	~MapGenerator();

	void draw(RenderTarget * window);
	std::vector<std::vector<TerrainType>> getMapSegment(const Vector2i &gridPos, const Vector2i &areaSize);

	//Set the map to a constant pos/scale when drawn
	void setConstDraw(const bool& state);

	std::vector<std::vector<TerrainType>> getTerrainTypes(const Vector2i &gridPos, const Vector2i &areaSize,
		const std::vector<std::vector<float>> heightMap,
		const std::vector<std::vector<float>> forsetMap,
		const std::vector<std::vector<float>> fieldMap
	);

	void updateTexture(const Vector2i &gridPos,
		const std::vector<std::vector<MapGenerator::TerrainType>>& terrainVec);
	void setDisplaySize(const Vector2f &size);

	static unsigned int generatePsuedoRandomSeed();
	static float addSquareMask(const int &x, const int &y, float noise, float islad_size, float max_width_factor, float gradientExp, bool inverse);
	static float addCircleMask(const int &x, const int &y, float noise, float islad_size, float max_width_factor, float gradientExp, bool inverse);
	std::vector<std::vector<float>> generateNoiseMap(const int& seed, const Vector2i &gridPos, const Vector2i &areaSize);
};

#endif
