#include "FSM.h"
#include <fstream>
#include <iostream>

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


// // ------------ TEST --------------
//
//FSM fsm;
//
//bool mouse_pressed = false;
//bool escape_pressed = false;
//
//fsm.AddTransition("A", "p", "B", [&mouse_pressed] { return mouse_pressed; });
//fsm.AddTransition("A", "q", "C", [] { return false; });
//fsm.AddTransition("B", "p", "C", [&mouse_pressed] { return !mouse_pressed; });
//fsm.AddTransition("C", "r", "A", [&escape_pressed] { return escape_pressed; });
//
//fsm.WriteToFile("test.txt");
//fsm.LoadFromFile("test.txt");
//
//fsm.SetState("A");
//
//if (fsm.GetState() != "A") std::cerr << "ERROR! first state isn't A\n";
//else std::cout << "set state A passed :)\n";
//
//fsm.TryTransit();
//if (fsm.GetState() != "A") std::cerr << "ERROR! transit from A without any reason\n";
//else std::cout << "try transit without any reason passed :)\n";
//
//mouse_pressed = true;
//fsm.TryTransit();
//if (fsm.GetState() != "B") std::cerr << "ERROR! p doesn't leads from A to B\n";
//else std::cout << "p leads from A to B passed :)\n";
//
//mouse_pressed = false;
//fsm.TryTransit();
//if (fsm.GetState() != "B") std::cerr << "ERROR! transit from B without any reason\n";
//else std::cout << "try transit without any reason passed :)\n";
//
//mouse_pressed = true;
//fsm.TryTransit();
//if (fsm.GetState() != "C") std::cerr << "ERROR! p doesn't leads from B to C\n";
//else std::cout << "p leads from B to C passed :)\n";
//
//mouse_pressed = false;
//fsm.TryTransit();
//if (fsm.GetState() != "C") std::cerr << "ERROR! transit from C without any reason when mouse isn't pressed\n";
//else std::cout << "try transit without any reason passed :)\n";
//
//mouse_pressed = true;
//fsm.TryTransit();
//if (fsm.GetState() != "C") std::cerr << "ERROR! transit from C without any reason when mouse_pressed\n";
//else std::cout << "try transit without any reason passed :)\n";
//
//escape_pressed = true;
//fsm.TryTransit();
//if (fsm.GetState() != "A") std::cerr << "ERROR! r doesn't leads from C to A\n";
//else std::cout << "r leads from C to A passed :)\n";

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

void FSM::OpenTransitionIf(std::vector<std::string> const& transition_names, std::function<bool()> p)
{
	bool found = false;
	for (int i = 0; i < transitions.size(); ++i)
	{
		for (auto transition : transition_names)
		{
			if (transitions[i] == transition)
			{
				OpenTransition(i, p);
				found = false;
			}
		}
		if (found)
		{
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
	for (int const tr : state_to_transitions[currentState])
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
	auto&& key = std::make_pair(currentState, transition);

	if (current_transition_to_state.count(key))
	{
		currentState = current_transition_to_state[key];
	}
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
		state_to_transitions[from] = {};
	}
	if (to == -1)
	{
		to = states.size();
		states.push_back(end);
		state_to_transitions[to] = {};
		//state_to_transition_to_state.emplace_back();
		//transition_to_state.emplace_back();
	}
	if (tr == -1)
	{
		tr = transitions.size();
		transitions.push_back(transition);
		check_transition.push_back(opened);
		//transition_to_state[from][tr] = to;
	}
	state_to_transitions[from].emplace(tr);
	current_transition_to_state[{from, tr}] = to;
	//bool newTransition = true;
	//for (auto& t : state_to_transition_to_state[from])
	//{
	//	if (t.second == tr)
	//	{
	//		newTransition = false;
	//		break;
	//	}
	//}
	//if (newTransition)
	//{
		//state_to_transition_to_state[from].push_back(tr);
	//}
	//transition_to_state[from][tr] = to;
}

void FSM::LoadFromFile(std::string const& filename)
{
	std::ifstream file(filename);
	std::string buff;
	int n;
	int n_2;
	int n_3;
	int sz;
	int sz_2;
	file >> currentState;
	states.reserve((file >> sz, sz));
	for (int i = 0; i < sz; ++i) states.push_back((file >> buff, buff));
	transitions.reserve((file >> sz, sz));
	check_transition.resize(sz, False);
	for (int i = 0; i < sz; ++i) transitions.push_back((file >> buff, buff));
	file >> sz;
	for (int i = 0; i < sz; ++i)
	{
		file >> n;
		state_to_transitions[n] = {};
		file >> sz_2;
		for (int j = 0; j < sz_2; ++j) state_to_transitions[n].insert((file >> n_2, n_2));
	}
	file >> sz;
	for (int i = 0; i < sz; ++i)
	{
		file >> n;
		file >> n_2;
		file >> n_3;
		current_transition_to_state[{n, n_2}] = n_3;
	}

	std::cout << "stop read\n";
}

void FSM::WriteToFile(std::string const& filename)
{
	std::ofstream file(filename);
	file << currentState << " ";
	file << states.size() << " ";
	for (auto& state : states) file << state << " ";
	file << transitions.size() << " ";
	for (auto& transition : transitions) file << transition << " ";
	file << state_to_transitions.size() << " ";
	for (auto& trs : state_to_transitions)
	{
		file << trs.first << " " << trs.second.size() << " ";
		for (int const state : trs.second) file << state << " ";
	}
	file << current_transition_to_state.size() << " ";
	for (const auto& tr : current_transition_to_state)
	{
		file << tr.first.first << " " << tr.first.second << " " << tr.second << " ";
	}

	std::cout << "stop write\n";
}

void FSM::GenerateCodeToFile(std::string const& filename) const
{
	std::ofstream file(filename, std::ios_base::app);
	for (auto const& transition : transitions)
	{
		file << "\t\t\tpr_fsm.OpenTransitionIf(\"" << transition << "\", []() {\n\t\t\t\treturn false;\n\t\t\t});\n";
	}
	file << '\n';
	bool first = true;
	for (auto const& state : states)
	{
		file << (first ? "" : "else ") << "if (state == \"" << state << "\") {}" << "\n";
		if (first) first = false;
	}
	file << '\n';
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
