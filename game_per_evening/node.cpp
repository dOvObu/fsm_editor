#include "node.h"

bool node::fsm_initialized;
FSM node::proto_fsm;
std::set<node*> node::_all_nodes;
