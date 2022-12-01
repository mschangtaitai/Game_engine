#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <string>

struct TagComponent {
  std::string tag;
};

struct TransformComponent {
  int posX;
  int posY;
};

struct MovementComponent {
  int speedX;
  int speedY;
};

struct SpriteComponent {
  int x = 0;
  int y = 0;
  int size = 32;
  SDL_Texture* texture;
};

#endif
