#include <set>

#include "Label.h"
#include <SFML/Graphics.hpp>





int main()
{
	sf::RenderWindow window{ sf::VideoMode{800, 600}, sf::String{} };
	Label label(sf::String{ L"проверка" });


	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e)) if (e.type == sf::Event::Closed) window.close();
		window.clear();
		window.draw(label);
		window.display();
	}
	return 0;
}
