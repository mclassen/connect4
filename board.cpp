#define __quick_and_dirty

#include <iostream>
#include <vector>
#include "constants.h"
#include "move_buffer.h"
#include "board.h"

board::board() {
  // std::cout << "board constructed!" << std::endl;
  fourConnected = false;
  for(int x = 0; x < constants::MAX_MOVES; x++) {
    square[x] = constants::NONE;
  }
  side = constants::WHITE;
  numberOfMove = 0;
  numberOfPossibleMoves = constants::MAX_FILES;
  threats = 0;
  {
    for(int x = 0; x < constants::MAX_FILES; x++) {
          piecesInFile[x] = 0;
    }
  }
  {
    for(int x = 0; x < constants::NUM_THREATS; x++) {
      threatsBlack[x] = 0;
      threatsWhite[x] = 0;
    }
  }
}

void board::Display() {
  for(int y = constants::MAX_RANKS-1; y >= 0; y --) {
    for(int x = 0; x < constants::MAX_FILES; x++) {
      if(square[y * constants::MAX_FILES + x] == constants::WHITE)
        std::cout << " O ";
      else if(square[y * constants::MAX_FILES + x] == constants::BLACK)
        std::cout << " X ";
      else
        std::cout << " . ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl 
            <<((side == constants::WHITE) ? "O " : "X ") 
            << "to move. " << "Threats: " << GetThreats()
            << std::endl;
}


#ifdef __quick_and_dirty
void board::GenerateMoves(MoveBuffer& buffer) {
  buffer.numMoves = 0;
  numberOfPossibleMoves = 0;
  int i = 3;
  if(piecesInFile[i] < constants::MAX_RANKS) {
    buffer.moves[buffer.numMoves] = ConvertFileToMove(i);
    numberOfPossibleMoves++;
    buffer.numMoves++;
  }
  i = 2;
  if(piecesInFile[i] < constants::MAX_RANKS) {
    buffer.moves[buffer.numMoves] = ConvertFileToMove(i);
    numberOfPossibleMoves++;
    buffer.numMoves++;
  }
  i = 4;
  if(piecesInFile[i] < constants::MAX_RANKS) {
    buffer.moves[buffer.numMoves] = ConvertFileToMove(i);
    numberOfPossibleMoves++;
    buffer.numMoves++;
  }
  i = 1;
  if(piecesInFile[i] < constants::MAX_RANKS) {
    buffer.moves[buffer.numMoves] = ConvertFileToMove(i);
    numberOfPossibleMoves++;
    buffer.numMoves++;  
  }
  i = 5;
  if(piecesInFile[i] < constants::MAX_RANKS) {
    buffer.moves[buffer.numMoves] = ConvertFileToMove(i);
    numberOfPossibleMoves++;
    buffer.numMoves++;
  }
  i = 0;
  if(piecesInFile[i] < constants::MAX_RANKS) {
    buffer.moves[buffer.numMoves] = ConvertFileToMove(i);
    numberOfPossibleMoves++;
    buffer.numMoves++;
  }
  i = 6;
  if(piecesInFile[i] < constants::MAX_RANKS) {
    buffer.moves[buffer.numMoves] = ConvertFileToMove(i);
    numberOfPossibleMoves++;
    buffer.numMoves++;
  }
}
#endif

void board::UpdateThreats(int file, int rank, 
  int side, bool makingMove) {
  int i = 0;
  int threatIndex = constants::THREAT_TABLE[file][rank][i];
  int black, white;
  int tempResult;

  while(threatIndex != constants::INVALID) {
    if(makingMove) {
      if(side == constants::BLACK) {
        threatsBlack[threatIndex] += 1;
        // std::cout << " threatsBlack: " << threatsBlack[threatIndex];
      } else {
        threatsWhite[threatIndex] += 1;
        // std::cout << " threadsWhite: " << threatsWhite[threatIndex];
      }
    }
    black = threatsBlack[threatIndex];
    white = threatsWhite[threatIndex];
    tempResult = 0;
    switch(white) {
      case 4:
        fourConnected = true;
        break;
      case 3:
        tempResult += (black == 0) ? constants::THREE_VAL : -constants::THREE_VAL/3;
        break;
      case 2:
        if(black == 0)
          tempResult += constants::TWO_VAL;
        break;
      case 1:
          tempResult += (black == 3) ? constants::THREE_VAL/3 : 0;
        break;
      case 0:
        if(black == 4) {
          fourConnected = true;
        } else if(black == 3) {
          tempResult -= constants::THREE_VAL;
        } else if(black == 2) {
          tempResult -= constants::TWO_VAL;
        }
        break;
      default:
        std::cout << "case fell through in UpdateThreats()" << std::endl;
        break;
    }
	// tempResult *= 7;
	// tempResult += side * (constants::MAX_RANKS - rank);
	// tempResult /= 8;
	
	
    if(makingMove) {
      threats += tempResult;
    } else {
      threats -= tempResult;
      if(side == constants::BLACK) {
        threatsBlack[threatIndex] -= 1;
        // std::cout << " threatsBlack: " << threatsBlack[threatIndex];
      } else {
        threatsWhite[threatIndex] -= 1;
        // std::cout << " threadsWhite: " << threatsWhite[threatIndex];
      }
    }
    i++;
    threatIndex = constants::THREAT_TABLE[file][rank][i];
  }
  
/*   if(makingMove) {
    if(side == constants::WHITE) {
      threats += constants::CENTER_TABLE[file][rank] / 2;
    } else {
      threats -= constants::CENTER_TABLE[file][rank] / 2;
    }
  } else {
    if(side == constants::WHITE) {
      threats -= constants::CENTER_TABLE[file][rank] / 2;
    } else {
      threats += constants::CENTER_TABLE[file][rank] / 2;
    }
  } */
  
}

bool board::MoveIsValid(int file, bool makingMove) const {
  if(makingMove)
    return ((file >= 0 && file < constants::MAX_FILES) && 
            piecesInFile[file] < constants::MAX_RANKS);
  else
    return ((file >= 0 && file < constants::MAX_FILES) &&
            piecesInFile[file] > 0 && numberOfMove > 0);
}

