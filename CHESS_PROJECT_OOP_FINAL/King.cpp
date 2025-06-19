#pragma execution_character_set("utf-8")
#include "King.h"
#include "Board.h"
#include "Rook.h"
#include <cmath>

King::King(Color color) : Figure(color) {}

wchar_t King::getSymbol() const {
    return color == Color::White ? L'♔' : L'♚';
}

bool King::isMoveValid(const Position& from, const Position& to, const Board& board) const {
    int rowDiff = std::abs(from.row - to.row);
    int colDiff = std::abs(from.col - to.col);

    // === Обикновен ход на царя ===
    if (rowDiff <= 1 && colDiff <= 1) {
        const Figure* target = board.getPieceAt(to);
        return !target || target->getColor() != this->getColor();
    }

    // === Рокада ===
    if (!this->wasMoved() && from.row == to.row) {
        // Къса рокада (цар от е към g / колона 4 -> 6)
        if (to.col == from.col + 2) {
            const Figure* f = board.getPieceAt(Position(from.row, 7));
            const Rook* rook = dynamic_cast<const Rook*>(f);

            if (rook && !rook->wasMoved() &&
                board.getPieceAt(Position(from.row, 5)) == nullptr &&
                board.getPieceAt(Position(from.row, 6)) == nullptr) {
                return true;
            }
        }
        // Дълга рокада (цар от е към c / колона 4 -> 2)
        else if (to.col == from.col - 2) {
            const Figure* f = board.getPieceAt(Position(from.row, 0));
            const Rook* rook = dynamic_cast<const Rook*>(f);

            if (rook && !rook->wasMoved() &&
                board.getPieceAt(Position(from.row, 1)) == nullptr &&
                board.getPieceAt(Position(from.row, 2)) == nullptr &&
                board.getPieceAt(Position(from.row, 3)) == nullptr) {
                return true;
            }
        }
    }

    return false;
}

Figure* King::clone() const {
    return new King(color);
}




