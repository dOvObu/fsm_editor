#pragma once
#include <iostream>

#include "pin.h"
#include "input.h"
#include <SFML/Graphics/Drawable.hpp>


class node : public sf::Drawable, public IEventHandler
{
public:
	std::string prev = "";
	void init(sf::RenderWindow& window)
	{
		if (!fsm_initialized)
		{
			pr_fsm.AddTransition("idle", "click", "drag");
			pr_fsm.AddTransition("idle", "in_selection_rect", "select_many");
			pr_fsm.AddTransition("drag", "release", "selected");
			pr_fsm.AddTransition("selected", "deny", "idle");
			pr_fsm.AddTransition("selected", "start_edit", "edit_name");
			pr_fsm.AddTransition("selected", "remove", "dead");
			pr_fsm.AddTransition("edit_name", "enter", "save_new_name");
			pr_fsm.AddTransition("edit_name", "deny", "selected");
			pr_fsm.AddTransition("save_new_name", "true", "selected");
			pr_fsm.AddTransition("select_many", "deny", "idle");
			pr_fsm.AddTransition("select_many", "remove", "dead");
			pr_fsm.AddTransition("select_many", "click", "drag_many");
			pr_fsm.AddTransition("drag_many", "release", "select_many");
			/*
			pr_fsm.OpenTransitionIf("in_selection_rectangle", [] { return false; });
			pr_fsm.OpenTransitionIf("deny", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Escape); });
			pr_fsm.OpenTransitionIf("start_typing", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::E); });
			pr_fsm.OpenTransitionIf("remove", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace); });
			pr_fsm.OpenTransitionIf("confirm", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Enter); });
			*/

			pr_fsm.OpenTransitionIf("in_selection_rect", []() {
				return false;
			});

			pr_fsm.OpenTransitionIf("deny", []() {
				return false;
			});
			pr_fsm.OpenTransitionIf("start_edit", []() {
				return false;
			});
			pr_fsm.OpenTransitionIf("remove", []() {
				return false;
			});
			pr_fsm.OpenTransitionIf("enter", []() {
				return false;
			});
			pr_fsm.OpenTransitionIf("true", []() {return true;});
			fsm_initialized = true;
		}
		fsm = pr_fsm;
		auto this_ = this;
		auto& offset = _offset;

		Input::SubscribeOn(sf::Event::EventType::MouseButtonPressed, this);

		pr_fsm.OpenTransitionIf("click", [this_]() { return !selected && this_->_shape.getGlobalBounds().contains(Input::GetMousePosition()); });
		pr_fsm.OpenTransitionIf("release", [this_]() {
			return false;
			});
		/*
		fsm.OpenTransitionIf("release", [this_]
		{
			bool const res = !sf::Mouse::isButtonPressed(sf::Mouse::Left);
			this_->set_selected(!res);
			return res;
		});
		fsm.OpenTransitionIf({"click", "click_many", "click_selected"}, [this_, &window, &offset]
		{
			bool const res = this_->is_pointed(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
			if (res)
			{
				offset = this_->get_position() - window.mapPixelToCoords(sf::Mouse::getPosition(window));
				this_->set_selected(res);
			}
			return res;
		});
		*/
	}
	~node() override
	{
		Input::Unsubscribe(this);
	}
	static bool selected;
	void update(sf::RenderWindow& window)
	{
		fsm.TryTransit();
		auto const state = fsm.GetState();
		if (prev != state) std::cout << state << std::endl;
		prev = state;


		if (state == "idle") { if (selected) _shape.setOutlineThickness(0.f); }
		else if (state == "drag") {}
		else if (state == "select_many") {}
		else if (state == "selected") { if (!selected) _shape.setOutlineColor(sf::Color::Cyan), _shape.setOutlineThickness(3.f), selected = true; }
		else if (state == "edit_name") {}
		else if (state == "dead") { _shape.setSize(sf::Vector2f{ 0,0 }); }
		else if (state == "save_new_name") {}
		else if (state == "drag_many") {}

		/*
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
		*/
	}

	void set_selected(bool const selected)
	{
		if (selected)
		{
			for (auto& nd : _all_nodes)
			{
				nd->_selected = false;
				nd->fsm.Transit(nd->fsm.GetTransitionIndex("release"));
			}
		}
		_selected = selected;
	}
	sf::Vector2f get_position() const {return _shape.getPosition(); }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_shape, states);
	}
	node(sf::Vector2f size, sf::Vector2f position)
	{
		_all_nodes.insert(this);
		_shape.setSize(size);
		_shape.setPosition(position);
	}
	~node() override
	{
		_all_nodes.erase(this);
		Input::Unsubscribe(this);
	}

	void OnEvent(sf::Event const& ev) override
	{
		switch (ev.type)
		{
		case sf::Event::MouseButtonPressed:
			if (_shape.getGlobalBounds().contains(Input::GetMousePosition()))
			{
				fsm.Transit(fsm.GetTransitionIndex("click"));
			}
			break;

		default:break;
		}
	}

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
	static FSM pr_fsm;
	static std::set<node*> _all_nodes;
};
