#ifndef __board
#define __board

inline int File(int sq) { return sq % constants::MAX_FILES; }
inline int Rank(int sq) { return sq / constants::MAX_FILES; }

class board {
 public:	
  board();

  void Display();
  void GenerateMoves(MoveBuffer& buffer);
  bool MoveIsValid(int file, bool makingMove) const;
  inline void MakeMove(int move);
  inline void UnmakeMove(int move);

  int GetSquare(int file, int rank) const { 
    return square[file + rank * constants::MAX_FILES]; 
  }
  int GetSquare(int sq) const { return square[sq]; }
  int GetSide() const { return side; }
  int GetPiecesInFile(int file) const { return piecesInFile[file]; }
  int GetNumberOfMove() const { return numberOfMove; }
  int GetNumberOfPossibleMoves() const { return numberOfPossibleMoves; }
  int GetThreats() const { return threats; }
  bool GetFourConnected() const { return fourConnected; }
  inline int ConvertFileToMove(int file) const;

 private:
  int square[constants::MAX_FILES * constants::MAX_RANKS];
  int threatsBlack[constants::NUM_THREATS];
  int threatsWhite[constants::NUM_THREATS];
  int side;  // -1 == BLACK, 0 == NONE, 1 == WHITE
  int piecesInFile[constants::MAX_FILES];
  int numberOfMove;
  int numberOfPossibleMoves;
  bool fourConnected;
  int threats;
  void UpdateThreats(int file, int rank, 
		     int side, bool makingMove);

};

inline void board::MakeMove(int move) {
  square[move] = side;
  UpdateThreats(File(move), Rank(move), side, true);
  side = -side;
  piecesInFile[File(move)]++;
  numberOfMove++;
}

inline void board::UnmakeMove(int move) {
  const int file = File(move);
  const int rank = Rank(move);
  square[move] = constants::NONE;
  piecesInFile[file]--;
  side = -side;
  UpdateThreats(file, rank, side, false);
  fourConnected = false;
  numberOfMove--;
}

inline int board::ConvertFileToMove(int file) const {
  if(file >= 0 && file < constants::MAX_FILES) {
    return piecesInFile[file] * constants::MAX_FILES + file;
  } else {
    return constants::INVALID;
  }
}


#endif // __board
