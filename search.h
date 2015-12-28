
#include "constants.h"
#include "hash.h"

#ifndef __search

class searchResult {
public:
  searchResult() : value(constants::WORST_VALUE) {}

  val_t GetValue() const { return value; }
  std::vector<square_t> GetVariation() const { return variation; }
  square_t GetMove() const { return variation[0]; }
  void clear() { 
    value = constants::WORST_VALUE;
    variation.clear();
  }
  void Print();
  void PrintVariation();

  void SetValue(const val_t newValue) { value = newValue; }
  void SetVariation(std::vector<square_t>& newVariation) { 
    variation = newVariation; 
  }
  void SetVariation(const square_t& firstMove);

 private:
  val_t value;
  std::vector<square_t> variation;
};

class search {
public:
    search(board& newBoard);
    ~search() {};

    const unsigned long long GetSearchedNodes() const { return searchedNodes; }
    void InitSearch(dist_t depth, searchResult& result, 
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
    square_t principleVariations[constants::MAX_DEPTH][constants::MAX_DEPTH + 1];
    dist_t maxDepth;
    unsigned long long searchedNodes;

    int PerformSearch(dist_t distance, dist_t depth,
                      val_t alpha, val_t beta);
    void ClearVariations();
    void CopyVariation(const dist_t depth, const square_t move);

    // Everything from the point of view of WHITE:
    // if BLACK has an advantage, then the score
    // should be negative
    //
    val_t Eval(board& aBoard) const {
      val_t result = 0;
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
