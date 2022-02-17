#include "Label.h"
#include "node.h"
#include <SFML/Graphics.hpp>




int main()
{
	sf::RenderWindow window{ sf::VideoMode{800, 600}, sf::String{} };
	Label label(sf::String{ L"проверка" });
	node nd({ 100.f,100.f }, { 100.f,100.f });
	node nd2({ 100.f,100.f }, { 100.f,100.f });
	nd.init(window);
	nd2.init(window);

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e)) if (e.type == sf::Event::Closed) window.close();
		nd.update(window);
		nd2.update(window);
		window.clear();
		window.draw(label);
		window.draw(nd);
		window.draw(nd2);
		window.display();
	}
	return 0;
}
