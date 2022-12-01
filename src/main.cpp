#include <iostream>
#include <string>
#include "Game/Game.h"

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 320;

Game *game = NULL;

int main(int argc, char* args[] )
{
  game = new Game();

  game->init("Is epic", SCREEN_WIDTH, SCREEN_HEIGHT);
  game->setup();

  while (game->running())
  {
    game->frameStart();
    game->handleEvents();
    game->update();
    game->render();
    game->frameEnd();
  }
  game->clean();

  return 0;
}