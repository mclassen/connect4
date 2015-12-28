#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <time.h>
#include "constants.h"
#include "move_buffer.h"
#include "board.h"
#include "hash.h"
#include "search_settings.h"
#include "search.h"

void searchResult::Print() {
  std::cout << " val.: " << std::setw(5) << value;
  std::cout << " tvar.: ";
  PrintVariation();
}

void searchResult::PrintVariation() {
	if (variation.size() > 0) {
		const int firstMove = File(variation[0]);
		std::cout << firstMove << " -";
		for (size_t i = 1; i < variation.size(); i++) {
			const int move = File(variation[i]);
			std::cout << " " << move;
		}
		std::cout << std::endl;
	}
}

void searchResult::SetVariation(const square_t& firstMove) {
  const square_t* pMove = &firstMove;
  variation.clear();
  while (*pMove != constants::INVALID_MOVE) {
    variation.push_back(*pMove++);
  }
}

search::search(board& newBoard) :
itsBoard(newBoard),
itsHash(HashTable(constants::HASH_ENTRIES)),
maxDepth(constants::MAX_DEPTH),
searchedNodes(0)
{
  startTime = time(NULL);
  stopTime = time(NULL);
  ClearVariations();
}

void search::InitSearch(dist_t depth, searchResult& result,
        SearchSettings& currentSettings) {
  searchedNodes = 0;

  val_t best = constants::WORST_VALUE;

  val_t alpha = constants::WORST_VALUE;
  val_t beta = constants::BEST_VALUE;

  std::cout << "performing search..." << std::endl;
  startTime = time(NULL);

  if (depth > maxDepth) {
    std::cout << "Max depth reached! " << std::endl;
  }

  for (dist_t currDepth = 1; currDepth <= depth; currDepth++) {

    stopTime = time(NULL);
    if ((currentSettings.GetTimeLimit() > 0) &&
            (GetElapsedTime() > currentSettings.GetTimeLimit())) {
      std::cout << "Out of time! " << std::endl;
      break;
    }


    if (currDepth == 1) {
      alpha = constants::WORST_VALUE;
      beta = constants::BEST_VALUE;
    } else {
      alpha = best - (constants::WINDOW);
      beta = best + (constants::WINDOW);
    }
    // clear all principle variations:
    //      ClearVariations();
    best = PerformSearch(currDepth, 0, alpha, beta);


#ifdef _connect4_debug_
    std::cout << "value: " << result.GetValue() << std::endl;
    std::cout << "alpha: " << alpha << std::endl;
    std::cout << "beta: " << beta << std::endl;
#endif

    if (best <= alpha) {
      std::cout << "-" << std::flush;
      beta = alpha;
      alpha = constants::WORST_VALUE;

      // clear all principle variations:
      //ClearVariations();
    } else if (best >= beta) {
      std::cout << "+" << std::flush;
      alpha = beta;
      beta = constants::BEST_VALUE;

      // clear all principle variations:
      //ClearVariations();
      best = PerformSearch(currDepth, 0, alpha, beta);
    } else {
      std::cout << " " << std::flush;
    }
    //std::cout << "Depth: ";
    std::cout << std::setw(3) << currDepth << ":";
    result.SetValue(best);
    result.SetVariation(principleVariations[0][0]);
    // itsHash.store(
    // itsBoard,
    // itsBoard.ConvertFileToMove(result.GetMove()), 
    // currDepth,
    // itsBoard.GetSide(),
    // best,
    // alpha,
    // beta
    // );
    result.Print();
  }
  stopTime = time(NULL);
} // end InitSearch...

int search::PerformSearch(dist_t distance, dist_t depth,
        val_t alpha, val_t beta) {
  // clear principle variation:
  principleVariations[depth][0] = constants::INVALID_MOVE;

  // the return value:
  val_t best = constants::WORST_VALUE;

  // another node has been reached
  searchedNodes++;

    if (constants::USE_HASH && distance > 0)
    {
        val_t hashValue = constants::INVALID_VALUE;
        square_t hash_move = constants::INVALID_MOVE;
        hashValue =
                itsHash.lookup(
                        itsBoard.GetHashKey(),
                        distance,
                        itsBoard.GetSide(),
                        alpha,
                        beta,
                        hash_move
        );
        if (hashValue != constants::INVALID_VALUE)
        {
          best = hashValue;
          CopyVariation(depth, hash_move);
          //std::cout << "#" << std::flush;
          return best;
        }
    }

  // check for "mate":
  if (itsBoard.GetFourConnected() == true && itsBoard.GetNumberOfMove() < constants::MAX_MOVES) {
    //std::cout << "4-connected!\n";
    best = (constants::WORST_VALUE + distance);
    return best;
  } else
    // check for "stalemate":
    if (itsBoard.GetFourConnected() == false && itsBoard.GetNumberOfMove() >= constants::MAX_MOVES - 1) {
    best = constants::STALEMATE_VALUE;
    return best;
  } else
    
    // check if we are still within the searchtree:
    if (distance > 0) {
    MoveBuffer buffer;

    // generate all moves:
    itsBoard.GenerateMoves(buffer);

    const square_t legal_moves = buffer.numMoves;
    
    int fractionalDistance = distance * 8;
    
    const dist_t minDist = 2u;
    dist_t cutoffDist;
    const square_t moveNum = itsBoard.GetNumberOfMove();

    if (distance >= minDist &&
            moveNum < constants::MAX_MOVES)
    {
      switch (legal_moves)
      {
        case 7:
          cutoffDist = 48;
          if (fractionalDistance >= cutoffDist) {
            fractionalDistance -= 4;
          }
          break;
        case 6:
          cutoffDist = 40u;
          if (fractionalDistance >= cutoffDist) {
            fractionalDistance -= 3;
          }
          break;
        case 5:
          cutoffDist = 32u;
          if (fractionalDistance >= cutoffDist) {
            fractionalDistance -= 2;
          }
          break;
        case 4:
          cutoffDist = 24u;
          if (fractionalDistance >= cutoffDist) {
            fractionalDistance -= 1;
          }
          else if (fractionalDistance <= 16)
          {
            fractionalDistance += 1;
          }
          break;
        case 3:
          cutoffDist = 20u;
          if (fractionalDistance >= cutoffDist) {
            fractionalDistance -= 1;
          }
          else if (fractionalDistance <= 18)
          {
            fractionalDistance += 1;
          }
          break;
        case 2:
          cutoffDist = 16;
          if (fractionalDistance >= cutoffDist) {
            fractionalDistance -= 1;
          }
          else if (fractionalDistance <= 12)
          {
            fractionalDistance += 2;
          }
          break;
        case 1:
          fractionalDistance += 4;
          break;
        default:
          break;
      }
      distance = fractionalDistance / 8;
    }
    distance += (itsBoard.GetNumberOfMove() + distance + 1) % 2;
    if (distance > constants::MAX_DEPTH - itsBoard.GetNumberOfMove()) {
      distance = constants::MAX_DEPTH - itsBoard.GetNumberOfMove();
    }

    for (int i = 0; i < legal_moves; i++) {
      square_t currMove = buffer.moves[i];
      
      // make a move:
      itsBoard.MakeMove(currMove);

      // the value in tempValue has to be negated, because the sides
      // have switched:
      val_t tempValue = -PerformSearch(distance - 1, depth + 1,
              -beta, -alpha);
      // un-make move:
      itsBoard.UnmakeMove(currMove);

      // if the new value is greater than the previous one,
      // it becomes the new best value and the new alpha value:
      if (tempValue > best) {

        best = tempValue;
        alpha = tempValue;

        // update the principle variation:
        CopyVariation(depth, currMove);
        // cut-off:
        if (best >= beta) {
          if (constants::USE_HASH && !(itsBoard.GetFourConnected() == true || itsBoard.GetNumberOfMove() >= constants::MAX_MOVES)) {
            square_t sq = itsBoard.ConvertFileToMove(currMove);
            itsHash.store(
                    itsBoard.GetHashKey(),
                    distance,
                    itsBoard.GetSide(),
                    best,
                    alpha,
                    beta,
                    sq
                    );
          }
          return best;
        }
      }
    }
  } else {
    // a "leaf" of the search tree has been reached,
    // thus the position is evaluated:
    best = (Eval(itsBoard));
  }
#ifdef _connect4_debug_
  std::cout << "returning result..." << best << " on depth "
          << depth << std::endl;
#endif
  return best;
}

void search::ClearVariations() {
  for (dist_t depth = 0; depth < maxDepth; depth++) {
    principleVariations[depth][0] = constants::INVALID_MOVE;
  }
}

void search::CopyVariation(const dist_t depth, const square_t move) {
  const square_t* pNextDepth_PV = &principleVariations[depth + 1][0];
  square_t* pCurrent_PV = &principleVariations[depth][0];

  // copy first move:
  *pCurrent_PV++ = move;

  // copy continuation:
  while (*pNextDepth_PV != constants::INVALID_MOVE)
  {
    *pCurrent_PV++ = *pNextDepth_PV++;
  }

  // mark end of variation:
  *pCurrent_PV = constants::INVALID_MOVE;
}

void search::PrintVariation() const {
	const square_t* pPrincipleVariation = &principleVariations[0][0];
	while (*pPrincipleVariation != constants::INVALID_MOVE) {
		int move = *pPrincipleVariation++;
		std::cout << " " << move;
	}
	std::cout << std::endl;
}

