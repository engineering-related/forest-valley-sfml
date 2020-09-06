#ifndef UTILS_FONT_HANDLER
#define UTILS_FONT_HANDLER

namespace util
{
	//Fonts
	struct fnt
	{
		struct GameFont {
			sf::Font* font;
			std::string fileName;
			GameFont(std::string fileName)
			{
				this->fileName = fileName;
				this->font = new sf::Font();
				if (!font->loadFromFile(fileName))
				{
					std::cout << "ERROR: Could not load font " + fileName + "." << std::endl;
				}
			}
			~GameFont()
			{
				delete font;
			}
		};

		inline static GameFont *const PixelFont = new GameFont("content/fonts/DisposableDroidBB.ttf");

	};
}

#endif //UTILS_FONT_HANDLER