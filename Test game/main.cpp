#include "game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Game myGame; 
    myGame.initWindow(hInstance, nCmdShow);
    myGame.run();
    return 0;
}
