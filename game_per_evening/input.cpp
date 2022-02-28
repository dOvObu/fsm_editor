#include "input.h"

sf::RenderWindow* Input::_window{ nullptr };
std::map<sf::Event::EventType, std::set<IEventHandler*>> Input::_eventSubs;
