#pragma once
#include "FSM.h"
#include "IEventHandler.h"
#include "Pool.h"

#include "connection.h"
#include "IColorizable.h"
#include "input.h"
#include "theme.h"

#include <SFML/Graphics.hpp>


class pin : public sf::Drawable, public IEventHandler, public IColorizable, public IConnectionOrigin
{
public:
	void init();

	void update();

	bool IsPointed() const
	{
		return _shape.getGlobalBounds().contains(Input::GetMousePosition());
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_shape, states);
		if (_connection != nullptr) target.draw(*_connection, states);
	}

	void OnEvent(sf::Event const& ev) override;

	void ApplyTheme() override
	{
		_shape.setFillColor(theme::PinColor);
		_shape.setSize(theme::PinSize);
	}

	sf::Vector2f ConnectionOrigin() const override
	{
		sf::FloatRect const rect = _shape.getGlobalBounds();
		return { rect.left + 0.5f * rect.width, rect.top + 0.5f * rect.height };
	}

	sf::Vector2f Position() const { return _shape.getPosition(); }
	void Position(sf::Vector2f const& size) { _shape.setPosition(size); }

	sf::Vector2f Size() const { return _shape.getSize(); }
	void Size(sf::Vector2f size) { _shape.setSize(size); }
	static void UpdateAll()
	{
		_all_pins.Foreach([](auto item) {item->update(); });
	}


	static bool IsMouseOverSomePin()
	{
		bool over_something = false;
		sf::Vector2f const mouse_position = Input::GetMousePosition();
		_all_pins.Foreach([&over_something, mouse_position](pin* item)
		{
			if (over_something) return;
			over_something = item->_shape.getGlobalBounds().contains(mouse_position);
		});
		return over_something;
	}

private:
	FSM fsm;
	sf::RectangleShape _shape;
	connection* _connection{ nullptr };
	
	static bool fsm_initialized;
	static FSM prototype_fsm;
	static Pool<pin*> _all_pins;
};
