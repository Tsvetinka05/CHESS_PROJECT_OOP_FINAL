#pragma execution_character_set("utf-8")
#include "App.h"
#include "Game.h"
#include "Position.h"
#include <iostream>

Position parsePosition(const std::string& input) {
    if (input.size() != 2) return Position(-1, -1);

    char file = tolower(input[0]);
    char rank = input[1];

    if (file < 'a' || file > 'h') return Position(-1, -1);
    if (rank < '1' || rank > '8') return Position(-1, -1);

    int col = file - 'a';
    int row = 8 - (rank - '0');

    return Position(row, col);
}




void App::run() {
    Game game;

    char loadChoice;
    do {
        std::cout << "Do you want to load the last game? (y - yes / n - no): ";
        std::cin >> loadChoice;
        loadChoice = std::tolower(loadChoice);
        std::cin.ignore(1000, '\n'); // изчистване на излишен вход
    } while (loadChoice != 'y' && loadChoice != 'n');

    if (loadChoice == 'y') {
        game.loadGame("chess_game.dat");
    }
    else {
        game.initialize();
    }

    while (true) {
        system("cls");
        game.printBoard();

        // Показване ако играчът е в шах
        if (game.isInCheck(game.getCurrentPlayer(), game.getBoard())) {
            std::cout << "You are in check!\n";
        }

        std::cout << "Turn: " << (game.getCurrentPlayer() == Color::White ? "White" : "Black") << ": ";

        std::string fromStr, toStr;
        std::cin >> fromStr >> toStr;

        if (std::cin.fail() || fromStr.length() != 2 || toStr.length() != 2) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input. Please enter coordinates like e2 e4.\n";
            system("pause");
            continue;
        }

        if (fromStr == "exit" || toStr == "exit")
            break;

        if (fromStr == "save") {
            game.saveGame("chess_game.dat");
            std::cout << "Game saved successfully!\n";
            system("pause");
            continue;
        }

        Position from = parsePosition(fromStr);
        Position to = parsePosition(toStr);

        if (!from.isValid() || !to.isValid()) {
            std::cout << "Invalid coordinates. Use squares like e2 or d7.\n";
            system("pause");
            continue;
        }

        if (!game.makeMove(from, to)) {
            std::cout << "Invalid move. Try again.\n";
            system("pause");
            continue;
        }

        system("cls");
        game.printBoard();

        // Валидация на избор за запазване
        char saveChoice;
        do {
            std::cout << "Do you want to save the game? (y - yes / n - no): ";
            std::cin >> saveChoice;
            saveChoice = std::tolower(saveChoice);
            std::cin.ignore(1000, '\n');
        } while (saveChoice != 'y' && saveChoice != 'n');

        if (saveChoice == 'y') {
            game.saveGame("chess_game.dat");
            std::cout << "Game saved successfully!\n";

            // Валидация за продължаване
            char cont;
            do {
                std::cout << "Do you want to continue playing? (y - yes / n - no): ";
                std::cin >> cont;
                cont = std::tolower(cont);
                std::cin.ignore(1000, '\n');
            } while (cont != 'y' && cont != 'n');

            if (cont == 'n') {
                std::cout << "Exiting the game.\n";
                break;
            }

            system("pause");
        }

        // Проверка за край на играта
        if (game.isCheckmate(game.getCurrentPlayer())) {
            std::cout << "Checkmate! The game is over.\n";
            break;
        }

        if (game.isStalemate(game.getCurrentPlayer())) {
            std::cout << "Stalemate! It's a draw.\n";
            break;
        }

        if (game.isInsufficientMaterial()) {
            std::cout << "Draw due to insufficient material.\n";
            break;
        }
    }
}



void App::saveGame() const {
    game.saveGame("chess_game.dat");
}

void App::loadGame() {
    game.loadGame("chess_game.dat");
}

