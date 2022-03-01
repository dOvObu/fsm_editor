#include "node.h"

bool node::fsm_initialized;
FSM node::pr_fsm;
std::set<node*> node::_all_nodes;
bool node::_is_showing_selected = false;
bool node::_pass_one_character_on_edit_name = false;
sf::String node::_prev_name = "untitled";
