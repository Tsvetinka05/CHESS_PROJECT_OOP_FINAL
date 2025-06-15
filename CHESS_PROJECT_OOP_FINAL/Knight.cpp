#pragma execution_character_set("utf-8")
#include "Knight.h"
#include "Board.h"
#include <cmath>

Knight::Knight(Color color) : Figure(color) {}

wchar_t Knight::getSymbol() const {
    return color == Color::White ? L'♘' : L'♞';
}

bool Knight::isMoveValid(const Position& from, const Position& to, const Board& board) const {
    int rowDiff = std::abs(from.row - to.row);
    int colDiff = std::abs(from.col - to.col);

    if (!((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)))
        return false;

    const Figure* target = board.getPieceAt(to);
    return target == nullptr || target->getColor() != color;
}

Figure* Knight::clone() const {
    return new Knight(color);
}
