#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

class Character {
public:
    Character(const std::string& name, int hp);
    virtual ~Character();

    virtual void attack();
    std::string getName() const;
    int getHP() const;
    void takeDamage(int damage);

protected:
    std::string name;
    int hp;
};

#endif
