#pragma once

#include "Pool.h"
#include "IConnectionOrigin.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include "input.h"
#define VERTS_COUNT 30

class connection : public sf::Drawable
{
public:
	connection()
	{
		_verts[0] = sf::Vertex();
		_verts[1] = sf::Vertex();
	}
	
	void init()
	{
		_from = nullptr;
		_to = nullptr;
	}
	
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(_verts, VERTS_COUNT, sf::Lines);
	}

	void update()
	{
		if (_from != nullptr) _verts[0].position = _from->ConnectionOrigin();
		
		_verts[VERTS_COUNT - 1].position = _to != nullptr
			? _to->ConnectionOrigin()
			: Input::GetMousePosition();

		sf::Vector2f const& b0 = _verts[0].position;
		sf::Vector2f const& b3 = _verts[VERTS_COUNT - 1].position;
		sf::Vector2f const& b1 = b0 + sf::Vector2f(20.f, 0.f);
		sf::Vector2f const& b2 = b3 + sf::Vector2f(0.f, -20.f);

		for (int i = 1; i < VERTS_COUNT - 1; ++i)
		{
			float const t = i / VERTS_COUNT;
			float const tt = t * t;
			float const dt = 1.f - t;
			float const dtt = dt * dt;

			_verts[i].position = dt * dtt * b0 + 3.f * t * dtt * b1 + 3.f * tt * dt * b2 + t * tt * b3;
		}
	}

	IConnectionOrigin* From() const { return _from; }
	void From(IConnectionOrigin* from) { _from = from; }

	IConnectionOrigin* To() const { return _to; }
	void To(IConnectionOrigin* to) { _to = to; }

	static connection* Create()
	{
		return _all_connection.Create();
	}
	
	static void Delete(connection* connection)
	{
		_all_connection.Delete(connection);
	}

	static void UpdateAll()
	{
		_all_connection.Foreach([](connection* item) { item->update(); });
	}

	static void DrawAll(sf::RenderWindow& window)
	{
		_all_connection.Foreach([&window](connection* item) { window.draw(*item); });
	}

private:
	IConnectionOrigin* _from{ nullptr };
	IConnectionOrigin* _to{ nullptr };
	sf::Vertex _verts[VERTS_COUNT];
	
	static Pool<connection*> _all_connection;
};

#undef VERTS_COUNT
