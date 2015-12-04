#ifndef _Hash_h_

#include "board.h"

#include <vector>

struct HashEntry {
	
	HashEntry(
		int distance,
		bool side,
		int value,
		int alpha,
		int beta,
		unsigned long key
	) :	itsDistance(distance),
		itsSide(side),
		itsValue(value),
		itsAlpha(alpha),
		itsBeta(beta),
		itsKey(key)
	{
		// nothing to do here
	}
		
	int itsDistance;
	bool itsSide;

	int itsValue;
	int itsAlpha;
	int itsBeta;

	unsigned long itsKey;

	bool isUpper() const { return itsValue < itsAlpha; }
	bool isLower() const { return itsValue > itsBeta; }
	bool isExact() const { return !isUpper() && !isLower(); }
};

class HashKey {
 public:
  HashKey();
  void init(const board& aBoard);

  void makeMove(int move, int side)
  {
	if(side == constants::WHITE)
		key ^= RANDOM_VALUES[move][0];
	else
		key ^= RANDOM_VALUES[move][1];
  }

  unsigned long getKey() const { return key; }

 private:
  static void initRandomValues();
  static unsigned long RANDOM_VALUES[constants::MAX_MOVES][2];
  static bool randomValuesInit;
  static unsigned long createUnsignedRand();

  unsigned long key;
};

class HashTable
{
public:
	HashTable(const unsigned long size) : itsSize(size)
	{
		entries.reserve(itsSize);
		init();
	}
	
	void init()
	{
		HashEntry invalidEntry(
			0,
			false,
			constants::INVALID,
			constants::INVALID,
			constants::INVALID,
			0
		);
		entries.assign(itsSize, invalidEntry);
	}
	
	void store(
		const board& currBoard,
		const int distance,
		const int side,
		const int value,
		const int alpha,
		const int beta)
	{
		HashKey key;
		key.init(currBoard);
		
		HashEntry entry(
			distance,
			constants::WHITE == side ? false : true,
			value,
			alpha,
			beta,
			key.getKey()
		);
		entries[key.getKey() % itsSize] = entry;
	}
	
	int lookup(
		const board& currBoard,
		const int distance,
		const int side,
		int& alpha,
		int& beta)
	{
		HashKey key;
		key.init(currBoard);
		
		HashEntry entry(entries[key.getKey() % itsSize]);
		const int value = entry.itsValue;
		if (value == constants::INVALID)
		{
			return value;
		}
		else
		{
			if (entry.itsDistance <= distance)
			{
				return constants::INVALID;
			}
			if (entry.isExact())
			{
				return value;
			}
			if (entry.isUpper() && value <= alpha)
			{
				return value;
			}
			if (value > alpha && value < beta)
			{
				beta = value;
				return constants::INVALID;
			}
			if (entry.isLower() && value >= beta)
			{
				return value;
			}
			if (value < beta && value > alpha)
			{
				alpha = value;
				return constants::INVALID;
			}
		}
	}

private:
	unsigned long itsSize;
	std::vector<HashEntry> entries;
	
};

#define _Hash_h_
#endif
