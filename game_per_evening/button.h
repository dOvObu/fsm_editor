#pragma once

#include "Label.h"
#include "Input.h"
#include "IColorizable.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <functional>
#include <map>

#include "theme.h"


class button : public sf::Drawable, public IEventHandler, IColorizable
{
public:
	void Subscribe(void* key, std::function<void()> cb)
	{
		_on_click[key] = cb;
	}

	void Unsubscribe(void* key)
	{
		_on_click.erase(key);
	}

	sf::Vector2f Position() const
	{
		return _shape.getPosition();
	}

	void Position(sf::Vector2f const& position)
	{
		_shape.setPosition(position);
		_label.Position(position + theme::ButtonDefaultTextOffset - sf::Vector2f{0.f,12.f});
	}

	sf::Vector2f Size() const
	{
		return _shape.getSize();
	}

	sf::Vector2f TextSize() const
	{
		auto rect = _label.Rect();
		return { rect.width, rect.height };
	}
	
	void Size(sf::Vector2f const& size)
	{
		_shape.setSize(size);
	}

	sf::String Text() const
	{
		return _label.Text();
	}

	void Text(sf::String const& text)
	{
		_label.Text(text);
	}

	void Click()
	{
		for (auto cb : _on_click) cb.second();
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_shape, states);
		target.draw(_label, states);
	}

	void ApplyTheme() override
	{
		_shape.setFillColor(theme::ButtonColor);
		_shape.setSize(TextSize() + 2.f*theme::ButtonDefaultTextOffset);
	}

	void Color(sf::Color color)
	{
		_shape.setFillColor(color);
	}

	button(sf::String text)
		:_label(text)
	{
		auto this_ = this;
		Input::SubscribeOn(sf::Event::MouseButtonReleased, this);
	}

	void OnEvent(sf::Event const& ev) override
	{
		if (_shape.getGlobalBounds().contains(Input::GetMousePosition())) Click();
	}

private:
	std::map<void*, std::function<void()>> _on_click;
	sf::RectangleShape _shape;
	Label _label;
};
