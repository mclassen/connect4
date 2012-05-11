#ifndef _Hash_h_

struct HashEntry {
  unsigned long flags; // 0..5   distance
                       // 6..11  bestmove
                       // 12..31 value
  unsigned long key;

  int Distance() const { return (flags >> 0) & 63; }
  int BestMove() const { return (flags >> 6) & 63; }
  int Value()    const { return (flags >> 12); }
};

class HashKey {
 public:
  HashKey();
  void InitPosition(const int board[constants::MAX_MOVES]);

  inline void MoveMade(int move, int side);
  operator unsigned() const { return key; }

 private:
  static void InitRandomValues();
  static unsigned RANDOM_VALUES[constants::MAX_MOVES][2];
  static bool randomValuesInit;
  static unsigned CreateUnsignedRand();

  unsigned key;
};

#define _Hash_h_
#endif
