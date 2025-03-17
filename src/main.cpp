#include <iostream>
#include <string>
#include <filesystem>

#include "Game.h"

int main(int, char**) {
    std::cout << "This is a little game for you to have fun!" << std::endl;
    
    Game& game = Game::getInstance();
    game.init();
    game.run();
    
    std::cout << "Happy Game!" << std::endl;
    return 0;
}

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}
#endif
