#include "Answer.h"

Pool<Answer*> Answer::_answers([]()->Answer* {return new Answer(); });
