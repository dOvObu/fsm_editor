#pragma once
#include "button.h"
#include "pin.h"
#include "Pool.h"

class Answer : public sf::Drawable
{
public:

	void init()
	{
		_button.Text("unknown");
		_pin.init();
		_pin.ApplyTheme();
	}

	Answer()
		:_button("unknown"),
		_deleteIt("X")
	{
		_button.ApplyTheme();
		_deleteIt.ApplyTheme();
		_deleteIt.Size({40.f,30.f});
		_deleteIt.Color(theme::CloseButtonColor);
	}

	sf::Vector2f Position() const
	{
		return _button.Position();
	}

	void Position(sf::Vector2f const& position)
	{
		_button.Position(position);
		_deleteIt.Position(position + sf::Vector2f{ -45.f, 5.f });
		_pin.Position(position + sf::Vector2f{ _button.Size().x, 0.f });
	}

	sf::Vector2f Size() const
	{
		return _button.Size();
	}

	void Size(sf::Vector2f const& size)
	{
		_button.Size(size);
	}

	sf::String Text() const
	{
		return _button.Text();
	}

	void Text(sf::String const& text)
	{
		_button.Text(text);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(_deleteIt, states);
		target.draw(_button, states);
		target.draw(_pin, states);
	}

	void OnCrossClicked(std::function<void()> cb)
	{
		_deleteIt.Subscribe(this, cb);
	}

	static Answer* Create()
	{
		return _answers.Create();
	}
	
	static void Delete(Answer* answer)
	{
		_answers.Delete(answer);
	}

private:
	button _button;
	button _deleteIt;
	pin _pin;
	static Pool<Answer*> _answers;
};
