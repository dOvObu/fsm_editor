#include "FSM.h"

std::function<bool()> FSM::False = [] { return false; };


//// init
//sf::RectangleShape shape(sf::Vector2f(100, 100));
//shape.setPosition(100, 100);

//FSM f;
//f.LoadFromFile("./Draggable.txt");
//sf::Vector2f offset;

//f.OpenTransitionIf("Click", [&shape, &window, &offset]{
//		const sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
//		offset = shape.getPosition() - mouse_position;
//		return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && shape.getGlobalBounds().contains(mouse_position);
//	});
//f.OpenTransitionIf("Release", []{
//		return !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
//	});

//// update
//f.TryTransit();

//if (f.GetState() == "Drag")
//{
//	shape.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)) + offset);
//}

//window.draw(shape);

void FSM::SetState(std::string const& state)
{
	for (int i = 0; i < states.size(); ++i) if (states[i] == state)
	{
		currentState = i;
		break;
	}
}

std::string FSM::GetState()
{
	return states[currentState];
}

int FSM::GetStateIndex() const
{
	return currentState;
}

void FSM::OpenTransitionIf(std::string const& transition, std::function<bool()> p)
{
	for (int i = 0; i < transitions.size(); ++i)
	{
		if (transitions[i] == transition)
		{
			OpenTransition(i, p);
			break;
		}
	}
}

void FSM::OpenTransition(int transitionIndex, bool const open)
{
	check_transition[transitionIndex] = [open] { return open; };
}

void FSM::OpenTransition(int transitionIndex, std::function<bool()> p)
{
	check_transition[transitionIndex] = p;
}

bool FSM::TryTransit()
{
	bool result = false;
	for (int tr : state_to_transition[currentState])
	{
		if (check_transition[tr]())
		{
			Transit(tr);
			result = true;
			break;
		}
	}
	return result;
}

void FSM::Transit(int transition)
{
	currentState = transition_to_state[transition];
}

void FSM::AddTransition(std::string const& start, std::string const& transition, std::string const& end,
	const std::function<bool()>& opened)
{
	int from = GetStateIndex(start);
	int tr = GetTransitionIndex(transition);
	int to = GetStateIndex(end);
	if (from == -1)
	{
		from = states.size();
		states.push_back(start);
		state_to_transition.emplace_back();
	}
	if (to == -1)
	{
		to = states.size();
		states.push_back(end);
		state_to_transition.emplace_back();
	}
	if (tr == -1)
	{
		tr = transitions.size();
		transitions.push_back(transition);
		check_transition.push_back(opened);
		transition_to_state.push_back(to);
	}
	bool newTransition = true;
	for (int t : state_to_transition[from])
	{
		if (t == tr)
		{
			newTransition = false;
			break;
		}
	}
	if (newTransition)
	{
		state_to_transition[from].push_back(tr);
	}
	transition_to_state[tr] = to;
}

void FSM::LoadFromFile(std::string const& filename)
{
	std::ifstream file(filename);
	std::string buff;
	int n;
	int sz;
	int sz_2;
	file >> currentState;
	file >> sz;
	states.reserve(sz);
	state_to_transition.resize(sz, {});
	for (int i = 0; i < sz; ++i)
	{
		file >> buff;
		states.push_back(buff);
		file >> sz_2;
		state_to_transition[i].reserve(sz_2);
		for (int j = 0; j < sz_2; ++j)
		{
			file >> n;
			state_to_transition[i].push_back(n);
		}
	}
	file >> sz;
	transitions.reserve(sz);
	transition_to_state.reserve(sz);
	check_transition.resize(sz, False);
	for (int i = 0; i < sz; ++i)
	{
		file >> buff;
		transitions.push_back(buff);
		file >> n;
		transition_to_state.push_back(n);
	}
}

void FSM::WriteToFile(std::string const& filename) const
{
	std::ofstream file(filename);
	file << currentState << " ";
	file << states.size() << " ";
	for (int i = 0; i < states.size(); ++i)
	{
		file << states[i] << " ";
		file << state_to_transition[i].size() << " ";
		for (int j = 0; j < state_to_transition[i].size(); ++j)
		{
			file << state_to_transition[i][j] << " ";
		}
	}
	file << transitions.size() << " ";
	for (int i = 0; i < transitions.size(); ++i)
	{
		file << transitions[i] << " ";
		file << transition_to_state[i] << " ";
	}
}

int FSM::GetStateIndex(std::string const& state) const
{
	return GetIndex(states, state);
}

int FSM::GetTransitionIndex(std::string const& transition) const
{
	return GetIndex(transitions, transition);
}

int FSM::GetIndex(std::vector<std::string> const& strs, std::string const& str)
{
	int res = -1;
	for (int i = 0; i < strs.size(); ++i)
	{
		if (strs[i] == str)
		{
			res = i;
			break;
		}
	}
	return res;
}
