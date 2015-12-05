
#include "constants.h"
#include "hash.h"

#ifndef __search

class searchResult {
public:
  searchResult() : value(constants::WORST_VALUE) {}

  valType GetValue() const { return value; }
  std::vector<sqType> GetVariation() const { return variation; }
  sqType GetMove() const { return variation[0]; }
  void clear() { 
    value = constants::WORST_VALUE;
    variation.clear();
  }
  void Print();
  void PrintVariation();

  void SetValue(const valType newValue) { value = newValue; }
  void SetVariation(std::vector<sqType>& newVariation) { 
    variation = newVariation; 
  }
  void SetVariation(const sqType& firstMove);

 private:
  valType value;
  std::vector<sqType> variation;
};

class search {
public:
    search(board& newBoard);
    ~search() {};

    const unsigned long long GetSearchedNodes() const { return searchedNodes; }
    void InitSearch(distType depth, searchResult& result, 
		    SearchSettings& itsSettings);
    
    void PrintVariation() const;
    const double GetElapsedTime() const { 
      return difftime(stopTime, startTime);
    }
    
private:
    board itsBoard;
    HashTable itsHash;
    time_t startTime;
    time_t stopTime;

    // evtl. die dimensionen vertauschen...
    sqType principleVariations[constants::MAX_DEPTH][constants::MAX_DEPTH + 1];
    distType maxDepth;
    unsigned long long searchedNodes;

    int PerformSearch(distType distance, distType depth,
                      valType alpha, valType beta);
    void ClearVariations();
    void CopyVariation(const distType depth, const sqType move);

    // Everything from the point of view of WHITE:
    // if BLACK has an advantage, then the score
    // should be negative
    //
    valType Eval(board& aBoard) const {
      valType result = 0;
      // const unsigned int unevenMoveBonus = 32;
      if(aBoard.GetSide() == constants::WHITE) {
        result = aBoard.GetThreats();
      } else {
        result = -aBoard.GetThreats();
      }
      // adjust for the advantage of white having the first move
//      if(abs(result) < abs(constants::BEST_VALUE - constants::MATE_RANGE)) {
//        result += numMove % 2 == 0 ? 2 * constants::WINDOW : 0;
////			std::max(2U,
////				std::min(2 * constants::WINDOW,
////					(4U * constants::MAX_DEPTH) / numMove));
//      }
      return result;
    }
};

#define __search
#endif
