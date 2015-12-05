#ifndef _Hash_h_

#include "board.h"
#include "constants.h"

#include <chrono>
#include <iomanip>
#include <vector>

struct HashEntry {

    HashEntry(
            distType distance,
            bool side,
            valType value,
            valType alpha,
            valType beta,
            sqType sq
            ) : 
    itsDistance(distance),
    itsSide(side),
    itsValue(value),
    itsAlpha(alpha),
    itsBeta(beta),
    itsSq(sq)
    {
        // nothing to do here
    }

    distType itsDistance;
    bool itsSide;

    valType itsValue;
    valType itsAlpha;
    valType itsBeta;
    sqType itsSq;

    bool isUpper() const {
        return itsValue < itsAlpha;
    }

    bool isLower() const {
        return itsValue > itsBeta;
    }

    bool isExact() const {
        return !isUpper() && !isLower();
    }

    static const HashEntry getInvalidEntry() {
        HashEntry invalidEntry(
                0,
                false,
                constants::INVALID,
                constants::INVALID,
                constants::INVALID,
                constants::INVALID_MOVE
                );
        return invalidEntry;
    }

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

    hashKeyType getKey() const {
        return key;
    }

private:
    static void initRandomValues();
    static hashKeyType RANDOM_VALUES[constants::MAX_MOVES * 2];
    static bool randomValuesInit;
    static hashKeyType createUnsignedRand(sqType seed);

    hashKeyType key;
};

class HashTable {
public:

    HashTable(const unsigned size) : itsSize(size) {
        entries.reserve(itsSize);
        init();
    }

    void init() {
        HashEntry invalidEntry(HashEntry::getInvalidEntry());
        entries.assign(itsSize, invalidEntry);
    }

    void store(
            const hashKeyType key,
            const distType distance,
            const sideType side,
            const valType value,
            const valType alpha,
            const valType beta,
            const sqType sq) {

        HashEntry entry(
                distance,
                constants::WHITE == side ? false : true,
                value,
                alpha,
                beta,
                sq
                );
        unsigned pos(key % itsSize);
        HashEntry previousEntry(entries[pos]);
        if (previousEntry.itsValue == constants::INVALID)
        {
            entries[pos] = entry;
        }
        else if (previousEntry.itsDistance < distance && previousEntry.itsSq == sq)
        {
            entries[pos] = entry;
        }
        else if (previousEntry.itsDistance < distance && previousEntry.itsSq != sq)
        {
//            std::cerr << "#";
            entries[pos] = entry;
        }
        else if (previousEntry.itsDistance >= distance && previousEntry.itsSq != sq)
        {
            // we have a hash collision!
//            std::cerr << "!";
        }
        else if (previousEntry.itsDistance >= distance)
        {
            // don't overwrite an existing entry when it has a higher distance
        }
        else
        {
            // shouldn't happen...
            std::cerr << std::endl << 
                    "=== sq: "  << sq << 
                    ", #sq: "   << previousEntry.itsSq << 
                    ", dist: "  << distance <<
                    ", #dist: " << previousEntry.itsDistance <<
                    ", val: "   << value << 
                    ", #val: "  << previousEntry.itsValue <<
                    " ==="      << std::endl;
        }
    }

    valType lookup(
            hashKeyType key,
            const distType distance,
            const sideType side,
            valType& alpha,
            valType& beta,
            sqType& move) {
        HashEntry entry(entries[key % itsSize]);
        const valType value = entry.itsValue;
        if (value == constants::INVALID) {
            move = constants::INVALID_MOVE;
            return value;
        } else {
            if (entry.itsDistance < distance) {
                move = File(entry.itsSq);
                return constants::INVALID;
            }
            if (entry.isExact()) {
                move = File(entry.itsSq);
                return value;
            }
            if (entry.isUpper() && value <= alpha) {
                move = File(entry.itsSq);
                return value;
            }
            if (value > alpha && value < beta) {
                move = File(entry.itsSq);
                beta = value;
                return constants::INVALID;
            }
            if (entry.isLower() && value >= beta) {
                move = File(entry.itsSq);
                return value;
            }
            if (value < beta && value > alpha) {
                move = File(entry.itsSq);
                alpha = value;
                return constants::INVALID;
            }
            else
            {
                return constants::INVALID;
            }
        }
    }

private:
    unsigned itsSize;
    std::vector<HashEntry> entries;

};

#define _Hash_h_
#endif
