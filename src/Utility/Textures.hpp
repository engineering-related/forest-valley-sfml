#ifndef UTIL_TEXTURE_HANDLER
#define UTIL_TEXTURES_HANDLER

namespace util
{
	struct txh
	{
		static std::vector<sf::Texture*> activeTextures;
		static sf::Texture* playerBase,
							items,
							Building,
							Cave,
							Dirt,
							Fancy,
							Ground,
							Ground2,
							Nature,
							Nature2,
							Rock,
							Stone;

		static void loadTexture(sf::Texture* texture, std::string fileName)
		{
			if(texture != nullptr) return;
			texture = new sf::Texture;
			txh::activeTextures.push_back(texture);
			texture->loadFromFile(fileName);
		}

		static void deleteTexture(sf::Texture* texture)
		{
			//The texture gets deleted and removed from the "activeTextures" vec
			util::fn::deleteObjInVector(texture, txh::activeTextures);
		}

		static void deleteAllTextures()
		{
			for (sf::Texture* texture : txh::activeTextures)
			{
				delete texture;
			}
		}
	};
}

#endif //UTIL_TEXTURE_HANDLER