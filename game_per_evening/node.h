#pragma once
#include <iostream>

#include "pin.h"
#include <SFML/Graphics/Drawable.hpp>

class node : public sf::Drawable
{
public:
	std::string prev = "";
	void init(sf::RenderWindow& window)
	{
		if (!fsm_initialized)
		{
			proto_fsm.LoadFromFile("node.fsm");

			proto_fsm.OpenTransitionIf("in_selection_rectangle", [] { return false; });
			proto_fsm.OpenTransitionIf("deny", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Escape); });
			proto_fsm.OpenTransitionIf("start_typing", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::E); });
			proto_fsm.OpenTransitionIf("remove", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace); });
			proto_fsm.OpenTransitionIf("confirm", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Enter); });
			fsm_initialized = true;
		}
		fsm = proto_fsm;
		auto this_ = this;
		auto& offset = _offset;
		fsm.OpenTransitionIf("release", [this_] {bool const res = !sf::Mouse::isButtonPressed(sf::Mouse::Left); this_->set_selected(!res); return res; });
		fsm.OpenTransitionIf({"click", "click_many", "click_selected"}, [this_, &window, &offset] { bool const res = this_->is_pointed(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left); if (res) { offset = this_->get_position() - window.mapPixelToCoords(sf::Mouse::getPosition(window)); this_->set_selected(res); } return res; });

		_shape.setSize({ 100.f,100.f });
		_shape.setPosition({ 100.f,100.f });
	}
	void update(sf::RenderWindow& window)
	{
		fsm.TryTransit();
		auto const state = fsm.GetState();
		if (prev != state) std::cout << state << std::endl;
		prev = state;
		if (state == "idle" && _selected)
		{
			_shape.setFillColor(sf::Color::White);
			set_selected(false);
		}
		else if (state == "drag")
		{
			_shape.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) + _offset);
			_shape.setFillColor(sf::Color::Cyan);
		}
		else if (state == "one_selected" && !_selected)
		{
			_shape.setFillColor(sf::Color::Cyan);
			set_selected(true);
		}
		else if (state == "edit_name") {}
		else if (state == "many_selected") {}
		else if (state == "drag_many") {}
		else if (state == "die")
		{
			_shape.setFillColor(sf::Color::Black);
		}
	}

	void set_selected(bool const selected)
	{
		if (selected)
		{
			for (auto& nd : _all_nodes)
			{
				nd->set_selected(false);
			}
		}
		_selected = selected;
	}
	sf::Vector2f get_position() const {return _shape.getPosition(); }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_shape, states);
	}
	node() { _all_nodes.insert(this); }
	~node() override { _all_nodes.erase(this); }

private:
	FSM fsm;
	sf::RectangleShape _shape;
	sf::Vector2f _offset;
	bool _selected { false };

	bool is_pointed(sf::RenderWindow const& window) const
	{
		return _shape.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	}
	static bool fsm_initialized;
	static FSM proto_fsm;
	static std::set<node*> _all_nodes;
};
