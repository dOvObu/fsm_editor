#include "Label.h"
#include "node.h"
#include <SFML/Graphics.hpp>




int main()
{
	sf::RenderWindow window{ sf::VideoMode{800, 600}, sf::String{} };
	Label label(sf::String{ L"проверка" });
	node nd;
	nd.init(window);

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e)) if (e.type == sf::Event::Closed) window.close();
		nd.update(window);
		window.clear();
		window.draw(label);
		window.draw(nd);
		window.display();
	}
	return 0;
}
