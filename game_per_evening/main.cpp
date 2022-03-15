#include "node.h"
#include "input.h"
#include "theme.h"


int main()
{
	sf::RenderWindow window{ sf::VideoMode{800, 600}, sf::String{} };
	Input::Init(&window);
	theme::Load("theme.txt");

	//Label label(sf::String{ L"проверка" });

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			Input::NotifySubsOn(e);
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
			if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::F3)
			{
				auto nd = node::Create();
				nd->SetSelected(true);
				nd->Transit("release");
			}
		}

		node::RemoveAllDead();
		node::UpdateAll();
		pin::UpdateAll();
		connection::UpdateAll();
		
		window.clear(theme::BackgroundColor);
		//window.draw(label);
		node::DrawAll(window);
		connection::DrawAll(window);
		window.display();
	}
	return 0;
}
