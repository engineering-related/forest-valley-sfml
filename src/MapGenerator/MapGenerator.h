#include "PerlinNoise.h"
#ifndef MAP_GENERATOR
#define MAP_GENERATOR

using namespace sf;

class MapGenerator
{
private:
	void initTerrainTypes();

public:
	enum class TerrainType {WATER_DEEP, WATER_SHALLOW, SAND, //Height
							GRASS_LIGHT, MINERALS, ROCK_DARK, ROCK_LIGHT,
							ROCK_LIGHT_2, ROCK_LIGHT_3, ROCK_LIGHT_4, SNOW,
							FOREST_DEEP, FOREST_SHALLOW, //Forest
							WHEAT}; //Wheat

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
	std::vector<std::vector<float>> heightMap;
	std::vector<std::vector<float>> forsetMap;
	std::vector<std::vector<float>> fieldMap;

	RenderTexture texture;
	Sprite sprite;

	MapGenerator(unsigned int seed, Vector2i mapDimensions, float noiseScale, int octaves, float persistance, float lacunarity, Vector2f offset, float elevation);
	~MapGenerator();

	void draw(RenderWindow * window);
	void update(/*const float &dt, const float &multiplier*/);
	void updateTexture();
	void setDisplaySize(const Vector2f &size);
	static unsigned int generatePsuedoRandomSeed();
	static float addSquareMask(const int &x, const int &y, float noise, float islad_size, float max_width_factor, float gradientExp, bool inverse);
	static float addCircleMask(const int &x, const int &y, float noise, float islad_size, float max_width_factor, float gradientExp, bool inverse);
	static std::vector<std::vector<float>> generateNoiseMap(const unsigned int & seed, const unsigned int & width, const unsigned int & height, float & scale, const int &octaves, const float &persistance, const float &lacunarity);
};

#endif
