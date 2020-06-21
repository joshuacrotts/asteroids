#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include "commons.h"

typedef struct Stage stage_t;
typedef struct Emitter emitter_t;
typedef struct Debris debris_t;

struct Debris {
  float x;
  float y;
  float dx;
  float dy;
  int life;
  SDL_Rect rect;
  SDL_Texture* texture;
  debris_t* next;
};

struct Emitter {
  entity_t particle_head, *particle_tail;
  int32_t x;
  int32_t y;
  uint32_t maxParticles;
  uint32_t id_flags;
  uint32_t flags;

  emitter_t* next;
};

struct Stage {
  animation_t animation_head, *animation_tail;
  entity_t explosion_head, *explosion_tail;
  entity_t entity_head, *entity_tail;
  debris_t debris_head, *debris_tail;
};

#endif
