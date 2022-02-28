#include "node.h"

bool node::fsm_initialized;
FSM node::pr_fsm;
std::set<node*> node::_all_nodes;
bool node::selected = false;
