#pragma execution_character_set("utf-8")
#include "Bishop.h"
#include "Board.h"
#include <cmath>

Bishop::Bishop(Color color) : Figure(color) {}

wchar_t Bishop::getSymbol() const {
    return color == Color::White ? L'♗' : L'♝';
}

bool Bishop::isMoveValid(const Position& from, const Position& to, const Board& board) const {
    int rowDiff = to.row - from.row;
    int colDiff = to.col - from.col;

    if (std::abs(rowDiff) != std::abs(colDiff))
        return false;

    int rowStep = (rowDiff > 0) ? 1 : -1;
    int colStep = (colDiff > 0) ? 1 : -1;

    int r = from.row + rowStep;
    int c = from.col + colStep;

    while (r != to.row && c != to.col) {
        if (board.getPieceAt({ r, c }) != nullptr)
            return false;
        r += rowStep;
        c += colStep;
    }

    const Figure* target = board.getPieceAt(to);
    return target == nullptr || target->getColor() != color;
}

Figure* Bishop::clone() const {
    return new Bishop(color);
}
