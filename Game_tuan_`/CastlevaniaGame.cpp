#include "CastlevaniaGame.h"

CastlevaniaGame::CastlevaniaGame(HINSTANCE hInstance, LPWSTR title) : Game(hInstance, title, WINDOW_WIDTH, WINDOW_HEIGHT)
{
}

CastlevaniaGame::~CastlevaniaGame()
{
}

void CastlevaniaGame::init()
{
	Game::init();
	// init game
//#if _DEBUG
//	SceneManager::getInstance()->addScene(new IntroScene());
//#else
//	SceneManager::getInstance()->addScene(new IntroScene());
//#endif
}

void CastlevaniaGame::release()
{
	Game::release();

	// release game
	SceneManager::getInstance()->clearScenes();
}

void CastlevaniaGame::updateInput(float deltatime)
{
	SceneManager::getInstance()->updateInput(deltatime);
}

void CastlevaniaGame::update(float deltatime)
{
	SceneManager::getInstance()->update(deltatime);
}

void CastlevaniaGame::draw()
{
	this->_spriteHandle->Begin(D3DXSPRITE_ALPHABLEND);

	SceneManager::getInstance()->draw(_spriteHandle);

	this->_spriteHandle->End();
}

void CastlevaniaGame::loadResource()
{
	// Game::init ?� g?i h�m n�y r?i n�n kh�ng c?n g?i l?i CastlevaniaGame::loadResource
	// load resource
	SpriteManager::getInstance()->loadResource(_spriteHandle);
	//SoundManager::getInstance()->loadSound(Game::hWindow->getWnd());
	//StageManager::getInstance()->loadResource();
}
