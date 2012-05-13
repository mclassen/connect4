#ifndef __search

class searchResult {
public:
  searchResult() : value(constants::WORST_VALUE) {}

  int GetValue() const { return value; }
  std::vector<int> GetVariation() const { return variation; }
  void clear() { 
    value = constants::WORST_VALUE;
    variation.clear();
  }
  void Print();
  void PrintVariation();

  void SetValue(int newValue) { value = newValue; }
  void SetVariation(std::vector<int>& newVariation) { 
    variation = newVariation; 
  }
  void SetVariation(const int& firstMove);

 private:
  int value;
  std::vector<int> variation;
};

class search {
public:
    search(board& newBoard);
    ~search() {};

    const long long GetSearchedNodes() const { return searchedNodes; }
    void InitSearch(unsigned int depth, searchResult& result, 
		    SearchSettings& itsSettings);
    void PrintVariation() const;
    const double GetElapsedTime() const { 
      return difftime(stopTime, startTime);
    }
    
private:
    board itsBoard;
    time_t startTime;
    time_t stopTime;

    // evtl. die dimensionen vertauschen...
    int principleVariations[constants::MAX_DEPTH][constants::MAX_DEPTH + 1];
    unsigned int maxDepth;
    long long searchedNodes;

    int PerformSearch(unsigned int distance, unsigned int depth,
                      int alpha, int beta);
    void ClearVariations();
    void CopyVariation(const unsigned int depth, const int move);

    // Everything from the point of view of WHITE:
    // if BLACK has an advantage, then the score
    // should be negative
    //
    int Eval(board& aBoard) const {
      int result = 0;
      const unsigned int numMove = aBoard.GetNumberOfMove();
      // const unsigned int unevenMoveBonus = 32;
      if(aBoard.GetSide() == constants::WHITE) {
        result = aBoard.GetThreats();
      } else {
        result = -aBoard.GetThreats();
      }
      // adjust for the advantage of white having the first move
      if(numMove > 2 && numMove < 20) {
        result += 5;
      }
      return result;
    }
};

#define __search
#endif
