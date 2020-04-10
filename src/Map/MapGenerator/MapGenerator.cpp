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
	this->update();
}


MapGenerator::~MapGenerator()
{

}

void MapGenerator::initTerrainTypes()
{
	//Init terrainVec
	this->terrainVec = std::vector<std::vector<TerrainType>>(this->mapDimensions.x, std::vector<TerrainType>(this->mapDimensions.y, TerrainType::WATER_DEEP));
	this->terrainVec2xScale = std::vector<std::vector<TerrainType>>(2*this->mapDimensions.x, std::vector<TerrainType>(2*this->mapDimensions.y, TerrainType::WATER_DEEP));

	//HeightMap standard
	this->heightRegions.push_back(Terrain(TerrainType::WATER_DEEP, 0.2f, Color(50, 80, 170)));
	this->heightRegions.push_back(Terrain(TerrainType::WATER_SHALLOW, 0.2f, Color(55, 102, 196)));
	this->heightRegions.push_back(Terrain(TerrainType::SAND, 0.25f, Color(208, 208, 128)));
	this->heightRegions.push_back(Terrain(TerrainType::GRASS_LIGHT, 0.65f, Color(85, 151, 24)));
	this->heightRegions.push_back(Terrain(TerrainType::MINERALS, 0.7f, Color(59, 49, 52)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_DARK, 0.8f, Color(76, 59, 58)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT, 0.9f, Color(93, 69, 64)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_2, 1.0f, Color(110, 79, 70)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_3, 2.0f, Color(127, 89, 76)));
	this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT_4, 4.0f, Color(144, 99, 82)));
	this->heightRegions.push_back(Terrain(TerrainType::SNOW, 6.0f, Color(255, 255, 255)));

	////HeightMap LOTR
	//this->heightRegions.push_back(Terrain(TerrainType::WATER_DEEP, 0.1f, Color(147, 156, 112)));
	//this->heightRegions.push_back(Terrain(TerrainType::WATER_SHALLOW, 0.2f, Color(153, 156, 112)));
	////this->heightRegions.push_back(Terrain(TerrainType::SAND, 0.25f, Color(208, 208, 128)));
	//this->heightRegions.push_back(Terrain(TerrainType::GRASS_LIGHT, 0.55f, Color(204, 192, 151)));
	////this->heightRegions.push_back(Terrain(TerrainType::MINERALS, 0.6f, Color(105, 105, 105)));
	//this->heightRegions.push_back(Terrain(TerrainType::ROCK_LIGHT, 0.7f, Color(181/1.5, 170/1.5, 132/1.5)));
	//this->heightRegions.push_back(Terrain(TerrainType::ROCK_DARK, 1.f, Color(153/2, 156/2, 112/2)));
	////this->heightRegions.push_back(Terrain(TerrainType::SNOW, 0.95f, Color(255, 255, 255)));

	//Forestmap
	this->forestRegions.push_back(Terrain(TerrainType::FOREST_DEEP, 0.20f, Color(0, 40, 0)));
	this->forestRegions[0].setRange(&heightRegions[static_cast<int>(TOrder::SAND)].value, &heightRegions[static_cast<int>(TOrder::MINERALS)].value);
	this->forestRegions.push_back(Terrain(TerrainType::FOREST_SHALLOW, 0.5f, Color(0, 60, 0)));
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

void MapGenerator::update(/*const float & dt, const float & multiplier*/)
{
	this->heightMap = MapGenerator::generateNoiseMap(this->seed, this->mapDimensions.x, this->mapDimensions.y, this->noiseScale, this->octaves, this->persistance, this->lacunarity, this->offset);
	this->forsetMap = MapGenerator::generateNoiseMap(this->seed + 1, this->mapDimensions.x, this->mapDimensions.y, this->noiseScale, this->octaves, this->persistance, this->lacunarity, this->offset);
	this->fieldMap = MapGenerator::generateNoiseMap(this->seed + 2, this->mapDimensions.x, this->mapDimensions.y, this->noiseScale, this->octaves, this->persistance, this->lacunarity, this->offset);
	this->setTerrainTypes();
	//this->removeSingles();
	this->updateTexture();
}

void MapGenerator::setConstDraw(const bool& state)
{
	this->constDraw = state;
}

void MapGenerator::setTerrainTypes()
{
	for (int y = 0; y < this->mapDimensions.y; y++)
	{
		for (int x = 0; x < this->mapDimensions.x; x++)
		{
			float currentHeight = pow(this->heightMap[x][y], this->elevation);
			float forestValue = this->forsetMap[x][y];
			float wheatValue = this->fieldMap[x][y];
			float boundryValue = MapGenerator::addSquareMask(x, y, currentHeight, (this->mapDimensions.x + this->mapDimensions.y) / 2.f, 0.3f, 4.f, true);

			//Add diffent height
			for (auto& heightRegion : this->heightRegions)
			{
				if (currentHeight <= heightRegion.value)
				{
					this->terrainVec[x][y] = heightRegion.type;
					for (int row = 0; row <= 1; row++)
					{
						for (int col = 0; col <= 1; col++)
						{
							this->terrainVec2xScale[2 * x + row][2 * y + col] = heightRegion.type;
						}
					}
					break;
				}
			}

			//Add forset to the map
			for (auto& forestRegion : this->forestRegions)
			{
				if (forestValue <= forestRegion.value && currentHeight > *forestRegion.startRange && currentHeight < *forestRegion.endRange)
				{
					this->terrainVec[x][y] = forestRegion.type;
					for (int row = 0; row <= 1; row++)
					{
						for (int col = 0; col <= 1; col++)
						{
							this->terrainVec2xScale[2 * x + row][2 * y + col] = forestRegion.type;
						}
					}
					break;
				}
			}

			//Add wheat to the map
			for (auto& wheatRegion : this->wheatRegions)
			{
				if (wheatValue <= wheatRegion.value && currentHeight > *wheatRegion.startRange && currentHeight < *wheatRegion.endRange)
				{
					if (this->terrainVec[x][y] == TerrainType::GRASS_LIGHT)
					{
						this->terrainVec[x][y] = wheatRegion.type;
						for (int row = 0; row <= 1; row++)
						{
							for (int col = 0; col <= 1; col++)
							{
								this->terrainVec2xScale[2 * x + row][2 * y + col] = wheatRegion.type;
							}
						}
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
				if (boundryValue >= this->heightRegions[static_cast<int>(TOrder::MINERALS)].value && boundryValue <= heightRegion.value)
				{
					this->terrainVec[x][y] = heightRegion.type;
					for (int row = 0; row <= 1; row++)
					{
						for (int col = 0; col <= 1; col++)
						{
							this->terrainVec2xScale[2 * x + row][2 * y + col] = heightRegion.type;
						}
					}
					break;
				}
			}
		}
	}
}

///////////////DONT LOOK, SEVERE AUTISM!!///////////
std::vector<MapGenerator::TerrainType> MapGenerator::getNeighbours(const int& x, const int& y)
{
	std::vector<TerrainType> neighbours;
	//TODO: Refactor with for loop
	TerrainType *top = &this->terrainVec[x][y-1],
				*left = &this->terrainVec[x-1][y],
				*right = &this->terrainVec[x+1][y],
				*bottom = &this->terrainVec[x][y+1],

				*tl = &this->terrainVec[x-1][y-1],
				*tr = &this->terrainVec[x+1][y-1],
				*bl = &this->terrainVec[x-1][y+1],
				*br = &this->terrainVec[x+1][y+1];

	neighbours.push_back(*top);
	neighbours.push_back(*left);
	neighbours.push_back(*right);
	neighbours.push_back(*bottom);

	neighbours.push_back(*tl);
	neighbours.push_back(*tr);
	neighbours.push_back(*bl);
	neighbours.push_back(*br);
	return neighbours;
}

void MapGenerator::checkRemoveSingle(const int& x, const int& y, std::vector<std::vector<bool>>& changedStates, const bool& root)
{
	if(!root && changedStates[x][y]) return;
	else if ( x == 0 || y == 0 || x == this->mapDimensions.x - 1 || y == this->mapDimensions.y - 1) return; // dont check outer cells

	TerrainType *current = &this->terrainVec[x][y], //Middle cell
		*top = &this->terrainVec[x][y - 1],
				*left = &this->terrainVec[x - 1][y],
				*right = &this->terrainVec[x + 1][y],
				*bottom = &this->terrainVec[x][y + 1],

				*tl = &this->terrainVec[x - 1][y - 1],
				*tr = &this->terrainVec[x + 1][y - 1],
				*bl = &this->terrainVec[x - 1][y + 1],
				*br = &this->terrainVec[x + 1][y + 1];

	/*							   xx
    Check for the specific type:   xxx   (with any rotation)
									xx
								*/

	if ((*top == *current || *bottom == *current) &&
	(*left == *current || *right == *current) &&
	((*tl == *current || *br == *current) || (*tr == *current || *bl == *current))) return; //No singles found
	else
	{
		std::vector<TerrainType> neighbours = this->getNeighbours(x, y);
		unsigned int count = 0;
		while (!((*top == *current || *bottom == *current) &&
			   (*left == *current || *right == *current) &&
			   ((*tl == *current || *br == *current) || (*tr == *current || *bl == *current))))
		{
			//If none of the sourrounding cells is viable, change to most common type of the neighbours
			//Change the adjecent cells to that type aswell
			if(count > neighbours.size() - 1) {
				*current = util::fn::most_common(neighbours.begin(), neighbours.end());
				*top = *current;
				*right = *current;
				*left = *current;
				*bottom = *current;
				break;
			};
			*current = neighbours[count];
			count++;
		}
		changedStates[x][y] = true;
		//Do the same for the adjecent cells
		checkRemoveSingle(x, y - 1, changedStates, false);
		checkRemoveSingle(x - 1, y, changedStates, false);
		checkRemoveSingle(x + 1, y, changedStates, false);
		checkRemoveSingle(x, y + 1, changedStates, false);
	}
}

void MapGenerator::removeSingles()
{
	/*Algorithm:
	c->changed = false
	-for cell c in cells:
		if(c->changed) continue
		-check the 4 sourronding cells (neighbours) if they are they same type
		-if not:
			change c
			c->changed = true
			-do the same check for the neighbours
	*/
	//Needs to be run 4 times to ensure all 4 sides after shifting
	for (int i = 0; i < 4; i++)
	{
		std::vector<std::vector<bool>> changedStates = std::vector<std::vector<bool>>(this->mapDimensions.x, std::vector<bool>(this->mapDimensions.y, false));
		for (int y = 0; y < this->mapDimensions.y; y++)
		{
			for (int x = 0; x < this->mapDimensions.x; x++)
			{
				//Recursive function, does the same for sourrounding cells if a single is found
				this->checkRemoveSingle(x, y, changedStates, true);
			}
		}
	}
}
////////////////////////////////////////////////////////

void MapGenerator::updateTexture()
{
	this->texture.clear();
	this->texture.create(this->terrainVec.size(), this->terrainVec[0].size());

	RectangleShape cell;
	cell.setOutlineThickness(0.f);
	cell.setSize(Vector2f(1.f, 1.f));

	for (int y = 0; y < this->mapDimensions.y; y++)
	{
		for (int x = 0; x < this->mapDimensions.x; x++)
		{
			cell.setPosition(static_cast<float>(x), static_cast<float>(y));
			for (auto& heightRegion : this->heightRegions)
			{
				if (this->terrainVec[x][y] == heightRegion.type)
					cell.setFillColor(heightRegion.color);
			}
			for (auto& forestRegion : this->forestRegions)
			{
				if (this->terrainVec[x][y] == forestRegion.type)
					cell.setFillColor(forestRegion.color);
			}
			for (auto& wheatRegion : this->wheatRegions)
			{
				if (this->terrainVec[x][y] == wheatRegion.type)
					cell.setFillColor(wheatRegion.color);
			}
			this->texture.draw(cell);
		}
	}
	this->texture.display();
	this->sprite.setTexture(this->texture.getTexture());
	this->constDrawPos = this->sprite.getPosition();
	this->constDrawScale = this->sprite.getScale();
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
	Clock clock;
	for (int i = 0; i < USHRT_MAX; i++)
	{
		std::cout << "" << std::endl;
		continue;
	}
	Int64 t = clock.getElapsedTime().asMicroseconds();
	unsigned int seed = (time(NULL) * t) % INT_MAX;
	return seed;
}

std::vector<std::vector<float>> MapGenerator::generateNoiseMap(const unsigned int& seed, const unsigned int& width, const unsigned int& height, float& scale, const int& octaves, const float& persistance, const float& lacunarity, const sf::Vector2f& offset)
{
	std::vector<std::vector<float>> noiseMap(width, std::vector<float>(height, 0));

	srand(seed);
	PerlinNoise pn(seed);

	std::vector<sf::Vector2f> octaveOffsets;
	for (int i = 0; i < octaves; i++)
	{
		float offsetX = util::fn::randFloatFromRange(-1000000.f, 1000000.f) + offset.x;
		float offsetY = util::fn::randFloatFromRange(-1000000.f, 1000000.f) + offset.y;
		octaveOffsets.push_back(sf::Vector2f(offsetX, offsetY));
	}

	if (scale <= 0)
	{
		scale = 0.0001f;
	}

	float maxNoiseHeight = std::numeric_limits<float>::min();
	float minNoiseHeight = std::numeric_limits<float>::max();

	double halfWidth = static_cast<float>(width / 2.f);
	double halfHeight = static_cast<float>(height / 2.f);

	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			float amplitude = 1;
			float frequency = 1;
			float noiseHeight = 0;

			for (int i = 0; i < octaves; i++)
			{
				float sampleX = (x - halfWidth) / scale * frequency + octaveOffsets[i].x;
				float sampleY = (y - halfHeight) / scale * frequency + octaveOffsets[i].y;

				float perlinValue = static_cast<float>(pn.noise(sampleX, sampleY) * 2 - 1);
				noiseHeight += perlinValue * amplitude;
				amplitude *= persistance;
				frequency *= lacunarity;
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
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			noiseMap[x][y] = PerlinNoise::invLerp(minNoiseHeight, maxNoiseHeight, noiseMap[x][y]);
		}
	}
	return noiseMap;
}