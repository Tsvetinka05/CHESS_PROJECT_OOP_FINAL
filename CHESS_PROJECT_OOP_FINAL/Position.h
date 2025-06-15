#pragma once

class Position {
public:
    int row;
    int col;

    Position();
    Position(int row, int col);

    bool isValid() const;
    bool operator==(const Position& other) const;
};
