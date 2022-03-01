#pragma once
#include <iostream>

#include "pin.h"
#include "input.h"
#include <SFML/Graphics/Drawable.hpp>

#include "Label.h"


class node : public sf::Drawable, public IEventHandler
{
public:
	std::string prev = "";
	void init(sf::RenderWindow& window)
	{
		if (!fsm_initialized)
		{
			pr_fsm.AddTransition("idle", "click", "drag");
			pr_fsm.AddTransition("_is_showing_selected", "click", "drag");
			pr_fsm.AddTransition("idle", "in_selection_rect", "select_many");
			pr_fsm.AddTransition("drag", "release", "_is_showing_selected");
			pr_fsm.AddTransition("_is_showing_selected", "deny", "idle");
			pr_fsm.AddTransition("_is_showing_selected", "start_edit", "edit_name");
			pr_fsm.AddTransition("_is_showing_selected", "remove", "dead");
			pr_fsm.AddTransition("edit_name", "enter", "save_new_name");
			pr_fsm.AddTransition("edit_name", "deny", "_is_showing_selected");
			pr_fsm.AddTransition("save_new_name", "true", "_is_showing_selected");
			pr_fsm.AddTransition("select_many", "deny", "idle");
			pr_fsm.AddTransition("select_many", "remove", "dead");
			pr_fsm.AddTransition("select_many", "click", "drag_many");
			pr_fsm.AddTransition("drag_many", "release", "select_many");

			pr_fsm.OpenTransitionIf("in_selection_rect", [] {
				return false;
			});

			pr_fsm.OpenTransitionIf("deny", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape); });
			pr_fsm.OpenTransitionIf("start_edit", [] { bool res = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E); if (res) _pass_one_character_on_edit_name = true; return res; });
			pr_fsm.OpenTransitionIf("remove", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace); });
			pr_fsm.OpenTransitionIf("enter", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter); });
			pr_fsm.OpenTransitionIf("true", [] {return true;});
			fsm_initialized = true;
		}
		fsm = pr_fsm;
		auto this_ = this;

		Input::SubscribeOn(sf::Event::EventType::MouseButtonPressed, this);
		Input::SubscribeOn(sf::Event::EventType::MouseButtonReleased, this);
		Input::SubscribeOn(sf::Event::EventType::TextEntered, this);

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
				offset = this_->Position() - window.mapPixelToCoords(sf::Mouse::getPosition(window));
				this_->set_selected(res);
			}
			return res;
		});
		*/
	}
	void update(sf::RenderWindow& window)
	{
		fsm.TryTransit();
		auto const state = fsm.GetState();
		if (prev != state) std::cout << state << std::endl;
		prev = state;


		if (state == "idle") { if (_is_showing_selected) _shape.setOutlineThickness(0.f), _is_showing_selected = false; }
		else if (state == "drag")
		{
			if (!_is_showing_selected) _shape.setOutlineColor(sf::Color::Cyan), _shape.setOutlineThickness(3.f), _is_showing_selected = true;
			Position(Input::GetMousePosition() + _offset);
		}
		else if (state == "select_many") {}
		else if (state == "_is_showing_selected") {}
		else if (state == "edit_name") {}
		else if (state == "dead") { _shape.setSize(sf::Vector2f{ 0,0 }); }
		else if (state == "save_new_name") {}
		else if (state == "drag_many") {}
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
	sf::Vector2f Position() const {return _shape.getPosition(); }
	void Position(sf::Vector2f const& position) { _shape.setPosition(position), _label.Position(position); }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_shape, states);
		target.draw(_label, states);
	}
	node(sf::Vector2f size, sf::Vector2f position) : _label(_prev_name)
	{
		_all_nodes.insert(this);
		_shape.setSize(size);
		Position(position);
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
			if (_shape.getGlobalBounds().contains(Input::GetMousePosition())) fsm.Transit(fsm.GetTransitionIndex("click")), _offset = Position() - Input::GetMousePosition();
			break;

		case sf::Event::MouseButtonReleased:
			if (_shape.getGlobalBounds().contains(Input::GetMousePosition())) fsm.Transit(fsm.GetTransitionIndex("release"));
			break;

		case sf::Event::TextEntered:
			std::cout << (int)ev.text.unicode << std::endl;
			if (fsm.GetState() == "edit_name")
			{
				if (_pass_one_character_on_edit_name) { _pass_one_character_on_edit_name = false; }
				else if (ev.text.unicode == 8) { _label.Text(_label.Text().substring(0, _label.Text().getSize() - 1)); }
				else if (ev.text.unicode == 27) { _label.Text(_prev_name); }
				else if (ev.text.unicode == 14) { _prev_name = _label.Text(); }
				else { _label.Text(_label.Text() + sf::String((char)ev.text.unicode)); }
			}
			break;

		default:break;
		}
	}

private:
	FSM fsm;
	sf::RectangleShape _shape;
	Label _label;
	sf::Vector2f _offset;
	bool _selected { false };

	bool is_pointed(sf::RenderWindow const& window) const
	{
		return _shape.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	}
	
	static bool fsm_initialized;
	static FSM pr_fsm;
	static std::set<node*> _all_nodes;
	static bool _is_showing_selected;
	static bool _pass_one_character_on_edit_name;
	static sf::String _prev_name;
};
