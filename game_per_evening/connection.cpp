#include "connection.h"

Pool<connection*> connection::_all_connection([]()->connection* { return new connection(); });
