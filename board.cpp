#define __quick_and_dirty

#include <iostream>
#include <vector>
#include <cmath>
#include "constants.h"
#include "move_buffer.h"
#include "board.h"

board::board() {
  // std::cout << "board constructed!" << std::endl;
  fourConnected = false;
  for(square_t x = 0; x < constants::MAX_MOVES; x++) {
    square[x] = constants::NONE;
  }
  side = constants::WHITE;
  numberOfMove = 0;
  numberOfPossibleMoves = constants::MAX_FILES;
  threats = 0;
  {
    for(square_t x = 0; x < constants::MAX_FILES; x++) {
          piecesInFile[x] = 0;
    }
  }
  {
    for(square_t x = 0; x < constants::NUM_THREATS; x++) {
      threatsBlack[x] = 0;
      threatsWhite[x] = 0;
    }
  }
}

void board::Display() {
  for(int y = constants::MAX_RANKS-1; y >= 0; y--) {
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
            << ", HashKey: " << GetHashKey()
            << std::endl;
}


#ifdef __quick_and_dirty
void board::GenerateMoves(MoveBuffer& buffer) {
  buffer.numMoves = 0;
  numberOfPossibleMoves = 0;
  if (fourConnected)
  {
    return;
  }
  
  square_t i = 3;
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

void board::UpdateThreats(square_t file, square_t rank, 
  side_t side, bool makingMove) {
  square_t i = 0;
  val_t threatIndex = constants::THREAT_TABLE[file][rank][i];
  side_t black, white;
  val_t tempResult;
  

  while(threatIndex != constants::INVALID_VALUE) {
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
	// const int THREE_VAL_ADJ = constants::RANK_THREE_VAL(rank, side);
	// const int TWO_VAL_ADJ = constants::RANK_TWO_VAL(rank, side);
    switch(white) {
      case 4:
        fourConnected = true;
        break;
      case 3:
        tempResult += (black == 0) ? constants::RANK_THREE_VAL(rank, side) : 0;
        break;
      case 2:
        if(black == 0)
          tempResult += constants::RANK_TWO_VAL(rank, side);
        break;
      case 1:
          //tempResult += (black == 3) ? 1 : 0;
        break;
      case 0:
        if(black == 4) {
          fourConnected = true;
        } else if(black == 3) {
          tempResult -= constants::RANK_THREE_VAL(rank, side);
        } else if(black == 2) {
          tempResult -= constants::RANK_TWO_VAL(rank, side);
        }
        break;
      default:
        std::cerr << "case fell through in UpdateThreats()" << std::endl;
        std::cerr << "white: " << white << ", black: " << black << std::endl;
        break;
    }

    if (makingMove) {
      threats += tempResult;
    } else {
      threats -= tempResult;
    }
	
	const val_t CENTER_TABLE_VAL =
          (5 * constants::CENTER_TABLE[file][rank]) / 8;
	if(makingMove) {
		if(side == constants::WHITE) {
			threats += CENTER_TABLE_VAL;
		} else {
			threats -= CENTER_TABLE_VAL;
		}
	} else {
		if(side == constants::WHITE) {
			threats -= CENTER_TABLE_VAL;
		} else {
			threats += CENTER_TABLE_VAL;
		}
	}
	  
	if (std::abs(threats) > 1 &&
	    std::abs(threats) < (constants::BEST_VALUE - constants::MATE_RANGE))
	{
		// const unsigned numMovesLeftFactor =
		    // 10u * log(constants::MAX_DEPTH - GetNumberOfMove() + 10);
		// threats = (10u * threats) / numMovesLeftFactor;
		// const unsigned stage = GetNumberOfMove() / 4u;
		// threats =
			// (stage <= 2) ? (3 * threats) / 4 :
				// (stage <= 4) ? (4 * threats) / 5 :
					// (stage <= 6) ? (5 * threats) / 6:
						// (stage <= 8) ? threats :
							// (9 * threats) / 8;
		
//		const int OPEN_FILES = GetNumberOfPossibleMoves();
//		switch(OPEN_FILES)
//		{
//			case 7:
//				threats = (2 * threats) / 3;
//			break;
//			case 6:
//				threats = (3 * threats) / 4;
//			break;
//			case 5:
//				threats = (4 * threats) / 5;
//			break;
//			case 4:
//				//threats = (7 * threats) / 8;
//			break;
//			case 3:
//				threats = (6 * threats) / 5;
//			break;
//			case 2:
//				threats = (5 * threats) / 4;
//			break;
//			case 1:
//				threats = (4 * threats) / 3;
//			break;
//			default:
//				std::cerr << "case fell through in UpdateThreats(): switch(OPEN_FILES)" << 
//					std::endl;	
//			break;
//		}
		
	}
	// if (std::abs(threats) < (constants::BEST_VALUE - constants::MATE_RANGE))
	// {
		// threats /= 2;
	// }
	
	if(!makingMove) {
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
}

bool board::MoveIsValid(square_t file, bool makingMove) const {
  if(makingMove)
    return ((file >= 0 && file < constants::MAX_FILES) && 
            piecesInFile[file] < constants::MAX_RANKS);
  else
    return ((file >= 0 && file < constants::MAX_FILES) &&
            piecesInFile[file] > 0 && numberOfMove > 0);
}

