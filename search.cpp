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
    std::cout << File(variation[0]) << " -";
    for (sqType i = 1; i < variation.size(); i++) {
      std::cout << " " << File(variation[i]);
    }
    std::cout << std::endl;
  }
}

void searchResult::SetVariation(const sqType& firstMove) {
  const sqType* pMove = &firstMove;
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

void search::InitSearch(distType depth, searchResult& result,
        SearchSettings& currentSettings) {
  searchedNodes = 0;

  valType best = constants::WORST_VALUE;

  valType alpha = constants::WORST_VALUE;
  valType beta = constants::BEST_VALUE;

  std::cout << "performing search..." << std::endl;
  startTime = time(NULL);

  if (depth > maxDepth) {
    std::cout << "Max depth reached! " << std::endl;
  }

  for (distType currDepth = 1; currDepth <= depth; currDepth++) {

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

int search::PerformSearch(distType distance, distType depth,
        valType alpha, valType beta) {
  // clear principle variation:
  principleVariations[depth][0] = constants::INVALID_MOVE;

  // the return value:
  valType best = constants::WORST_VALUE;

  // another node has been reached
  searchedNodes++;

    if (constants::USE_HASH && distance > 0)
    {
        valType hashValue = constants::INVALID;
        sqType hash_move = constants::INVALID_MOVE;
        hashValue =
                itsHash.lookup(
                        itsBoard.GetHashKey(),
                        distance,
                        itsBoard.GetSide(),
                        alpha,
                        beta,
                        hash_move
        );
        if (hashValue != constants::INVALID)
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

    const sqType legal_moves = buffer.numMoves;
    
    
    
    const distType minDist = 4u;
    distType cutoffDist;
    const sqType moveNum = itsBoard.GetNumberOfMove();

    if (distance >= minDist &&
            moveNum < constants::MAX_MOVES - 1)
    {
      switch (legal_moves)
      {
        case 7:
          cutoffDist = 4u;
          if (distance >= cutoffDist && distance % 2) {
            distance -= distance / 8;
          }
          break;
        case 6:
          cutoffDist = 8u;
          if (distance >= cutoffDist && distance % 2) {
            distance -= distance / 8;
          }
          break;
        case 5:
          cutoffDist = 16u;
          if (distance >= cutoffDist && distance % 2) {
            distance -= distance / 16;
          }
          break;
        case 4:
          cutoffDist = 24u;
          if (distance >= cutoffDist && distance % 2) {
            distance -= distance / 16;
          }
          break;
        case 3:
          cutoffDist = 8u;
          if (distance <= cutoffDist && distance % 2) {
            distance += distance / 4;
          }
          break;
        case 2:
          distance += distance / 4;
          break;
        case 1:
//          distance += distance / 8;
          break;
        default:
          break;
      }
//
//       const bool isEvenMove = moveNum % 2 == 0;
//       if (isEvenMove)
//       {
//       distance +=
//       moveNum <= constants::MAX_DEPTH/2 ? -1u : 0u;
//       }
    }
    distance += (itsBoard.GetNumberOfMove() + distance + 1) % 2;
    if (distance > constants::MAX_DEPTH - itsBoard.GetNumberOfMove()) {
      distance = constants::MAX_DEPTH - itsBoard.GetNumberOfMove();
    }

    for (int i = 0; i < legal_moves; i++) {
      sqType currMove = buffer.moves[i];
      
      // make a move:
      itsBoard.MakeMove(currMove);

      // the value in tempValue has to be negated, because the sides
      // have switched:
      valType tempValue = -PerformSearch(distance - 1, depth + 1,
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
            sqType sq = itsBoard.ConvertFileToMove(currMove);
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
  for (distType depth = 0; depth < maxDepth; depth++) {
    principleVariations[depth][0] = constants::INVALID_MOVE;
  }
}

void search::CopyVariation(const distType depth, const sqType move) {
  const sqType* pNextDepth_PV = &principleVariations[depth + 1][0];
  sqType* pCurrent_PV = &principleVariations[depth][0];

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
  const sqType* pPrincipleVariation = &principleVariations[0][0];
  while (*pPrincipleVariation != constants::INVALID_MOVE) {
    std::cout << " " << *pPrincipleVariation++;
  }
  std::cout << std::endl;
}

