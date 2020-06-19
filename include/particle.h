#ifndef PARTICLE_H
#define PARTICLE_H

#include "commons.h"
#include "../lib/stds/include/draw.h"

extern App app;

extern Entity* add_particle(float, float, float, float, float, float, uint16_t, uint16_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, float, uint32_t);
extern Entity* add_animated_particle(float, float, float, float, float, float, uint16_t, uint32_t, Animation*);
extern Animation* add_animation(char*, uint8_t, float);
extern void particle_update(Entity*);
extern void particle_draw(Entity*);
extern void particle_die(Entity*);
extern void animation_update(Entity*);
extern void animation_draw(Entity*);
extern void animation_die(Entity*);

#endif
