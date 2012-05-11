class SearchSettings {
 public:
  SearchSettings() : timeLimit(0) {}
  
  void SetTimeLimit(int timeInSeconds) {
    timeLimit = timeInSeconds;
  }
  int GetTimeLimit() const {
    return timeLimit;
  }

 private:
  int timeLimit;
};