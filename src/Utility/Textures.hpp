#ifndef UTIL_TEXTURE_HANDLER
#define UTIL_TEXTURES_HANDLER

namespace util
{
	struct txh
	{
		//A vector of all the active textures
		inline static std::vector<sf::Texture*> *const activeTextures = new std::vector<sf::Texture*>;
		struct Data
		{
			sf::Texture* texture;
			std::string fileName;
			bool loaded = false;
			Data(std::string fileName)
			{
				this->fileName = fileName;
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

			void deleteTexture()
			{
				//The texture gets deleted and removed from the "activeTextures" vec
				util::fn::deleteObjInVector(this->texture, *util::txh::activeTextures);
				this->loaded = false;
			}
		};

			//Define all static textures
			///////////////////////////////////////////////////////////////////////////////////
			inline static Data *const PlayerBase = new Data("PlayerBase.png"),
									  *const Items = new Data("content/items/Items.png"),
									  *const Building = new Data("Building.png"),
									  *const Cave = new Data("Cave.png"),
									  *const Dirt = new Data("Dirt.png"),
									  *const Fancy = new Data("Fancy.png"),
									  *const Ground = new Data("Ground.png"),
									  *const Ground2 = new Data("Ground2.png"),
									  *const Nature = new Data("Nature.png"),
									  *const Nature2 = new Data("Nature2.png"),
									  *const Rock = new Data("Rock.png"),
									  *const Stone = new Data("Stone.png");
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