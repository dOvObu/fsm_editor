#pragma once
#include "pin.h"

class node
{
public:
	void init()
	{
		if (!fsm_initialized)
		{
			proto_fsm.AddTransition("idle", "click", "drag");
			proto_fsm.AddTransition("idle", "in_selection_rectangle", "many_selected");
			proto_fsm.AddTransition("idle", "click", "drag");
			proto_fsm.AddTransition("idle", "click", "drag");
			fsm_initialized = true;
		}

	}
	void update()
	{
	}

private:
	FSM fsm;
	sf::RectangleShape _shape;

	static bool fsm_initialized;
	static FSM proto_fsm;
	static std::set<pin*> _all_pins;
};
