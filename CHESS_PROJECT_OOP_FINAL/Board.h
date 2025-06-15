#pragma once

#include "Figure.h"
#include "Position.h"

struct LastMove {
    Position from;
    Position to;
    bool valid = false;
};



class Board {
private:
    Figure* squares[8][8];

    Position lastMoveFrom;
    Position lastMoveTo;

    LastMove lastMove;

public:
    Board();
    ~Board();

    Board(const Board& other);
    Board& operator=(const Board& other);

    void initialize();

    const Figure* getPieceAt(const Position& pos) const;
    Figure* getPieceAt(const Position& pos);

    void movePiece(const Position& from, const Position& to, bool simulate = false);

    void removePieceAt(const Position& pos);

    void print(bool whitePerspective) const;

    void clear();

    void setPieceAt(const Position& pos, Figure* fig);

    Position getLastMoveFrom() const;
    Position getLastMoveTo() const;
    void setLastMove(const Position& from, const Position& to);

};
