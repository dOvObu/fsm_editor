#ifndef THEME_H
#define THEME_H
#include <SFML/Graphics/Color.hpp>

#include <string>
#include <fstream>
#include <SFML/System/Vector2.hpp>

class theme
{
public:
	static sf::Color BackgroundColor;
	static sf::Color NodeBackgroundColor;
	static sf::Color ButtonColor;
	static sf::Vector2f ButtonDefaultTextOffset;
	static sf::Color PinColor;
	static sf::Vector2f PinSize;
	static sf::Color CloseButtonColor;

	static void Load(std::string const& path)
	{
		std::ifstream file(path);
		int r, g, b, a;
		float x, y;

		file >> r >> g >> b >> a;
		BackgroundColor = sf::Color(r, g, b, a);
		while (file.get() != '\n');
		
		file >> r >> g >> b >> a;
		NodeBackgroundColor = sf::Color(r, g, b, a);
		while (file.get() != '\n');
		
		file >> r >> g >> b >> a;
		ButtonColor = sf::Color(r, g, b, a);
		while (file.get() != '\n');
		
		file >> x >> y;
		ButtonDefaultTextOffset = {x, y};
		while (file.get() != '\n');
		
		file >> r >> g >> b >> a;
		PinColor = sf::Color(r, g, b, a);
		while (file.get() != '\n');
		
		file >> x >> y;
		PinSize = { x, y };
		while (file.get() != '\n');
		
		file >> r >> g >> b >> a;
		CloseButtonColor = sf::Color(r, g, b, a);
	}
};

#endif // !THEME_H
