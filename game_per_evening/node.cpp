#include "node.h"

bool node::fsm_initialized;
FSM node::pr_fsm;
Pool<node*> node::_all_nodes{ []()->node* {return new node({ 100.f,100.f }, Input::GetMousePosition()); } };
sf::String node::_prev_name = "untitled";
node* node::_dragged_node{ nullptr };
node* node::_overlapped_node{ nullptr };



//pr_fsm.AddTransition("idle", "click", "drag");
//pr_fsm.AddTransition("selected", "click", "drag");
//pr_fsm.AddTransition("idle", "in_selection_rect", "select_many");
//pr_fsm.AddTransition("drag", "release", "selected");
//pr_fsm.AddTransition("selected", "deny", "idle");
//pr_fsm.AddTransition("selected", "start_edit", "edit_name");
//pr_fsm.AddTransition("selected", "remove", "dead");
//pr_fsm.AddTransition("edit_name", "enter", "save_new_name");
//pr_fsm.AddTransition("edit_name", "deny", "selected");
//pr_fsm.AddTransition("save_new_name", "true", "selected");
//pr_fsm.AddTransition("select_many", "deny", "idle");
//pr_fsm.AddTransition("select_many", "remove", "dead");
//pr_fsm.AddTransition("select_many", "click", "drag_many");
//pr_fsm.AddTransition("drag_many", "release", "select_many");
//pr_fsm.WriteToFile("node.fsm");
