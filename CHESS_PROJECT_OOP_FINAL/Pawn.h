#pragma once

#include "Figure.h"


class Pawn : public Figure {
public:
    Pawn(Color color);
    virtual wchar_t getSymbol() const override;
    virtual bool isMoveValid(const Position& from, const Position& to, const Board& board) const override;
    virtual Figure* clone() const override;
    char getType() const override { return 'P'; }
};
