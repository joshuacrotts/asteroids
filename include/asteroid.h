#ifndef ASTEROID_H
#define ASTEROID_H

#include "commons.h"
#include "../lib/stds/include/draw.h"
#include "../lib/stds/include/animation.h"


/*
 *
 */
extern entity_t* 
add_asteroid(float, float);


/*
 *
 */
extern void 
asteroid_update(entity_t*);


/*
 *
 */
extern void 
asteroid_draw(entity_t*);


/*
 *
 */
extern void 
swap_asteroid_animation(entity_t*);


#endif