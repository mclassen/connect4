#include <iostream>
#include <stdlib.h>
#include "constants.h"
#include "move_buffer.h"
#include "board.h"
#include "hash.h"

bool HashKey::randomValuesInit = false;
unsigned HashKey::RANDOM_VALUES[constants::MAX_MOVES][2];

HashKey::HashKey() :
  key(0) {
  if(randomValuesInit == false)
    InitRandomValues();
}

void HashKey::InitPosition(const int board[constants::MAX_MOVES]) {
  key = 1;
  for(int sq = 0; sq < constants::MAX_MOVES; sq++) {
    switch(board[sq]) {
    case constants::WHITE:
      MoveMade(sq, constants::WHITE);
      break;
    case constants::BLACK:
      MoveMade(sq, constants::BLACK);
      break;
    }
  }
}

void HashKey::InitRandomValues() {
  for(int sq = 0; sq < constants::MAX_MOVES; sq++) {
    for(int side = 0; side < 2; side++) {
      RANDOM_VALUES[sq][side] = CreateUnsignedRand();
    }
  }
}

unsigned HashKey::CreateUnsignedRand() {
  unsigned result = 0;
  for(unsigned byte = 0; byte < sizeof(unsigned); byte++) {
    const unsigned randomByte = abs(rand()) % 256;
    result |= (randomByte << (byte * 8));
  }
  return result;
}

inline void HashKey::MoveMade(int move, int side) {
  if(side == constants::WHITE)
    key ^= RANDOM_VALUES[move][0];
  else
    key ^= RANDOM_VALUES[move][1];
}
