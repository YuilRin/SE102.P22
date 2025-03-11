#ifndef GAME_H
#define GAME_H

#include "Character.h"
#include "Boss.h"
#include <vector>

class Game {
public:
    Game();
    ~Game();
    void run();
private:
    std::vector<Character*> characters;
    Boss* boss;
};

#endif
