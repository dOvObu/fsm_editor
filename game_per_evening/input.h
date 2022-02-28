#ifndef INPUT_H
#define INPUT_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Mouse.hpp>
#include <map>
#include <set>
#include <SFML/Window/Event.hpp>

#include "IEventHandler.h"

class Input
{
public:
	static void Init(sf::RenderWindow* window)
	{
		_window = window;
	}

	static sf::Vector2f GetMousePosition()
	{
		return _window->mapPixelToCoords(sf::Mouse::getPosition(*_window));
	}
	
	static void SubscribeOn(sf::Event::EventType type, IEventHandler* handler)
	{
		_eventSubs[type].insert(handler);
	}

	static void SubscribeOn(sf::Event::EventType type, IEventHandler& handler)
	{
		_eventSubs[type].insert(&handler);
	}

	static void UnsubscribeFrom(sf::Event::EventType type, IEventHandler& handler)
	{
		_eventSubs[type].erase(&handler);
	}

	static void Unsubscribe(IEventHandler* p_handler)
	{
		Unsubscribe(*p_handler);
	}

	static void Unsubscribe(IEventHandler& handler)
	{
		for (auto& subscribers : _eventSubs)
		{
			if (subscribers.second.count(&handler))
			{
				subscribers.second.erase(&handler);
			}
		}
	}

	static void NotifySubsOn(sf::Event const& e)
	{
		if (_eventSubs.count(e.type))
		{
			for (auto eventSub : _eventSubs[e.type])
			{
				eventSub->OnEvent(e);
			}
		}
	}

private:
	static sf::RenderWindow* _window;
	static std::map<sf::Event::EventType, std::set<IEventHandler*>> _eventSubs;
};


#endif // ! INPUT_H
