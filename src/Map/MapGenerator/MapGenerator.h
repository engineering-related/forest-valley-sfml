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
	void initTerrainTypes();

public:
	enum class TerrainType {WATER_DEEP, WATER_SHALLOW, SAND,
							GRASS_LIGHT, WHEAT, FOREST_SHALLOW, FOREST_DEEP,
							MINERALS, ROCK_DARK, ROCK_LIGHT,
							ROCK_LIGHT_2, ROCK_LIGHT_3, ROCK_LIGHT_4, SNOW
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

	std::vector<std::vector<TerrainType>> terrainVec;
	std::vector<std::vector<TerrainType>> terrainVec2xScale;

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

	void draw(RenderTarget * window);
	void update(/*const float &dt, const float &multiplier*/);
	//Set the map to a constant pos/scale when drawn
	void setConstDraw(const bool& state);
	void setTerrainTypes();
	std::vector<TerrainType> getNeighbours(const int& x, const int& y);
	void checkRemoveSingle(const int& x, const int& y, std::vector<std::vector<bool>>& changedStates, const bool&root);
	void removeSingles();
	void updateTexture();
	void setDisplaySize(const Vector2f &size);

	static unsigned int generatePsuedoRandomSeed();
	static float addSquareMask(const int &x, const int &y, float noise, float islad_size, float max_width_factor, float gradientExp, bool inverse);
	static float addCircleMask(const int &x, const int &y, float noise, float islad_size, float max_width_factor, float gradientExp, bool inverse);
	static std::vector<std::vector<float>> generateNoiseMap(const unsigned int& seed, const unsigned int& width, const unsigned int& height, float& scale, const int& octaves, const float& persistance, const float& lacunarity, const sf::Vector2f& offset);
};

#endif
