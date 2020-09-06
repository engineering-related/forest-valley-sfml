#ifndef UTIL_TEXTURE_HANDLER
#define UTIL_TEXTURE_HANDLER

namespace util
{
	struct txh
	{
		//A vector of all the active textures
		inline static std::vector<sf::Texture*> *const activeTextures = new std::vector<sf::Texture*>;
		struct Data
		{
		private:
			sf::Texture* texture;
			std::string fileName;
			sf::Vector2i nrOfSheetImages;
			bool loaded = false;

		public:
			Data(std::string fileName, sf::Vector2i nrOfSheetImages)
			{
				this->fileName = fileName;
				this->nrOfSheetImages = nrOfSheetImages;
			}
			~Data(){ this->deleteTexture(); }

			void loadTexture()
			{
				if(this->loaded) return;
				this->texture = new sf::Texture;
				util::txh::activeTextures->push_back(this->texture);
				this->texture->loadFromFile(this->fileName);
				this->loaded = true;
			}

			const bool& isLoaded() const
			{
				return this->loaded;
			}

			sf::Texture* getTexture()
			{
				if (!this->loaded)
				{
					this->texture = new sf::Texture;
					util::txh::activeTextures->push_back(this->texture);
					this->texture->loadFromFile(this->fileName);
					this->loaded = true;
				}
				return this->texture;
			}
			const sf::Vector2i& getNrOfSheetImages() const
			{
				return this->nrOfSheetImages;
			}

			void deleteTexture()
			{
				//The texture gets deleted and removed from the "activeTextures" vec
				util::fn::deleteObjInVector(this->texture, *util::txh::activeTextures);
				this->loaded = false;
			}
		};

		//Define all static textures
		///////////////////////////////////////////////////////////////////////////////////
		inline static Data *const PlayerBase = new Data("content/characters/PlayerPoke.png", sf::Vector2i(4, 4)),
								  *const Items = new Data("content/items/Items.png", sf::Vector2i(8, 1)),
								  // *const Building = new Data("Building.png" /*, sf::Vector2i(?, ?)*/),
									  //*const Cave = new Data("Cave.png" /*, sf::Vector2i(?, ?)*/),
									  //*const Dirt = new Data("Dirt.png"),
									  //*const Fancy = new Data("Fancy.png"),
									  *const Ground = new Data("content/terrain/Ground.png", sf::Vector2i(480 / 32, 768 / 32)),
								  //*const Ground2 = new Data("Ground2.png");
										  *const Nature = new Data("content/terrain/Nature.png", sf::Vector2i(480/32, 128/32)),
								  //*const Nature2 = new Data("Nature2.png"),
											  *const Rock = new Data("content/terrain/Rock.png", sf::Vector2i(224 / 32, 320 / 32));
		// *const Stone = new Data("Stone.png");
		//////////////////////////////////////////////////////////////////////////////////


		static void deleteAllTextures()
		{
			for (sf::Texture* texture : *util::txh::activeTextures)
			{
				delete texture;
			}
		}

		static std::size_t getNrOfActiveTextures()
		{
			return util::txh::activeTextures->size();
		}
	};
}

#endif //UTIL_TEXTURE_HANDLER