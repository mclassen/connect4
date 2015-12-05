#include <chrono>
#include <iostream>
#include <iomanip>
#include <limits>
#include <random>
#include <stdlib.h>
#include "constants.h"
#include "move_buffer.h"
#include "board.h"
#include "hash.h"

bool HashKey::randomValuesInit = false;
hashKeyType HashKey::RANDOM_VALUES[constants::MAX_MOVES * 2];

//void HashKey::init(const board& aBoard) {
//  for (sqType sq = 0; sq < constants::MAX_MOVES; sq++) {
//    switch (aBoard.GetSquare(sq)) {
//      case constants::WHITE:
//        makeMove(sq, constants::WHITE);
//        break;
//      case constants::BLACK:
//        makeMove(sq, constants::BLACK);
//        break;
//    }
//  }
//}

void HashKey::initRandomValues() {
  for (sqType sq = 0; sq < constants::MAX_MOVES * 2; sq++) {
    RANDOM_VALUES[sq] = createUnsignedRand(sq);
  }
  HashKey::randomValuesInit = true;
}

hashKeyType HashKey::createUnsignedRand(sqType sq) {

  std::default_random_engine generator;

  hashKeyType result = 0;

  const sqType seed = sq;
  
  generator.seed(seed);
  hashKeyType MAX_KEY = std::numeric_limits<hashKeyType>::max();
  std::uniform_int_distribution<hashKeyType> distribution(0, MAX_KEY);

  result = distribution(generator);
  //std::cerr << std::setw(12) << result;
  
  return result;
}
