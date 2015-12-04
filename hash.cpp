#include <iostream>
#include <stdlib.h>
#include "constants.h"
#include "move_buffer.h"
#include "board.h"
#include "hash.h"

bool HashKey::randomValuesInit = false;
unsigned long HashKey::RANDOM_VALUES[constants::MAX_MOVES][2];

HashKey::HashKey() :
  key(0) {
  if(randomValuesInit == false)
    initRandomValues();
}

void HashKey::init(const board& aBoard) {
  key = 1;
  for(int sq = 0; sq < constants::MAX_MOVES; sq++) {
    switch(aBoard.GetSquare(sq))
	{
		case constants::WHITE:
			makeMove(sq, constants::WHITE);
			break;
		case constants::BLACK:
			makeMove(sq, constants::BLACK);
			break;
    }
  }
}

void HashKey::initRandomValues() {
	for(int sq = 0; sq < constants::MAX_MOVES; sq++) {
		RANDOM_VALUES[sq][0] = createUnsignedRand();
		RANDOM_VALUES[sq][1] = createUnsignedRand();
	}
	HashKey::randomValuesInit = true;
}

unsigned long HashKey::createUnsignedRand() {
  unsigned long result = 0;
  for(unsigned long byte = 0; byte < sizeof(unsigned long); byte++) {
    const unsigned long randomByte = abs(rand()) % 256;
    result |= (randomByte << (byte * 8));
  }
  return result;
}
