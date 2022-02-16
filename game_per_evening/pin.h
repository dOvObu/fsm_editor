#pragma once
#include "FSM.h"
#include <SFML/Graphics.hpp>
#include <set>

class pin
{
public:
	void init(sf::RenderWindow& window);
	void update();

	bool is_pointed(sf::RenderWindow const& window) const;
	pin();
	~pin();

	static pin* get_first_pointed(sf::RenderWindow const& window);
private:
	FSM fsm;
	sf::RectangleShape _shape;
	
	static bool fsm_initialized;
	static FSM prototype_fsm;
	static std::set<pin*> _all_pins;
};
