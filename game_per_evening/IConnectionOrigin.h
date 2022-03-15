#pragma once
#include <SFML/System/Vector2.hpp>

class IConnectionOrigin
{
public:
	virtual sf::Vector2f ConnectionOrigin() const = 0;
};
