#ifndef I_EVENT_HANDLER_H
#define I_EVENT_HANDLER_H
#include <SFML/Window/Event.hpp>

class IEventHandler
{
public:
	virtual void OnEvent(sf::Event const& ev) = 0;
};

#endif // ! I_EVENT_HANDLER_H
