#pragma execution_character_set("utf-8")
#include "Rook.h"
#include "Board.h"

Rook::Rook(Color color) : Figure(color) {}

wchar_t Rook::getSymbol() const {
    return color == Color::White ? L'♖' : L'♜';
}

bool Rook::isMoveValid(const Position& from, const Position& to, const Board& board) const {
    if (from.row != to.row && from.col != to.col)
        return false;

    int rowStep = (to.row > from.row) ? 1 : (to.row < from.row ? -1 : 0);
    int colStep = (to.col > from.col) ? 1 : (to.col < from.col ? -1 : 0);

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

Figure* Rook::clone() const {
    return new Rook(color);
}
