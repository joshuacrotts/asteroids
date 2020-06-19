#ifndef ASTEROID_H
#define ASTEROID_H

#include "commons.h"
#include "../lib/stds/include/draw.h"
#include "../lib/stds/include/animation.h"

extern Entity* add_asteroid(float, float);
extern void asteroid_update(Entity*);
extern void asteroid_draw(Entity*);
extern void swap_asteroid_animation(Entity*);


#endif