#include "../include/asteroid.h"

static void check_bounds(Entity*);

Entity* add_asteroid(float x, float y) {
    Entity* asteroid;

    asteroid = malloc(sizeof(Entity));
    memset(asteroid, 0, sizeof(Entity));
    asteroid->x = x;
    asteroid->y = y;
    asteroid->dx = randomFloat(-4, 4);
    asteroid->dy = randomFloat(-4, 4);
    asteroid->idFlags |= ID_ASTEROID_MASK;
    asteroid->health = 2;

    Animation* a;
    a = add_spritesheet("res/img/rock.png", 16, randomFloat(0.04f, 0.10f), 0, 0);
    asteroid->w = a->w;
    asteroid->h = a->h;

    asteroid->animation = a;

    return asteroid;
}

void asteroid_update(Entity* a) {
    if (a->animation != NULL) {
        animation_update(a);
    }
    check_bounds(a);

    a->x += a->dx;
    a->y += a->dy;
}

void asteroid_draw(Entity* a) {
    if (a->animation != NULL) {
        animation_draw(a);
    }
}

static void check_bounds(Entity* a) {
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

void swap_asteroid_animation(Entity* a) {
    a->animation = add_spritesheet("res/img/rock_small.png", 16, randomFloat(0.05f, 0.10f), 0, 0);
}