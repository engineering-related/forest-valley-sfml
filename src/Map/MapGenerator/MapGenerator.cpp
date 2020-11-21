#include "MapGenerator.h"

MapGenerator::MapGenerator(unsigned int seed, Vector2i mapDimensions, float noiseScale, int octaves, float persistance, float lacunarity, Vector2f offset, float elevation)
{
	if (mapDimensions.x < 1) mapDimensions.x = 1;
	if (mapDimensions.y < 1) mapDimensions.y = 1;
	if (lacunarity < 1) lacunarity = 1;
	if (octaves < 0) octaves = 0;

	this->seed = seed;
	this->mapDimensions = mapDimensions;
	this->noiseScale = noiseScale;
	this->octaves = octaves;
	this->persistance = persistance;
	this->lacunarity = lacunarity;
	this->offset = offset;
	this->elevation = elevation;
	this->constDraw = false;

	this->initTerrainTypes();
	this->initTexture();
}


MapGenerator::~MapGenerator()
{

}

void MapGenerator::initTexture()
{
	this->texture.clear();
	this->texture.create(this->mapDimensions.x, this->mapDimensions.y);
}

void MapGenerator::initTerrainTypes()
{
	//HeightMap standard
	this->heightRegions.push_back(Terrain(TerrainType::WATER_DEEP, 0.15f, Color(50, 80, 170)));
	this->heightRegions.push_back(Terrain(TerrainType::WATER_SHALLOW, 0.15f, Color(55, 102, 196)));
	this->heightRegions.push_back(Terrain(TerrainType::SAND, 0.22f, Color(208, 208, 128)));
	this->heightRegions.push_back(Terrain(TerrainType::GRASS_LIGHT, 0.60f, Color(85, 151, 24)));
	this->heightRegions.push_back(Terrain(TerrainType::MINERALS, 0.67f, Color(59, 49, 52)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_DARK, 0.75f, Color(76, 59, 58)));
	//Rock light
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT, 0.8f, Color(93, 69, 64)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_2, 0.85f, Color(110, 79, 70)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_3, 0.90f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_4, 0.95f, Color(144, 99, 82)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_5, 1.0f, Color(110, 79, 70)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_6, 1.05f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_7, 1.1f, Color(144, 99, 82)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_8, 1.15f, Color(110, 79, 70)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_9, 1.20f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_10, 1.25f, Color(110, 79, 70)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_11, 1.30f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_12, 1.35f, Color(144, 99, 82)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_13, 1.40f, Color(110, 79, 70)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_14, 1.45f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_15, 1.50f, Color(144, 99, 82)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_16, 1.55f, Color(110, 79, 70)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_17, 1.60f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_18, 1.65f, Color(110, 79, 70)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_19, 1.70f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_20, 1.75f, Color(144, 99, 82)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_21, 1.80f, Color(110, 79, 70)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_22, 1.85f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_23, 1.90f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_24, 1.95f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_25, 2.0f, Color(127, 89, 76)));

	this->heightRegions.push_back(Terrain(TerrainType::SNOW, 2.05f, Color(255, 255, 255)));

	//HeightMap LOTR
	/*this->heightRegions.push_back(Terrain(TerrainType::WATER_DEEP, 0.1f, Color(147, 156, 112)));
	this->heightRegions.push_back(Terrain(TerrainType::WATER_SHALLOW, 0.2f, Color(153, 156, 112)));
	this->heightRegions.push_back(Terrain(TerrainType::SAND, 0.25f, Color(208, 208, 128)));
	this->heightRegions.push_back(Terrain(TerrainType::GRASS_LIGHT, 0.55f, Color(204, 192, 151)));
	this->heightRegions.push_back(Terrain(TerrainType::MINERALS, 0.6f, Color(105, 105, 105)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT, 0.7f, Color(181/1.5, 170/1.5, 132/1.5)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_DARK, 0.95f, Color(153/2, 156/2, 112/2)));
	this->heightRegions.push_back(Terrain(TerrainType::SNOW, 1.0f, Color(255, 255, 255)));*/

	//Forestmap
	this->forestRegions.push_back(Terrain(TerrainType::FOREST_DEEP, 0.3f, Color(0, 40, 0)));
	this->forestRegions[0].setRange(&heightRegions[static_cast<int>(TOrder::SAND)].value, &heightRegions[static_cast<int>(TOrder::MINERALS)].value);
	this->forestRegions.push_back(Terrain(TerrainType::FOREST_SHALLOW, 0.6f, Color(0, 60, 0)));
	this->forestRegions[1].setRange(&heightRegions[static_cast<int>(TOrder::SAND)].value, &heightRegions[static_cast<int>(TOrder::MINERALS)].value);

	//Fieldmap
	this->wheatRegions.push_back(Terrain(TerrainType::WHEAT, 0.10f, Color(208, 176, 132)));
	this->wheatRegions[0].setRange(&heightRegions[static_cast<int>(TOrder::SAND)].value, &heightRegions[static_cast<int>(TOrder::GRASS_LIGHT)].value);
}

void MapGenerator::draw(RenderTarget* window)
{
	if(this->constDraw)
		util::fn::mapSpriteToWindowCoords(window, &this->sprite, this->constDrawPos, this->constDrawScale);

	window->draw(this->sprite);
}


std::vector<std::vector<MapGenerator::TerrainType>> MapGenerator::getMapSegment(const Vector2i &gridPos, const Vector2i &areaSize)
{
	return this->getTerrainTypes(gridPos, areaSize,
		this->generateNoiseMap(this->seed, gridPos, areaSize),
		this->generateNoiseMap(this->seed+1, gridPos, areaSize),
		this->generateNoiseMap(this->seed+2, gridPos, areaSize)
	);
}

void MapGenerator::setConstDraw(const bool& state)
{
	this->constDraw = state;
}

std::vector<std::vector<MapGenerator::TerrainType>> MapGenerator::getTerrainTypes(const Vector2i &gridPos, const Vector2i &areaSize,
	const std::vector<std::vector<float>> heightMap,
	const std::vector<std::vector<float>> forsetMap,
	const std::vector<std::vector<float>> fieldMap
)
{
	std::vector<std::vector<MapGenerator::TerrainType>> terrainVec = std::vector<std::vector<TerrainType>>(heightMap.size(), std::vector<TerrainType>(heightMap[0].size(), TerrainType::WATER_DEEP));

	for (int y = 0; y < areaSize.y; y++)
	{
		for (int x = 0; x < areaSize.x; x++)
		{
			float currentHeight = pow(heightMap[x][y], this->elevation);
			float forestValue = forsetMap[x][y];
			float wheatValue = fieldMap[x][y];
			float boundryValue = MapGenerator::addSquareMask(x + gridPos.x, y + gridPos.y, currentHeight, (this->mapDimensions.x + this->mapDimensions.y) / 2.f, 0.3f, 3.f, true);

			//Add diffent height
			for (auto& heightRegion : this->heightRegions)
			{
				if (currentHeight <= heightRegion.value)
				{
					terrainVec[x][y] = heightRegion.type;
					break;
				}
			}

			//Add forset to the map
			for (auto& forestRegion : this->forestRegions)
			{
				if (forestValue <= forestRegion.value && currentHeight > *forestRegion.startRange && currentHeight < *forestRegion.endRange)
				{
					terrainVec[x][y] = forestRegion.type;
					break;
				}
			}

			//Add wheat to the map
			for (auto& wheatRegion : this->wheatRegions)
			{
				if (wheatValue <= wheatRegion.value && currentHeight > *wheatRegion.startRange && currentHeight < *wheatRegion.endRange)
				{
					if (terrainVec[x][y] == TerrainType::GRASS_LIGHT)
					{
						terrainVec[x][y] = wheatRegion.type;
					}
					break;
				}
			}

			//Add border to the map
			if (boundryValue > this->heightRegions[static_cast<int>(TOrder::SNOW)].value)
			{
				boundryValue = this->heightRegions[static_cast<int>(TOrder::SNOW)].value;
			}
			for (auto& heightRegion : this->heightRegions)
			{
																										//REMOVE THIS TO MAKE ISLAND
				if (boundryValue >= this->heightRegions[static_cast<int>(TOrder::MINERALS)].value && boundryValue <= heightRegion.value)
				{
					terrainVec[x][y] = heightRegion.type;
					break;
				}
			}
		}
	}
	return terrainVec;
}

void MapGenerator::updateTexture(const Vector2i &gridPos,
	const std::vector<std::vector<MapGenerator::TerrainType>>& terrainVec)
{
	RectangleShape cell;
	cell.setOutlineThickness(0.f);
	cell.setSize(Vector2f(1.f, 1.f));

	for (int y = 0; y < terrainVec[0].size(); y++)
	{
		for (int x = 0; x < terrainVec.size(); x++)
		{
			cell.setPosition(static_cast<float>(x) + gridPos.x, static_cast<float>(y) + gridPos.y);
			for (auto& heightRegion : this->heightRegions)
			{
				if (terrainVec[x][y] == heightRegion.type)
					cell.setFillColor(heightRegion.color);
			}
			for (auto& forestRegion : this->forestRegions)
			{
				if (terrainVec[x][y] == forestRegion.type)
					cell.setFillColor(forestRegion.color);
			}
			for (auto& wheatRegion : this->wheatRegions)
			{
				if (terrainVec[x][y] == wheatRegion.type)
					cell.setFillColor(wheatRegion.color);
			}
			this->texture.draw(cell);
		}
	}
	this->texture.display();
	this->sprite.setTexture(this->texture.getTexture());
}

float MapGenerator::addSquareMask(const int& x, const int& y, float noise, float island_size, float max_width_factor, float gradientExp, bool inverse)
{
	float distance_x = fabs(x - island_size * 0.5f);
	float distance_y = fabs(y - island_size * 0.5f);
	float distance = fmax(distance_x, distance_y); // square mask

	float max_width = island_size * max_width_factor;
	float delta = distance / max_width;
	float gradient = pow(delta, gradientExp);

	if (!inverse)
		noise *= fmax(0.0f, 1.f - gradient);
	else
		noise *= fmax(0.0f, gradient);
	return noise;
}

float MapGenerator::addCircleMask(const int& x, const int& y, float noise, float island_size, float max_width_factor, float gradientExp, bool inverse)
{
	float distance_x = fabs(x - island_size * 0.5f);
	float distance_y = fabs(y - island_size * 0.5f);
	float distance = sqrt(distance_x * distance_x + distance_y * distance_y); // circular mask

	float max_width = island_size * max_width_factor;
	float delta = distance / max_width;
	float gradient = pow(delta, gradientExp);

	if (!inverse)
		noise *= fmax(0.0f, 1.f - gradient);
	else
		noise *= fmax(0.0f, gradient);
	return noise;
}

void MapGenerator::setDisplaySize(const Vector2f& size)
{
	Vector2f scale = Vector2f(size.x / this->texture.getSize().x, size.y / this->texture.getSize().y);
	this->constDrawScale = scale;
	this->sprite.setScale(scale);
}

unsigned int MapGenerator::generatePsuedoRandomSeed()
{
	Int64 t = time(NULL);
	unsigned int seed = (t * t) % INT_MAX;
	return seed;
}

std::vector<std::vector<float>> MapGenerator::generateNoiseMap(const int& seed, const Vector2i &gridPos, const Vector2i &areaSize)
{
	std::vector<std::vector<float>> noiseMap(areaSize.x, std::vector<float>(areaSize.y, 0));

	srand(seed);
	PerlinNoise pn(seed);

	std::vector<sf::Vector2f> octaveOffsets;
	for (int i = 0; i < this->octaves; i++)
	{
		float offsetX = util::fn::randFloatFromRange(-1000000.f, 1000000.f) + this->offset.x;
		float offsetY = util::fn::randFloatFromRange(-1000000.f, 1000000.f) + this->offset.y;

		octaveOffsets.push_back(sf::Vector2f(offsetX, offsetY));
	}
	float scale = this->noiseScale;

	if (scale <= 0)
	{
		scale = 0.0001f;
	}

	float maxNoiseHeight = std::numeric_limits<float>::min();
	float minNoiseHeight = std::numeric_limits<float>::max();

	double halfWidth = static_cast<float>(mapDimensions.x / 2.f);
	double halfHeight = static_cast<float>(mapDimensions.y / 2.f);

	for (int y = 0; y < areaSize.y; y++)
	{
		for (int x = 0; x < areaSize.x; x++)
		{
			float amplitude = 1;
			float frequency = 1;
			float noiseHeight = 0;

			for (int i = 0; i < this->octaves; i++)
			{
				float sampleX = (x + gridPos.x - halfWidth) / scale * frequency + octaveOffsets[i].x;
				float sampleY = (y + gridPos.y - halfHeight) / scale * frequency + octaveOffsets[i].y;

				float perlinValue = static_cast<float>(pn.noise(sampleX, sampleY) * 2 - 1);
				noiseHeight += perlinValue * amplitude;
				amplitude *= this->persistance;
				frequency *= this->lacunarity;
			}

			if (noiseHeight > maxNoiseHeight)
			{
				maxNoiseHeight = noiseHeight;
			}
			else if (noiseHeight < minNoiseHeight)
			{
				minNoiseHeight = noiseHeight;
			}
			noiseMap[x][y] = noiseHeight;
		}
	}
	//Normalize noisemap
	for (int y = 0; y < areaSize.y; y++)
	{
		for (int x = 0; x < areaSize.x; x++)
		{
			noiseMap[x][y] = util::fn::map(noiseMap[x][y], -1, 1, 0, 1);
		}
	}
	return noiseMap;
}