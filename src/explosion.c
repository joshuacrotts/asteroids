#include "../include/explosion.h"

Entity* add_explosion(enum ExplosionType type, float x, float y) {
    Entity* e;

    e = malloc(sizeof(Entity));
    memset(e, 0, sizeof(Entity));

    e->x = x;
    e->y = y;
    
    Animation* a;

    switch (type) {
        case SMALL: a = add_spritesheet("res/img/explosions/type_A.png", 20, 0.015f, 0, 0); break;
        case MEDIUM: a = add_spritesheet("res/img/explosions/type_B.png", 64, 0.015f, 0, 0); break;
        case LARGE: a = add_spritesheet("res/img/explosions/type_C.png", 48, 0.015f, 0, 0); break;
        default: exit(EXIT_FAILURE);
    }

    e->animation = a;

    return e;
}

void explosion_update(Entity* e) {
    if (e->animation != NULL) {
        if (e->animation->currentFrameID == e->animation->numberOfFrames - 1) {
            e->flags |= DEATH_MASK;
            return;
        } else {
            animation_update(e);
        }
    }
}

void explosion_draw(Entity* e) {
    if (e->animation != NULL) {
        animation_draw(e);
    }
}

void explosion_die(Entity* e) {
    if (e->flags & DEATH_MASK) {
        free(e);
        return;
    }
}