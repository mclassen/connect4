#ifndef _MoveBuffer_h_

#include "constants.h"

struct MoveBuffer {
  sqType moves[constants::MAX_FILES];
  sqType numMoves;
};

#define _MoveBuffer_h_
#endif
