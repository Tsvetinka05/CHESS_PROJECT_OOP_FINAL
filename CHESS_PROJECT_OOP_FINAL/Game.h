#pragma once

#include "Board.h"
#include <string>

class Game {
private:
    Board board;
    Color currentPlayer;
    Position findKing(Color color) const;
    bool isAttacked(const Position& pos, Color byColor) const;
    Position lastMoveFrom, lastMoveTo;

public:
    Game();

    void printBoard() const;

    bool isMoveLegal(const Position& from, const Position& to) const;
    bool makeMove(const Position& from, const Position& to);

    bool isInCheck(Color color) const;
    bool isCheckmate(Color color);
    bool isStalemate(Color color);
    bool hasLegalMove(Color color) const;

    Color getCurrentPlayer() const;
    void switchTurn();

    const Board& getBoard() const;
    void setLastMove(const Position& from, const Position& to);

    void saveGame(const std::string& filename) const;
    void loadGame(const std::string& filename);

    void initialize();
    bool isInsufficientMaterial() const;



    void clearBoard(); 
};

