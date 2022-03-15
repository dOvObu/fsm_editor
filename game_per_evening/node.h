#pragma once

#include "pin.h"
#include "input.h"
#include "set_utility.h"
#include "IColorizable.h"
#include "Label.h"
#include "theme.h"

#include <SFML/Graphics/Drawable.hpp>

#include "Answer.h"
#include "button.h"
#include "Pool.h"


class node : public sf::Drawable, public IEventHandler, public IColorizable, public IConnectionOrigin
{
public:
	std::string prev = "";

	void ApplyTheme() override
	{
		_shape.setFillColor(theme::NodeBackgroundColor);
		_add_answer.ApplyTheme();
		_add_answer.Size({ _shape.getSize().x, _add_answer.Size().y });
	}

	void init()
	{
		if (!fsm_initialized)
		{
			pr_fsm.LoadFromFile("node.fsm");

			pr_fsm.OpenTransitionIf("in_selection_rect", [] { return false; });

			pr_fsm.OpenTransitionIf("deny", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape); });
			pr_fsm.OpenTransitionIf("start_edit", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2); });
			pr_fsm.OpenTransitionIf("remove", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace); });
			pr_fsm.OpenTransitionIf("enter", [] { return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter); });
			pr_fsm.OpenTransitionIf("true", [] { return true; });
			fsm_initialized = true;
		}
		fsm = pr_fsm;
		_dead = false;
		auto self = this;
		Input::SubscribeOn(sf::Event::EventType::MouseButtonPressed, this);
		Input::SubscribeOn(sf::Event::EventType::MouseButtonReleased, this);
		Input::SubscribeOn(sf::Event::EventType::TextEntered, this);

		for (auto answer : _answers) Answer::Delete(answer);
		_answers.clear();

		_add_answer.Subscribe(this, [self]
		{
			self->_answers.push_back(Answer::Create());
			self->_answers.back()->OnCrossClicked([self]()
			{
				Answer::Delete(self->_answers.back());
				self->_answers.pop_back();
				self->Position(self->Position());
			});
			self->Position(self->Position());
		});
		ApplyTheme();
	}

	node* update()
	{
		fsm.TryTransit();
		auto const state = fsm.GetState();
		prev = state;

		if (state == "idle")
		{
			if (_is_showing_selected)
			{
				_shape.setOutlineThickness(0.f);
				_is_showing_selected = false;
			}
		}
		else if (state == "drag")
		{
			if (!_is_showing_selected)
			{
				_shape.setOutlineColor(sf::Color::Cyan);
				_shape.setOutlineThickness(3.f);
				_is_showing_selected = true;
			}
			Position(Input::GetMousePosition() + _offset);
		}
		else if (state == "select_many") {}
		else if (state == "selected") {}
		else if (state == "edit_name") {}
		else if (state == "dead")
		{
			_dead = true;
		}
		else if (state == "save_new_name") {}
		else if (state == "drag_many") {}
		
		return this;
	}

	void SetSelected(bool const selected)
	{
		if (selected)
		{
			_all_nodes.Foreach([](auto nd)
			{
				nd->_selected = false;
				nd->fsm.Transit(nd->fsm.GetTransitionIndex("release"));
			});
		}
		_selected = selected;
	}
	sf::Vector2f Position() const {return _shape.getPosition(); }
	void Position(sf::Vector2f const& position)
	{
		_shape.setPosition(position);
		_label.Position(position);
		float yOffset = _shape.getSize().y;
		for (Answer* answer : _answers)
		{
			yOffset += 5.f;
			answer->Position(position + sf::Vector2f{ 0.f, yOffset });
			yOffset += answer->Size().y;
		}
		yOffset += 5.f;
		_add_answer.Position(position + sf::Vector2f{ 0.f, yOffset });
	}
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_shape, states);
		target.draw(_label, states);
		for (auto answer : _answers) target.draw(*answer, states);
		target.draw(_add_answer, states);
	}

	node* Transit(std::string const& transition)
	{
		fsm.Transit(fsm.GetTransitionIndex(transition));
		return this;
	}

	static node* Create()
	{
		node* n = _all_nodes.Create();
		n->Position(Input::GetMousePosition());
		return n;
	}


	node(sf::Vector2f size, sf::Vector2f position) : _label(_prev_name), _add_answer("+")
	{
		_all_colorizables.insert(this);
		_shape.setSize(size);
		Position(position);
	}
	~node() override
	{
		_all_colorizables.erase(this);
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
				_offset = Position() - Input::GetMousePosition();
				auto self = this;
				_all_nodes.Foreach([self](node* item)
				{
					if (self != item)
					{
						item->fsm.Transit(item->fsm.GetTransitionIndex("deny"));
					}
				});
			}
			break;

		case sf::Event::MouseButtonReleased:
			if (_shape.getGlobalBounds().contains(Input::GetMousePosition()))
			{
				fsm.Transit(fsm.GetTransitionIndex("release"));
			}
			break;

		case sf::Event::TextEntered:
			if (fsm.GetState() == "edit_name")
			{
				if (ev.text.unicode == 8) { _label.Text(_label.Text().substring(0, _label.Text().getSize() - 1)); }
				else if (ev.text.unicode == 27) { _label.Text(_prev_name); }
				else if (ev.text.unicode == 13) { _prev_name = _label.Text(); }
				else
				{
					_label.Text(_label.Text() + ev.text.unicode);
				}
			}
			break;

		default:break;
		}
	}

	sf::Vector2f ConnectionOrigin() const override
	{
		return _shape.getPosition() + sf::Vector2f(0.5f * _shape.getSize().x, 0.f);
	}

	static void RemoveAllDead()
	{
		_all_nodes.DeleteAll([](node* x) { return x->_dead; });
	}

	static void UpdateAll()
	{
		_all_nodes.Foreach([](node* item) { item->update(); });
	}

	static void DrawAll(sf::RenderWindow& window)
	{
		_all_nodes.Foreach([&window](node* item){ window.draw(*item); });
	}

	static bool IsMouseOverSomeNode()
	{
		bool over_something = false;
		sf::Vector2f const mouse_position = Input::GetMousePosition();
		_all_nodes.Foreach([&over_something, mouse_position](node* item)
		{
			if (over_something) return;
			over_something = item->_shape.getGlobalBounds().contains(mouse_position);
			if (over_something) _overlapped_node = item;
		});
		return over_something;
	}

	static IConnectionOrigin* LastNodeUnderMouse()
	{
		return _overlapped_node;
	}

private:
	FSM fsm;
	sf::RectangleShape _shape;
	Label _label;
	sf::Vector2f _offset;
	bool _selected { false };
	bool _dead{ false };
	bool _is_showing_selected{ false };
	std::vector<Answer*> _answers;
	button _add_answer;

	bool is_pointed(sf::RenderWindow const& window) const
	{
		return _shape.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	}
	
	static bool fsm_initialized;
	static FSM pr_fsm;
	
	static Pool<node*> _all_nodes;
	static sf::String _prev_name;
	static node* _dragged_node;
	static node* _overlapped_node;
};
