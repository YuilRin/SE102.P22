#include "Game.h"
#include <iostream>

Game::Game() {
    boss = new Boss("Final Boss", 1000);
    characters.push_back(new Character("Hero", 100));
}

Game::~Game() {
    for (auto* character : characters) {
        delete character;
    }
    delete boss;
}

void Game::run() {
    std::cout << "Game is running..." << std::endl;

    for (auto* character : characters) {
        character->attack();
    }

    boss->attack();
    boss->specialAttack();
}
