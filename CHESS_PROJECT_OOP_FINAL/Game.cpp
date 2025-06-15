#include "Game.h"
#include "King.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include <iostream>
#include <fstream>
#include <limits>



Game::Game() : currentPlayer(Color::White),
lastMoveFrom(-1, -1), lastMoveTo(-1, -1) {
    board.initialize();
}


void Game::printBoard() const {
    std::wcout << L"From White's perspective:\n";
    board.print(true);

    std::wcout << L"\nFrom Black's perspective:\n";
    board.print(false);
}


Color Game::getCurrentPlayer() const {
    return currentPlayer;
}

void Game::switchTurn() {
    currentPlayer = (currentPlayer == Color::White) ? Color::Black : Color::White;
}

const Board& Game::getBoard() const {
    return board;
}

bool Game::isMoveLegal(const Position& from, const Position& to) const {
    const Figure* piece = board.getPieceAt(from);
    if (!piece || piece->getColor() != currentPlayer)
        return false;

    if (!piece->isMoveValid(from, to, board))
        return false;

    Board tempBoard = board;
    tempBoard.movePiece(from, to, true);

    Game tempGame = *this;
    tempGame.board = tempBoard;
    return !tempGame.isInCheck(currentPlayer);
}

bool Game::makeMove(const Position& from, const Position& to) {

    if (!from.isValid() || !to.isValid()) {
        std::cout << "[ERROR] Invalid coordinates.\n";
        return false;
    }

    Figure* moving = board.getPieceAt(from);
    if (!moving) {
        std::cout << "[ERROR] No piece at source position.\n";
        return false;
    }

    Color pieceColor = moving->getColor();

    if (pieceColor != currentPlayer) {
        std::cout << "[ERROR] It's not this player's turn.\n";
        return false;
    }

    if (!isMoveLegal(from, to)) {
        std::cout << "[ERROR] Move is not legal.\n";
        return false;
    }

    board.movePiece(from, to);


    Figure* landed = board.getPieceAt(to);
    if (landed && dynamic_cast<King*>(landed)) {
        int row = to.row;
        if (to.col - from.col == 2) {
            board.movePiece(Position(row, 7), Position(row, 5));
        }
        else if (from.col - to.col == 2) {
            board.movePiece(Position(row, 0), Position(row, 3));
        }
    }

    switchTurn();
    return true;
}






Position Game::findKing(Color color) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Position pos(row, col);
            const Figure* fig = board.getPieceAt(pos);
            if (fig && fig->getColor() == color && fig->getSymbol() == (color == Color::White ? L'♔' : L'♚'))
                return pos;
        }
    }
    return Position(-1, -1);
}

bool Game::isAttacked(const Position& pos, Color byColor) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Position attackerPos(row, col);
            const Figure* fig = board.getPieceAt(attackerPos);
            if (fig && fig->getColor() == byColor && fig->isMoveValid(attackerPos, pos, board))
                return true;
        }
    }
    return false;
}

bool Game::isInCheck(Color color) const {
    Position kingPos = findKing(color);
    Color opponent = (color == Color::White ? Color::Black : Color::White);
    return isAttacked(kingPos, opponent);
}


bool Game::hasLegalMove(Color color) const {
    for (int fromRow = 0; fromRow < 8; ++fromRow) {
        for (int fromCol = 0; fromCol < 8; ++fromCol) {
            Position from(fromRow, fromCol);
            const Figure* fig = board.getPieceAt(from);
            if (!fig || fig->getColor() != color)
                continue;

            for (int toRow = 0; toRow < 8; ++toRow) {
                for (int toCol = 0; toCol < 8; ++toCol) {
                    Position to(toRow, toCol);
                    if (!fig->isMoveValid(from, to, board))
                        continue;

                    Board tempBoard = board;
                    tempBoard.movePiece(from, to, true);

                    Game tempGame = *this;
                    tempGame.board = tempBoard;

                    if (!tempGame.isInCheck(color))
                        return true;
                }
            }
        }
    }
    return false;
}

bool Game::isCheckmate(Color color) {
    return isInCheck(color) && !hasLegalMove(color);
}


bool Game::isStalemate(Color color) {
    return !isInCheck(color) && !hasLegalMove(color);
}

void Game::saveGame(const std::string& filename) const {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Cannot open file for saving!\n";
        return;
    }

    // Save current player
    outFile.write(reinterpret_cast<const char*>(&currentPlayer), sizeof(currentPlayer));

    // Save each square on the board
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            const Figure* fig = board.getPieceAt(Position(i, j));
            if (fig) {
                char type = fig->getType();
                char color = static_cast<char>(fig->getColor()); 
                bool moved = fig->wasMoved();

                outFile.write(&type, sizeof(type));
                outFile.write(&color, sizeof(color));
                outFile.write(reinterpret_cast<const char*>(&moved), sizeof(moved));
            }
            else {
                char empty = '0';
                outFile.write(&empty, sizeof(empty));
            }
        }
    }

    outFile.close();
    std::cout << "Game saved successfully.\n";
}



void Game::loadGame(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Cannot open file for loading!\n";
        return;
    }

    board.clear();

    // Load current player
    inFile.read(reinterpret_cast<char*>(&currentPlayer), sizeof(currentPlayer));

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            char type;
            inFile.read(&type, sizeof(type));

            if (!inFile) {
                std::cerr << "Error while reading piece type from file.\n";
                return;
            }

            if (type != '0') {
                char colorChar;
                bool moved;

                inFile.read(&colorChar, sizeof(colorChar));
                inFile.read(reinterpret_cast<char*>(&moved), sizeof(moved));

                Color color = static_cast<Color>(colorChar);
                Figure* fig = nullptr;

                switch (type) {
                case 'P': fig = new Pawn(color); break;
                case 'R': fig = new Rook(color); break;
                case 'N': fig = new Knight(color); break;
                case 'B': fig = new Bishop(color); break;
                case 'Q': fig = new Queen(color); break;
                case 'K': fig = new King(color); break;
                default:
                    std::cerr << "Unknown figure type in save file.\n";
                    return;
                }

                if (moved) fig->markMoved();

                board.setPieceAt(Position(i, j), fig);
            }
            else {
                board.setPieceAt(Position(i, j), nullptr);
            }
        }
    }

    inFile.close();
    std::cout << "Game loaded successfully.\n";
}


void Game::initialize() {
    board.initialize();
    currentPlayer = Color::White;
    lastMoveFrom = Position();
    lastMoveTo = Position();
}


bool Game::isInsufficientMaterial() const {
    int minorPieces = 0;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Figure* fig = board.getPieceAt(Position(row, col));
            if (!fig) continue;

            char type = fig->getType();
            if (type == 'K') continue;

            if (type == 'N' || type == 'B') {
                ++minorPieces;
            }
            else {
                return false;
            }

            if (minorPieces > 1) {
                return false;
            }
        }
    }

    return true;
}





void Game::clearBoard() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c) {
            Figure* fig = board.getPieceAt(Position(r, c));
            delete fig;
            board.setPieceAt(Position(r, c), nullptr);
        }
}

