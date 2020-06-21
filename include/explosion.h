#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "commons.h"
#include "../lib/stds/include/draw.h"
#include "../lib/stds/include/animation.h"


/*
 *
 */
extern entity_t* 
add_explosion(enum ExplosionType, float, float);


/*
 *
 */
extern void 
explosion_update(entity_t*);


/*
 *
 */
extern void 
explosion_draw(entity_t*);


/*
 *
 */

extern void 
explosion_die(entity_t*);

#endif