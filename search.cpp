#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include "constants.h"
#include "move_buffer.h"
#include "board.h"
#include "hash.h"
#include "search_settings.h"
#include "search.h"

void searchResult::Print() {
  std::cout << "val.: " << value;
  std::cout << " var.: ";
  PrintVariation();
}

void searchResult::PrintVariation() {
  if (variation.size() > 0) {
    std::cout << File(variation[0]) << " -";
    for (unsigned int i = 1; i < variation.size(); i++) {
      std::cout << "  " << File(variation[i]);
    }
    std::cout << std::endl;
  }
}

void searchResult::SetVariation(const int& firstMove) {
  const int* pMove = &firstMove;
  variation.clear();
  while (*pMove != constants::INVALID) {
    variation.push_back(*pMove++);
  }
}

search::search(board& newBoard) :
itsBoard(newBoard),
itsHash(constants::HASH_SIZE),
maxDepth(constants::MAX_DEPTH),
searchedNodes(0) {
  startTime = time(NULL);
  stopTime = time(NULL);
  ClearVariations();
}

void search::InitSearch(unsigned int depth, searchResult& result,
        SearchSettings& currentSettings) {
  searchedNodes = 0;

  int best = constants::WORST_VALUE;

  int alpha = constants::WORST_VALUE;
  int beta = constants::BEST_VALUE;

  std::cout << "performing search..." << std::endl;
  startTime = time(NULL);

  if (depth > maxDepth) {
    std::cout << "Max depth reached! " << std::endl;
  }

  for (unsigned int currDepth = 1; currDepth <= depth; currDepth++) {

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
    }
    best = PerformSearch(currDepth, 0, alpha, beta);
    std::cout << "Depth: ";
    std::cout << currDepth << " ";
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

int search::PerformSearch(unsigned int distance, unsigned int depth,
        int alpha, int beta) {
  // clear principle variation:
  principleVariations[depth][0] = constants::INVALID;

  // the return value:
  int best = constants::WORST_VALUE;

  // another node has been reached
  searchedNodes++;

    if (distance > 0)
    {
        int hashValue = constants::INVALID;
        hashValue =
                itsHash.lookup(
                        itsBoard,
                        distance,
                        itsBoard.GetSide(),
                        alpha,
                        beta
        );
        if (hashValue != constants::INVALID)
        {
          best = hashValue;
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

    const int legal_moves = buffer.numMoves;
    
    
    
    const unsigned minDist = 6u;
    unsigned cutoffDist = 8u;
    const unsigned moveNum = itsBoard.GetNumberOfMove();

    if (distance >= minDist &&
            moveNum < constants::MAX_MOVES - 1) {
      switch (legal_moves) {
        case 7:
          cutoffDist = 6u;
          if (distance >= cutoffDist && distance % 3u > 0) {
            distance -= 1u;
          }
          break;
        case 6:
          cutoffDist = 8u;
          if (distance >= cutoffDist && distance % 2u == 0) {
            distance -= 1u;
          }
          break;
        case 5:
          cutoffDist = 16u;
          if (distance >= cutoffDist && distance % 2u == 0) {
            distance -= 1u;
          }
          break;
        case 4:
          cutoffDist = 21u;
          if (distance >= cutoffDist && distance % 3u == 0) {
            distance -= 1u;
          }
          break;
        case 3:
          cutoffDist = 12u;
          if (distance <= cutoffDist && distance % 3u == 0) {
            distance += 1u;
          }
          break;
        case 2:
          cutoffDist = 15u;
          if (distance <= cutoffDist && distance % 2u == 0) {
            distance += 1u;
          }
          break;
        case 1:
          if (distance % 2u == 0) {
            distance += 1u;
          }
          break;
        default:
          break;
      }

      distance += (itsBoard.GetNumberOfMove() + distance + 1) % 2;

       const bool isEvenMove = moveNum % 2 == 0;
       if (isEvenMove)
       {
       distance +=
       moveNum <= constants::MAX_DEPTH/2 ? -1u : 0u;
       }
    }
    
    if (distance > constants::MAX_DEPTH - itsBoard.GetNumberOfMove()) {
      distance = constants::MAX_DEPTH - itsBoard.GetNumberOfMove();
    }

    for (int i = 0; i < buffer.numMoves; i++) {
      // make a move:
      itsBoard.MakeMove(buffer.moves[i]);

      // the value in tempValue has to be negated, because the sides
      // have switched:
      int tempValue = -PerformSearch(distance - 1, depth + 1,
              -beta, -alpha);
      // un-make move:
      itsBoard.UnmakeMove(buffer.moves[i]);

      // if the new value is greater than the previous one,
      // it becomes the new best value and the new alpha value:
      if (tempValue > best) {

        best = tempValue;
        alpha = tempValue;

        // update the principle variation:
        CopyVariation(depth, buffer.moves[i]);
        // cut-off:
        if (best >= beta) {
          if (!(itsBoard.GetFourConnected() == true || itsBoard.GetNumberOfMove() >= constants::MAX_MOVES)) {
            itsHash.store(
                    itsBoard,
                    distance,
                    itsBoard.GetSide(),
                    best,
                    alpha,
                    beta
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
  for (unsigned int depth = 0; depth < maxDepth; depth++) {
    principleVariations[depth][0] = constants::INVALID;
  }
}

void search::CopyVariation(const unsigned int depth, const int move) {
  const int* pNextDepth_PV = &principleVariations[depth + 1][0];
  int* pCurrent_PV = &principleVariations[depth][0];

  // copy first move:
  *pCurrent_PV++ = move;

  // copy continuation:
  while (*pNextDepth_PV != constants::INVALID)
    *pCurrent_PV++ = *pNextDepth_PV++;

  // mark end of variation:
  *pCurrent_PV = constants::INVALID;
}

void search::PrintVariation() const {
  const int* pPrincipleVariation = &principleVariations[0][0];
  while (*pPrincipleVariation != constants::INVALID) {
    std::cout << "  " << *pPrincipleVariation++;
  }
  std::cout << std::endl;
}

