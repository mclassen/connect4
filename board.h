#ifndef __board
#define __board

#include "constants.h"
#include "hash.h"

class board {
 public:
  board();

  void Display();
  void GenerateMoves(MoveBuffer& buffer);
  bool MoveIsValid(square_t file, bool makingMove) const;
  inline void MakeMove(square_t move);
  inline void UnmakeMove(square_t move);

  square_t GetSquare(square_t file, square_t rank) const { 
    return square[file + rank * constants::MAX_FILES]; 
  }
  square_t GetSquare(int sq) const { return square[sq]; }
  side_t GetSide() const { return side; }
  square_t GetPiecesInFile(square_t file) const { return piecesInFile[file]; }
  square_t GetNumberOfMove() const { return numberOfMove; }
  square_t GetNumberOfPossibleMoves() const { return numberOfPossibleMoves; }
  val_t GetThreats() const { return threats; }
  bool GetFourConnected() const { return fourConnected; }
  inline square_t ConvertFileToMove(square_t file) const;
  
  inline const hashKey_t GetHashKey() const { return itsHashKey.getKey(); }
  
  static inline square_t GetMove(const square_t sq)
  {
	  return sq % constants::MAX_FILES;
  }

 private:
  side_t square[constants::MAX_FILES * constants::MAX_RANKS];
  val_t threatsBlack[constants::NUM_THREATS];
  val_t threatsWhite[constants::NUM_THREATS];
  side_t side;  // -1 == BLACK, 0 == NONE, 1 == WHITE
  square_t piecesInFile[constants::MAX_FILES];
  square_t numberOfMove;
  square_t numberOfPossibleMoves;
  bool fourConnected;
  val_t threats;
  
  HashKey itsHashKey;
  void UpdateThreats(square_t file, square_t rank, 
                     side_t side, bool makingMove);

};

inline void board::MakeMove(square_t sq) {
  square[sq] = side;
  UpdateThreats(File(sq), Rank(sq), side, true);
  itsHashKey.makeMove(sq, side);
  side = -side;
  piecesInFile[File(sq)]++;
  numberOfMove++;
}

inline void board::UnmakeMove(square_t sq) {
  const square_t file = File(sq);
  const square_t rank = Rank(sq);
  square[sq] = constants::NONE;
  piecesInFile[file]--;
  side = -side;
  UpdateThreats(file, rank, side, false);
  itsHashKey.makeMove(sq, side);
  fourConnected = false;
  numberOfMove--;
}

inline square_t board::ConvertFileToMove(square_t file) const {
  if(file >= 0 && file < constants::MAX_FILES) {
    return piecesInFile[file] * constants::MAX_FILES + file;
  } else {
    return constants::INVALID_MOVE;
  }
}

#endif // __board
