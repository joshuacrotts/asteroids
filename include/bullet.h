#ifndef BULLET_H
#define BULLET_H

#include "commons.h"
#include "../lib/stds/include/draw.h"


/*
 *
 */
extern entity_t*
add_bullet(float, float, uint16_t);


/*
 *
 */
extern void 
bullet_update(entity_t*);


/*
 *
 */
extern void 
bullet_draw(entity_t*);

#endif