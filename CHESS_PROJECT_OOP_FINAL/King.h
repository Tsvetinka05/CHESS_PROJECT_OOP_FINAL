#pragma once

#include "Figure.h"


class King : public Figure {
public:
    King(Color color);
    virtual wchar_t getSymbol() const override;
    virtual bool isMoveValid(const Position& from, const Position& to, const Board& board) const override;
    virtual Figure* clone() const override;
    bool isCastlingMove(const Position& from, const Position& to, const Board& board) const;
    char getType() const override { return 'K'; }
};
