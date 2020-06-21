#ifndef PLAYER_H
#define PLAYER_H

#include "commons.h"
#include "../lib/stds/include/draw.h"


entity_t *player;
extern app_t app;
extern stage_t stage;


/*
 *
 */
extern void 
init_player(void);


/*
 *
 */
extern void 
player_update(void);


/*
 *
 */
extern void 
player_draw(void);


/*
 *
 */
extern void 
player_die(void);

#endif