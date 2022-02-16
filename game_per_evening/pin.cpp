#include "pin.h"

bool pin::fsm_initialized;
FSM pin::prototype_fsm;
std::set<pin*> pin::_all_pins;


void pin::init(sf::RenderWindow& window)
{
	if (!fsm_initialized)
	{
		prototype_fsm.AddTransition("idle", "click", "create_connection");
		prototype_fsm.AddTransition("create_connection", "deny", "idle");
		prototype_fsm.AddTransition("create_connection", "release_on_nothing", "create_node_selected");
		prototype_fsm.AddTransition("create_node_selected", "true", "complete_connection");
		prototype_fsm.AddTransition("create_connection", "release_on_other", "complete_connection");
		prototype_fsm.AddTransition("complete_connection", "true", "idle");
		auto* this_pin = this;
		//fsm.WriteToFile("./pin.txt");

		prototype_fsm.OpenTransitionIf("click", [&window, this_pin] { bool const res = this_pin->is_pointed(window); return res; });
		prototype_fsm.OpenTransitionIf("deny", [] { bool const res = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape); return res; });
		prototype_fsm.OpenTransitionIf("release_on_nothing", [&window]()->bool { bool const res = get_first_pointed(window) == nullptr; return res; });
		prototype_fsm.OpenTransitionIf("release_on_other", [&window]()->bool { auto const pointed = get_first_pointed(window); bool const res = pointed != nullptr; return res; });
		prototype_fsm.OpenTransitionIf("true", [] { return true; });
		fsm_initialized = true;
	}
	fsm = prototype_fsm;
}


void pin::update()
{
	fsm.TryTransit();
	auto const state = fsm.GetState();
	if (state == "idle") {}
	else if (state == "create_connection") {}
	else if (state == "create_node_selected") {}
	else if (state == "complete_connection") {}
}

bool pin::is_pointed(sf::RenderWindow const& window) const
{
	return _shape.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
}

pin::pin()
{ _all_pins.insert(this); }

pin::~pin()
{ _all_pins.erase(this); }

pin* pin::get_first_pointed(sf::RenderWindow const& window)
{
	pin* result = nullptr;
	for (auto p : _all_pins)
	{
		if (p->is_pointed(window))
		{
			result = p;
			break;
		}
	}
	return result;
}
