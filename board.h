#ifndef __board
#define __board

#include "constants.h"
#include "hash.h"

class board {
 public:
  board();

  void Display();
  void GenerateMoves(MoveBuffer& buffer);
  bool MoveIsValid(sqType file, bool makingMove) const;
  inline void MakeMove(sqType move);
  inline void UnmakeMove(sqType move);

  sqType GetSquare(sqType file, sqType rank) const { 
    return square[file + rank * constants::MAX_FILES]; 
  }
  sqType GetSquare(int sq) const { return square[sq]; }
  sideType GetSide() const { return side; }
  sqType GetPiecesInFile(sqType file) const { return piecesInFile[file]; }
  sqType GetNumberOfMove() const { return numberOfMove; }
  sqType GetNumberOfPossibleMoves() const { return numberOfPossibleMoves; }
  valType GetThreats() const { return threats; }
  bool GetFourConnected() const { return fourConnected; }
  inline sqType ConvertFileToMove(sqType file) const;
  
  inline const hashKeyType GetHashKey() const { return itsHashKey.getKey(); }
  
  static inline sqType GetMove(const sqType sq)
  {
	  return sq % constants::MAX_FILES;
  }

 private:
  sideType square[constants::MAX_FILES * constants::MAX_RANKS];
  valType threatsBlack[constants::NUM_THREATS];
  valType threatsWhite[constants::NUM_THREATS];
  sideType side;  // -1 == BLACK, 0 == NONE, 1 == WHITE
  sqType piecesInFile[constants::MAX_FILES];
  sqType numberOfMove;
  sqType numberOfPossibleMoves;
  bool fourConnected;
  valType threats;
  
  HashKey itsHashKey;
  void UpdateThreats(sqType file, sqType rank, 
                     sideType side, bool makingMove);

};

inline void board::MakeMove(sqType sq) {
  square[sq] = side;
  UpdateThreats(File(sq), Rank(sq), side, true);
  itsHashKey.makeMove(sq, side);
  side = -side;
  piecesInFile[File(sq)]++;
  numberOfMove++;
}

inline void board::UnmakeMove(sqType sq) {
  const sqType file = File(sq);
  const sqType rank = Rank(sq);
  square[sq] = constants::NONE;
  piecesInFile[file]--;
  side = -side;
  UpdateThreats(file, rank, side, false);
  itsHashKey.makeMove(sq, side);
  fourConnected = false;
  numberOfMove--;
}

inline sqType board::ConvertFileToMove(sqType file) const {
  if(file >= 0 && file < constants::MAX_FILES) {
    return piecesInFile[file] * constants::MAX_FILES + file;
  } else {
    return constants::INVALID_MOVE;
  }
}

#endif // __board
