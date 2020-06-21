#include "../include/bullet.h"


entity_t* 
add_bullet(float x, float y, uint16_t angle) {
    entity_t *b;

    b = malloc(sizeof(entity_t));
    memset(b, 0, sizeof(entity_t));

    b->x = x;
    b->y = y;

    b->angle = angle;
    b->id_flags |= ID_BULLET_MASK;
    b->texture[0] = load_texture("res/img/blue_bullet.png");
    SDL_QueryTexture(b->texture[0], NULL, NULL, &b->w, &b->h);

    return b;
}


void 
bullet_update(entity_t *b) {
    b->dx = (float) cos(to_radians(b->angle)) * 6;
    b->dy = (float) sin(to_radians(b->angle)) * 6;

    b->x += b->dx;
    b->y += b->dy;

    if (b->x < 0 || b->x + b->w > SCREEN_WIDTH || 
        b->y < 0 || b->y + b->h > SCREEN_HEIGHT) {
            b->flags |= DEATH_MASK;
        }
}


void 
bullet_draw(entity_t* b) {
    blit_texture_rotated(b->texture[0], b->x, b->y, b->angle + 90);
}