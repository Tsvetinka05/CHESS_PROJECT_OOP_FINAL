#pragma once

#include "Game.h"

class App {
    Game game;
public:
    void run();
    void saveGame() const;
    void loadGame();
};

