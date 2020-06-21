#include "../include/explosion.h"

entity_t* 
add_explosion(enum ExplosionType type, float x, float y) {
    entity_t *e;

    e = malloc(sizeof(entity_t));
    memset(e, 0, sizeof(entity_t));

    e->x = x;
    e->y = y;
    
    animation_t *a;

    switch (type) {
        case SMALL: a = add_spritesheet("res/img/explosions/type_A.png", 20, 0.015f, 0, 0); break;
        case MEDIUM: a = add_spritesheet("res/img/explosions/type_B.png", 64, 0.015f, 0, 0); break;
        case LARGE: a = add_spritesheet("res/img/explosions/type_C.png", 48, 0.015f, 0, 0); break;
        default: exit(EXIT_FAILURE);
    }

    e->animation = a;

    return e;
}


void 
explosion_update(entity_t *e) {
    if (e->animation != NULL) {
        if (e->animation->current_frame_id == e->animation->number_of_frames - 1) {
            e->flags |= DEATH_MASK;
            return;
        } else {
            animation_update(e);
        }
    }
}


void 
explosion_draw(entity_t *e) {
    if (e->animation != NULL) {
        animation_draw(e);
    }
}


void 
explosion_die(entity_t *e) {
    if (e->flags & DEATH_MASK) {
        free(e);
        return;
    }
}