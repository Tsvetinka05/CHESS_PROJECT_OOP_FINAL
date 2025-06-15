#include "Board.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Rook.h"
#include "ConsoleColors.h"
#include <iomanip>
#include <locale>

#pragma execution_character_set("utf-8")

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>


Board::Board() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            squares[r][c] = nullptr;

    clear();
}


Board::~Board() {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            delete squares[i][j];
}


Board::Board(const Board& other) {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (other.squares[i][j])
                squares[i][j] = other.squares[i][j]->clone();
            else
                squares[i][j] = nullptr;

    lastMove = other.lastMove;
}



Board& Board::operator=(const Board& other) {
    if (this != &other) {
        clear();
        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c)
                squares[r][c] = other.squares[r][c] ? other.squares[r][c]->clone() : nullptr;
    }
    return *this;
}

void Board::clear() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            delete squares[r][c];
            squares[r][c] = nullptr;
        }
}

const Figure* Board::getPieceAt(const Position& pos) const {
    return pos.isValid() ? squares[pos.row][pos.col] : nullptr;
}

Figure* Board::getPieceAt(const Position& pos) {
    return pos.isValid() ? squares[pos.row][pos.col] : nullptr;
}

void Board::removePieceAt(const Position& pos) {
    if (pos.isValid()) {
        delete squares[pos.row][pos.col];
        squares[pos.row][pos.col] = nullptr;
    }
}

void Board::print(bool whitePerspective) const {
    std::wcout.imbue(std::locale("en_US.utf8"));

    int rowStart = whitePerspective ? 7 : 0;
    int rowEnd = whitePerspective ? -1 : 8;
    int rowStep = whitePerspective ? -1 : 1;

    int colStart = whitePerspective ? 0 : 7;
    int colEnd = whitePerspective ? 8 : -1;
    int colStep = whitePerspective ? 1 : -1;

    std::wcout << L"\n  ";
    for (int c = colStart; c != colEnd; c += colStep)
        std::wcout << static_cast<wchar_t>('a' + c) << L" ";
    std::wcout << L"\n";

    for (int r = rowStart; r != rowEnd; r += rowStep) {
        int rankLabel = 8 - r;
        std::wcout << rankLabel << L" ";

        for (int c = colStart; c != colEnd; c += colStep) {
            const Figure* fig = squares[r][c];

            bool isWhiteSquare = (r + c) % 2 == 0;
            int bgColor = isWhiteSquare ? 8 : 4;
            int textColor = fig && fig->getColor() == Color::White ? 15 : 0;

            setColor(textColor, bgColor);

            if (fig)
                std::wcout << fig->getSymbol() << L" ";
            else
                std::wcout << L"  ";
        }

        setColor(15, 0);
        std::wcout << rankLabel << L"\n";
    }

    std::wcout << L"  ";
    for (int c = colStart; c != colEnd; c += colStep)
        std::wcout << static_cast<wchar_t>('a' + c) << L" ";
    std::wcout << L"\n";

    setColor(15, 0);
}
void Board::initialize() {
    clear();

    for (int col = 0; col < 8; ++col)
        squares[1][col] = new Pawn(Color::Black);

    for (int col = 0; col < 8; ++col)
        squares[6][col] = new Pawn(Color::White);

    squares[0][0] = new Rook(Color::Black);
    squares[0][1] = new Knight(Color::Black);
    squares[0][2] = new Bishop(Color::Black);
    squares[0][3] = new Queen(Color::Black);
    squares[0][4] = new King(Color::Black);
    squares[0][5] = new Bishop(Color::Black);
    squares[0][6] = new Knight(Color::Black);
    squares[0][7] = new Rook(Color::Black);

    squares[7][0] = new Rook(Color::White);
    squares[7][1] = new Knight(Color::White);
    squares[7][2] = new Bishop(Color::White);
    squares[7][3] = new Queen(Color::White);
    squares[7][4] = new King(Color::White);
    squares[7][5] = new Bishop(Color::White);
    squares[7][6] = new Knight(Color::White);
    squares[7][7] = new Rook(Color::White);
}




void Board::movePiece(const Position& from, const Position& to, bool simulate) {
    if (!from.isValid() || !to.isValid()) {
        std::cout << "[ERROR] Invalid coordinates.\n";
        return;
    }

    Figure* moving = squares[from.row][from.col];
    if (!moving) {
        std::cout << "[ERROR] No piece at source square.\n";
        return;
    }

    //     En Passant capture 
    if (dynamic_cast<Pawn*>(moving) && from.col != to.col && squares[to.row][to.col] == nullptr) {
        int direction = (moving->getColor() == Color::White) ? 1 : -1;
        Position capturedPos(to.row + direction, to.col);
        Figure* captured = squares[capturedPos.row][capturedPos.col];

        if (captured && dynamic_cast<Pawn*>(captured) &&
            captured->getColor() != moving->getColor()) {

            if (!simulate) {
                delete captured;
            }
            squares[capturedPos.row][capturedPos.col] = nullptr;
        }
    }

    //      Standard capture
    if (squares[to.row][to.col] && squares[to.row][to.col] != moving) {
        if (!simulate) delete squares[to.row][to.col];
        squares[to.row][to.col] = nullptr;
    }

    //      Move the piece 
    squares[to.row][to.col] = moving;
    squares[from.row][from.col] = nullptr;

    if (!simulate) {
        moving->markMoved();
        setLastMove(from, to);
    }

    //      Promotion 
    if (!simulate) {
        Figure*& landed = squares[to.row][to.col];
        if (dynamic_cast<Pawn*>(landed)) {
            bool promote = (landed->getColor() == Color::White && to.row == 0) ||
                (landed->getColor() == Color::Black && to.row == 7);

            if (promote) {
                try {
                    std::cout << "Choose promotion (q – Queen, r – Rook, b – Bishop, n – Knight): ";
                    char choice;
                    std::cin >> choice;

                    Color color = landed->getColor();
                    delete landed;
                    landed = nullptr;

                    switch (choice) {
                    case 'q': landed = new Queen(color); break;
                    case 'r': landed = new Rook(color); break;
                    case 'b': landed = new Bishop(color); break;
                    case 'n': landed = new Knight(color); break;
                    default:
                        std::cout << "Invalid choice. Promoting to Queen.\n";
                        landed = new Queen(color); break;
                    }

                }
                catch (const std::exception& ex) {
                    std::cerr << "[EXCEPTION] Promotion failed: " << ex.what() << std::endl;
                }
                catch (...) {
                    std::cerr << "[EXCEPTION] Unknown error during promotion!" << std::endl;
                }
            }
        }
    }
}



void Board::setPieceAt(const Position& pos, Figure* fig) {
    if (squares[pos.row][pos.col])
        delete squares[pos.row][pos.col];
    squares[pos.row][pos.col] = fig;
}


Position lastMoveFrom = Position(-1, -1);
Position lastMoveTo = Position(-1, -1);

Position Board::getLastMoveFrom() const {
    return lastMove.valid ? lastMove.from : Position(-1, -1);
}

Position Board::getLastMoveTo() const {
    return lastMove.valid ? lastMove.to : Position(-1, -1);
}


void Board::setLastMove(const Position& from, const Position& to) {
    lastMove.from = from;
    lastMove.to = to;
    lastMove.valid = true;
}



