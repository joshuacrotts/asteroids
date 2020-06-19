#include "../include/bullet.h"

Entity* add_bullet(float x, float y, uint16_t angle) {
    Entity* b;

    b = malloc(sizeof(Entity));
    memset(b, 0, sizeof(Entity));

    b->x = x;
    b->y = y;

    b->angle = angle;
    b->idFlags |= ID_BULLET_MASK;
    b->texture[0] = loadTexture("res/img/blue_bullet.png");
    SDL_QueryTexture(b->texture[0], NULL, NULL, &b->w, &b->h);

    return b;
}

void bullet_update(Entity* b) {
    b->dx = (float) cos(toRadians(b->angle)) * 6;
    b->dy = (float) sin(toRadians(b->angle)) * 6;

    b->x += b->dx;
    b->y += b->dy;

    if (b->x < 0 || b->x + b->w > SCREEN_WIDTH || 
        b->y < 0 || b->y + b->h > SCREEN_HEIGHT) {
            b->flags |= DEATH_MASK;
        }
}

void bullet_draw(Entity* b) {
    blitRotated(b->texture[0], b->x, b->y, b->angle + 90);
}