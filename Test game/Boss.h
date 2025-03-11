#pragma once
#ifndef BOSS_H
#define BOSS_H

#include "Character.h"

class Boss : public Character {
public:
    Boss(const std::string& name, int hp);
    ~Boss();

    void attack() override;
    void specialAttack();
};

#endif
