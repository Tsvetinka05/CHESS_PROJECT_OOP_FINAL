#pragma execution_character_set("utf-8")
#include "King.h"
#include "Board.h"
#include <cmath>

King::King(Color color) : Figure(color) {}

wchar_t King::getSymbol() const {
    return color == Color::White ? L'♔' : L'♚';
}

bool King::isMoveValid(const Position& from, const Position& to, const Board& board) const {
    int rowDiff = std::abs(from.row - to.row);
    int colDiff = std::abs(from.col - to.col);

    if (rowDiff <= 1 && colDiff <= 1) {
        const Figure* target = board.getPieceAt(to);
        return !target || target->getColor() != this->getColor();
    }

    return false;
}

Figure* King::clone() const {
    return new King(color);
}

bool King::isCastlingMove(const Position& from, const Position& to, const Board& board) const {
    if (wasMoved()) return false;
    if (from.row != to.row || std::abs(from.col - to.col) != 2) return false;

    int row = from.row;
    int dir = (to.col > from.col) ? 1 : -1;
    int rookCol = (dir == 1) ? 7 : 0;

    const Figure* rook = board.getPieceAt({ row, rookCol });
    if (!rook || rook->getColor() != color || rook->wasMoved())
        return false;

    for (int c = from.col + dir; c != rookCol; c += dir) {
        if (board.getPieceAt(Position(row, c)) != nullptr)
            return false;
    }

    return true;
}


