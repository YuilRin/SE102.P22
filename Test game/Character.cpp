#include "CastlevaniaGame.h"

CastlevaniaGame::CastlevaniaGame(HINSTANCE hInstance, LPWSTR title)
    : Game(hInstance, title, 800, 600, 60, 0)
{
}

void CastlevaniaGame::init()
{
    Game::init();
}

void CastlevaniaGame::update(float deltatime)
{
    // Update logic here
}

void CastlevaniaGame::draw()
{
    Game::render();
}

void CastlevaniaGame::release()
{
    Game::release();
}
