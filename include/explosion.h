#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "commons.h"
#include "../lib/stds/include/draw.h"
#include "../lib/stds/include/animation.h"

extern Entity* add_explosion(enum ExplosionType, float, float);
extern void explosion_update(Entity*);
extern void explosion_draw(Entity*);
extern void explosion_die(Entity*);

#endif