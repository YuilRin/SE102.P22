#pragma once
#ifndef __CASTLEVANIA_GAME_H__
#define __CASTLEVANIA_GAME_H__

#include "Game.h"

class CastlevaniaGame : public Game
{
public:
    CastlevaniaGame(HINSTANCE hInstance, LPWSTR title);
    void virtual init() override;
    void virtual update(float deltatime) override;
    void virtual draw() override;
    void virtual release() override;
};

#endif // __CASTLEVANIA_GAME_H__
