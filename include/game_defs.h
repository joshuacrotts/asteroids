#ifndef GAME_DEFS_H
#define GAME_DEFS_H

#include "commons.h"

#define DEATH_MASK                       0x10000000
#define MOVE_MASK                        0x20000000

#define INSETS                           8
#define ID_SHIP_MASK                     0x00000100
#define ID_ASTEROID_MASK                 0x00000200
#define ID_BULLET_MASK                   0x00000400
#define ID_EXPLOSION_MASK                0x00000800
#define ID_PARTICLE_MASK                 0x00004000
#define ID_P_SQUARE_MASK                 0x00008000
#define ID_P_CIRCLE_MASK                 0x00010000
#define ID_DEBRIS_MASK                   0x00001000

enum ExplosionType {
    SMALL,
    MEDIUM,
    LARGE
};

#endif
