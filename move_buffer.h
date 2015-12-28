#ifndef _MoveBuffer_h_

#include "constants.h"

struct MoveBuffer {
  square_t moves[constants::MAX_FILES];
  square_t numMoves;
};

#define _MoveBuffer_h_
#endif
