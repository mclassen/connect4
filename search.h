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

    const long GetSearchedNodes() const { return searchedNodes; }
    void InitSearch(int depth, searchResult& result, 
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
    int maxDepth;
    long searchedNodes;

    int PerformSearch(int distance, int depth,
                      int alpha, int beta);
    void ClearVariations();
    void CopyVariation(const int depth, const int move);

    int Eval(board& aBoard) const {
      if(aBoard.GetSide() == constants::WHITE) {
        return aBoard.GetThreats();
      } else {
        return -aBoard.GetThreats();
      }
    }
};

#define __search
#endif
