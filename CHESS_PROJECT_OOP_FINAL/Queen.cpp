#pragma execution_character_set("utf-8")
#include "Queen.h"
#include "Board.h"
#include <cmath>

Queen::Queen(Color color) : Figure(color) {}

wchar_t Queen::getSymbol() const {
    return color == Color::White ? L'♕' : L'♛';
}

bool Queen::isMoveValid(const Position& from, const Position& to, const Board& board) const {
    int rowDiff = to.row - from.row;
    int colDiff = to.col - from.col;

    if (rowDiff == 0 && colDiff == 0)
        return false;

    int rowStep = (rowDiff == 0) ? 0 : (rowDiff > 0 ? 1 : -1);
    int colStep = (colDiff == 0) ? 0 : (colDiff > 0 ? 1 : -1);

    if (std::abs(rowDiff) != std::abs(colDiff) && rowDiff != 0 && colDiff != 0)
        return false;

    int r = from.row + rowStep;
    int c = from.col + colStep;

    while (r != to.row || c != to.col) {
        if (board.getPieceAt({ r, c }) != nullptr)
            return false;
        r += rowStep;
        c += colStep;
    }

    const Figure* target = board.getPieceAt(to);
    return target == nullptr || target->getColor() != color;
}

Figure* Queen::clone() const {
    return new Queen(color);
}
