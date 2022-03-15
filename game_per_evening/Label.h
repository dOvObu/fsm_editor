#ifndef LABEL_H
#define LABEL_H
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>

class Label : public sf::Drawable
{
public:
	explicit Label(sf::String const& s)
		: string(s)
	{
		if (!fontLoaded)
		{
			font.loadFromFile("./JetBrainsMono-Regular.ttf");
			fontLoaded = true;
		}
		text.setString(s);
		text.setFont(font);
	}

	void Position(sf::Vector2f const& v) { text.setPosition(v); }
	sf::Vector2f Position() const { return text.getPosition(); }

	void Scale(sf::Vector2f const& v) { text.setScale(v); }
	sf::Vector2f Scale() const { return text.getScale(); }

	sf::FloatRect LocalRect() { text.getLocalBounds(); }
	sf::FloatRect Rect() const { return text.getGlobalBounds(); }

	void Text(sf::String const& v) { text.setString(v); }
	sf::String Text() const { return text.getString(); }

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(text);
	}


private:
	sf::Text text;
	std::wstring string;

	static sf::Font font;
	static bool fontLoaded;
};



#endif
