#ifndef FSM_H
#define FSM_H
#include <fstream>
#include <functional>
#include <string>
#include <vector>

class FSM
{
public:
	void SetState(std::string const& state);

	std::string GetState();
	int GetStateIndex() const;

	void OpenTransitionIf(std::string const& transition, std::function<bool()> p);
	void OpenTransition(int transitionIndex, bool const open = true);
	void OpenTransition(int transitionIndex, std::function<bool()> p);

	bool TryTransit();
	void Transit(int transition);
	void AddTransition(std::string const& start, std::string const& transition, std::string const& end, const std::function<bool()>& opened = False);

	void LoadFromFile(std::string const& filename);
	void WriteToFile(std::string const& filename) const;

	int GetStateIndex(std::string const& state) const;
	int GetTransitionIndex(std::string const& transition) const;

private:
	static int GetIndex(std::vector<std::string> const& strs, std::string const& str);
	int currentState = 0;
	static std::function<bool()> False;
	std::vector<std::vector<int>> state_to_transition;
	std::vector<int> transition_to_state;
	std::vector<std::function<bool()>> check_transition;
	std::vector<std::string> transitions;
	std::vector<std::string> states;
};


#endif
