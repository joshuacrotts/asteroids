#ifndef BULLET_H
#define BULLET_H

#include "commons.h"
#include "../lib/stds/include/draw.h"

extern Entity* add_bullet(float, float, uint16_t);
extern void bullet_update(Entity*);
extern void bullet_draw(Entity*);

#endif