#pragma once

#include "Position.h"

enum class Color { White, Black };

class Board;

enum class FigureType {
    Pawn, Rook, Knight, Bishop, Queen, King
};

class Figure {
protected:
    Color color;
    bool hasMoved = false; 

public:
    Figure(Color color);
    virtual ~Figure();

    virtual wchar_t getSymbol() const = 0;
    virtual bool isMoveValid(const Position& from, const Position& to, const Board& board) const = 0;
    virtual Figure* clone() const = 0;

    virtual char getType() const = 0;

    void markMoved() { hasMoved = true; }
    bool wasMoved() const {
        return hasMoved;
    }
    Color getColor() const;
};