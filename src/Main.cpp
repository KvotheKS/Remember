#include "Game.h"

int main(int argc, char** argv){
    Game& game = Game::GetInstance();
    game.Push(new PingusState());
    game.Run();

    return 0;
}