#ifndef _Hash_h_

#include "board.h"
#include "constants.h"

#include <chrono>
#include <cstdint>
#include <iomanip>
#include <vector>

struct HashEntry {

	HashEntry()
	{
		// set isEmpty flag:
		bool isEmpty = true;
		setIsEmpty(isEmpty);
	}

    HashEntry(
            dist_t distance,
			side_t side,
			square_t   sq,
            val_t  value,
            val_t  alpha,
            val_t  beta
            )
    {
        // set all the flags:
		setDistance(distance);
		setSide(side);
		setSquare(sq);
		setValue(value);
		setAlpha(alpha);
		setBeta(beta);
		
		const bool isUpper = value < alpha;
		const bool isLower = value > beta;
		const bool isExact = !isUpper && !isLower;
		const bool isEmpty = false;
		setIsUpper(isUpper);
		setIsLower(isLower);
		setIsExact(isExact);
		setIsEmpty(isEmpty);
    }

	// 0..5   distance ( 6 bit)
	// 6      side     ( 1 bit)
	// 7..12  square   ( 6 bit)
	// 13..24 value    (12 bit)
	// 25..36 alpha    (12 bit)
	// 37..48 beta     (12 bit)
	// 49     isUpper  ( 1 bit)
	// 50     isLower  ( 1 bit)
	// 51     isExact  ( 1 bit)
	// 52     isEmpty  ( 1 bit)
	hashEntry_t flags;

	// getter functions:
	dist_t  getDistance() const	{ return (flags >>  0) &   63; }
	bool	getSide() const		{ return (flags >>  6) &    1; }
	square_t getSquare() const	{ return (flags >>  7) &   63; }
	val_t   getValue() const	{ return (flags >> 13) & 2047; }
	val_t   getAlpha() const	{ return (flags >> 25) & 2047; }
	val_t   getBeta() const		{ return (flags >> 37) & 2047; }
	bool	isUpper() const		{ return (flags >> 49) &    1; }
	bool	isLower() const		{ return (flags >> 50) &    1; }
	bool	isExact() const		{ return (flags >> 51) &    1; }
	bool	isEmpty() const		{ return (flags >> 52) &    1; }

	// setter functions:
	void setDistance(dist_t distance)	{ flags = (1UL >> 0) & (distance & 63); }
	void setSide(bool side)				{ flags = (1UL >> 6) & (side & 1); }
	void setSquare(square_t sq)			{ flags = (1UL >> 7) & (sq & 63); }
	void setValue(val_t  value)			{ flags = (1UL >> 13) & (value & 2047); }
	void setAlpha(val_t  alpha)			{ flags = (1UL >> 25) & (alpha & 2047); }
	void setBeta(val_t beta)			{ flags = (1UL >> 37) & (beta  & 2047); }
	void setIsUpper(bool isUpper)		{ flags = (1UL >> 49) & isUpper; }
	void setIsLower(bool isLower)		{ flags = (1UL >> 50) & isLower; }
	void setIsExact(bool isExact)		{ flags = (1UL >> 51) & isExact; }
	void setIsEmpty(bool isEmpty)		{ flags = (1UL >> 52) & isEmpty; }
};

class HashKey {
public:

    HashKey() : key(0) {
        if (randomValuesInit == false) {
            initRandomValues();
        }
    }
//    void init(const board& aBoard);

    void makeMove(int sq, int side) {
        if (side == constants::WHITE) {
            key ^= RANDOM_VALUES[sq];
        } else if (side == constants::BLACK) {
            key ^= RANDOM_VALUES[sq + constants::MAX_MOVES];
        }
    }

    hashKey_t getKey() const {
        return key;
    }

private:
    static void initRandomValues();
    static hashKey_t RANDOM_VALUES[constants::MAX_MOVES * 2];
    static bool randomValuesInit;
    static hashKey_t createUnsignedRand(square_t seed);

    hashKey_t key;
};

class HashTable {
public:

    HashTable(const unsigned size) : itsSize(size) {
        entries.reserve(itsSize);
        init();
    }

    void init() {
		HashEntry emptyEntry;
		entries.assign(itsSize, emptyEntry);
    }

    void store(
            const hashKey_t key,
            const dist_t distance,
            const side_t side,
            const val_t value,
            const val_t alpha,
            const val_t beta,
            const square_t sq) {

        HashEntry entry(
                distance,
                constants::WHITE == side,
                value,
                alpha,
                beta,
                sq
                );
        unsigned pos(key % itsSize);
        HashEntry previousEntry(entries[pos]);
        if (previousEntry.getValue() == constants::INVALID_VALUE)
        {
            entries[pos] = entry;
        }
        else if (previousEntry.getDistance() < distance && previousEntry.getSquare() == sq)
        {
            entries[pos] = entry;
        }
        else if (previousEntry.getDistance() < distance && previousEntry.getSquare() != sq)
        {
//            std::cerr << "#";
            entries[pos] = entry;
        }
        else if (previousEntry.getDistance() >= distance && previousEntry.getSquare() != sq)
        {
            // we have a hash collision!
//            std::cerr << "!";
        }
        else if (previousEntry.getDistance() >= distance)
        {
            // don't overwrite an existing entry when it has a higher distance
        }
        else
        {
            // shouldn't happen...
            std::cerr << std::endl << 
                    "=== sq: "  << sq << 
                    ", #sq: "   << previousEntry.getSquare() << 
                    ", dist: "  << distance <<
                    ", #dist: " << previousEntry.getDistance() <<
                    ", val: "   << value << 
                    ", #val: "  << previousEntry.getValue() <<
                    " ==="      << std::endl;
        }
    }

    val_t lookup(
            hashKey_t key,
            const dist_t distance,
            const side_t side,
            val_t& alpha,
            val_t& beta,
            square_t& move) {
        HashEntry entry(entries[key % itsSize]);
		const bool isEmpty = entry.isEmpty();
        const val_t value = entry.getValue();
		const square_t sq = entry.getSquare();
		if (isEmpty)
		{
			move = constants::INVALID_MOVE;
			return constants::INVALID_VALUE;
		}
        if (value == constants::INVALID_VALUE) {
            move = constants::INVALID_MOVE;
            return value;
        }
		if (entry.getDistance() < distance) {
			move = File(sq);
			return constants::INVALID_VALUE;
		}
		if (entry.isExact()) {
			move = File(sq);
			return value;
		}
		if (entry.isUpper() && value <= alpha) {
			move = File(sq);
			return value;
		}
		if (value > alpha && value < beta) {
			move = File(sq);
			beta = value;
			return constants::INVALID_VALUE;
		}
		if (entry.isLower() && value >= beta) {
			move = File(sq);
			return value;
		}
		if (value < beta && value > alpha) {
			move = File(sq);
			alpha = value;
			return constants::INVALID_VALUE;
		}
		else
		{
			move = constants::INVALID_MOVE;
			return constants::INVALID_VALUE;
		}
    }

private:
    unsigned itsSize;
    std::vector<HashEntry> entries;

};

#define _Hash_h_
#endif
