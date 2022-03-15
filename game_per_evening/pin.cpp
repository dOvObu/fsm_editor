#include "pin.h"
#include "node.h"

#include "input.h"

bool pin::fsm_initialized;
FSM pin::prototype_fsm;
Pool<pin*> pin::_all_pins([]() { return new pin(); });

void pin::init()
{
	if (!fsm_initialized)
	{
		prototype_fsm.AddTransition("idle", "click", "create_connection");
		prototype_fsm.AddTransition("create_connection", "deny", "idle");
		prototype_fsm.AddTransition("create_connection", "release_on_nothing", "create_node_selected");
		prototype_fsm.AddTransition("create_node_selected", "connected", "complete_connection");
		prototype_fsm.AddTransition("create_connection", "release_on_other", "complete_connection");
		prototype_fsm.AddTransition("complete_connection", "connected", "idle");

		//prototype_fsm.WriteToFile("pin.fsm");
		//prototype_fsm.LoadFromFile("pin.fsm");

		prototype_fsm.OpenTransitionIf("release_on_nothing", []
		{
			bool res = !sf::Mouse::isButtonPressed(sf::Mouse::Left) && !node::IsMouseOverSomeNode();
			return res;
		});
		prototype_fsm.OpenTransitionIf("release_on_other", []
		{
			bool res = !sf::Mouse::isButtonPressed(sf::Mouse::Left) && node::IsMouseOverSomeNode();
			return res;
		});
		//prototype_fsm.OpenTransitionIf("connected", [] { return false; });
		fsm_initialized = true;
	}
	fsm = prototype_fsm;
	Input::SubscribeOn(sf::Event::MouseButtonReleased, this);
	Input::SubscribeOn(sf::Event::KeyReleased, this);

	pin* self = this;
	fsm.OpenTransitionIf("deny", [self]
		{
			bool const res = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
			if (res && self->_connection != nullptr)
			{
				connection::Delete(self->_connection);
				self->_connection = nullptr;
			}
			return res;
		});
}

void pin::update()
{
	fsm.TryTransit();
	auto const state = fsm.GetState();
		//else if (state == "create_connection") {}
	if (state == "create_node_selected")
	{
		std::cout << "create_node_selected\n";
		_connection->To(node::Create());
		fsm.Transit(fsm.GetTransitionIndex("connected"));
	}
	else if (state == "complete_connection")
	{
		std::cout << "complete_connection\n";
		_connection->To(node::LastNodeUnderMouse());
		fsm.Transit(fsm.GetTransitionIndex("connected"));
	}
	//if (_connection != nullptr) _connection->update();
}

void pin::OnEvent(sf::Event const& ev)
{
	if (ev.type == sf::Event::MouseButtonReleased)
	{
		std::string const state = fsm.GetState();

		if (state == "idle" && IsPointed() && fsm.Transit(fsm.GetTransitionIndex("click")))
		{
			_connection = connection::Create();
			_connection->From(this);
		}
		//else if (state == "create_connection")
		//{
		//	if (node::IsMouseOverSomeNode()) fsm.Transit(fsm.GetTransitionIndex("release_on_other"));
		//	else fsm.Transit(fsm.GetTransitionIndex("release_on_nothing"));
		//}
	}
}

// // --------------- GENERATED OUT OF -------------------
