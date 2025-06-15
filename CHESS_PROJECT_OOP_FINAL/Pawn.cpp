#pragma execution_character_set("utf-8")
#include "Pawn.h"
#include "Board.h"
#include <cmath>

Pawn::Pawn(Color color) : Figure(color) {}

wchar_t Pawn::getSymbol() const {
    return color == Color::White ? L'♙' : L'♟';
}


bool Pawn::isMoveValid(const Position& from, const Position& to, const Board& board) const {
    int direction = (color == Color::White) ? -1 : 1;
    int startRow = (color == Color::White) ? 6 : 1;

    int rowDiff = to.row - from.row;
    int colDiff = to.col - from.col;

    if (colDiff == 0 && rowDiff == direction && board.getPieceAt(to) == nullptr)
        return true;

    if (colDiff == 0 && rowDiff == 2 * direction && from.row == startRow &&
        board.getPieceAt({ from.row + direction, from.col }) == nullptr &&
        board.getPieceAt(to) == nullptr)
        return true;

    if (std::abs(colDiff) == 1 && rowDiff == direction) {
        const Figure* target = board.getPieceAt(to);
        if (target && target->getColor() != color)
            return true;

        Position lastFrom = board.getLastMoveFrom();
        Position lastTo = board.getLastMoveTo();
        const Figure* lastMoved = board.getPieceAt(lastTo);

        if (lastMoved &&
            dynamic_cast<const Pawn*>(lastMoved) &&
            lastMoved->getColor() != color &&
            std::abs(lastFrom.row - lastTo.row) == 2 &&
            lastTo.row == from.row && lastTo.col == to.col) {
            return true;
        }
    }

    return false;
}

Figure* Pawn::clone() const {
    return new Pawn(color);
}
