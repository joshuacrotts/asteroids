#include "../include/asteroid.h"

static void check_bounds(entity_t*);


entity_t* 
add_asteroid(float x, float y) {
    entity_t *asteroid;

    asteroid = malloc(sizeof(entity_t));
    memset(asteroid, 0, sizeof(entity_t));
    asteroid->x = x;
    asteroid->y = y;
    asteroid->dx = random_float(-4, 4);
    asteroid->dy = random_float(-4, 4);
    asteroid->id_flags |= ID_ASTEROID_MASK;
    asteroid->health = 2;

    animation_t *a;
    a = add_spritesheet("res/img/rock.png", 16, random_float(0.04f, 0.10f), 0, 0);
    asteroid->w = a->w;
    asteroid->h = a->h;

    asteroid->animation = a;

    return asteroid;
}


void 
asteroid_update(entity_t* a) {
    if (a->animation != NULL) {
        animation_update(a);
    }
    check_bounds(a);

    a->x += a->dx;
    a->y += a->dy;
}


void 
asteroid_draw(entity_t* a) {
    if (a->animation != NULL) {
        animation_draw(a);
    }
}


void 
swap_asteroid_animation(entity_t* a) {
    a->animation = add_spritesheet("res/img/rock_small.png", 16, random_float(0.05f, 0.10f), 0, 0);
}


/*
 *
 */
static void 
check_bounds(entity_t* a) {
    if (a->x < 0) {
        a->x = SCREEN_WIDTH;
    }

    if (a->x > SCREEN_WIDTH) {
        a->x = 0;
    }

    if (a->y < 0) {
        a->y = SCREEN_HEIGHT;
    }

    if (a->y > SCREEN_HEIGHT) {
        a->y = 0;
    }
}