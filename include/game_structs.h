#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include "commons.h"

typedef struct Stage Stage;
typedef struct Emitter Emitter;
typedef struct Debris Debris;

struct Debris {
  float x;
  float y;
  float dx;
  float dy;
  int life;
  SDL_Rect rect;
  SDL_Texture* texture;
  Debris* next;
};

struct Emitter {
  Entity particleHead, *particleTail;
  int32_t x;
  int32_t y;
  uint32_t maxParticles;
  uint32_t idFlags;
  uint32_t flags;

  Emitter* next;
};

struct Stage {
  Animation animationHead, *animationTail;
  Entity explosionHead, *explosionTail;
  Entity entityHead, *entityTail;
  Debris debrisHead, *debrisTail;
};

#endif
