#include "../include/player.h"
#include "../include/bullet.h"

static bool forwardMovement = false;

static void key_input_listener(void);
static void check_bounds(void);


void 
init_player(void) {
    if (player != NULL) {
        free(player);
    }
    
    player = malloc(sizeof(entity_t));
    memset(player, 0, sizeof(entity_t));

    player->x = SCREEN_WIDTH / 2;
    player->y = SCREEN_HEIGHT / 2;

    player->texture[0] = load_texture("res/img/ship.png");
    player->texture[1] = load_texture("res/img/ship_thrust.png");

    SDL_QueryTexture(player->texture[0], NULL, NULL, &player->w, &player->h);
}


void 
player_update(void) {
    if (player->flags & DEATH_MASK) {
        return;
    }

    key_input_listener();
    check_bounds();

    if (forwardMovement) {
        player->dx += (float) cos(to_radians(player->angle)) * 0.2f;
        player->dy += (float) sin(to_radians(player->angle)) * 0.2f;
    } else {
        player->dx *= 0.99f;
        player->dy *= 0.99f;
    }

    int maxSpeed = 15;
    float speed = (float) sqrt(player->dx * player->dx + player->dy * player->dy);

    if (speed > maxSpeed) {
        player->dx *= maxSpeed / speed;
        player->dy *= maxSpeed / speed;
    }

    player->x += player->dx;
    player->y += player->dy;
    player->life--;
}


void 
player_draw(void) {
    if (player->flags & DEATH_MASK) {
        return;
    }

    if (forwardMovement) {
        blit_texture_rotated(player->texture[1], player->x, player->y, player->angle);
    } else {
        blit_texture_rotated(player->texture[0], player->x, player->y, player->angle);
    }
}


/*
 *
 */
static void 
key_input_listener(void) {
    if (player == NULL) {
        return;
    }

    if (app.keyboard[SDL_SCANCODE_A]) {
        player->angle -= 3;
    }

    if (app.keyboard[SDL_SCANCODE_D]) {
        player->angle += 3;
    }

    if (app.keyboard[SDL_SCANCODE_SPACE] && player->life < 0) {
        player->life = FPS;

        entity_t *b;
        b = add_bullet(player->x, player->y, player->angle);
        stage.entity_tail->next = b;
        stage.entity_tail = b;
    }

    if (app.keyboard[SDL_SCANCODE_W]) {
        forwardMovement = true;
    } else {
        forwardMovement = false;
    }
}


/*
 *
 */
static void 
check_bounds(void) {
    if (player->x < 0) {
        player->x = SCREEN_WIDTH;
    }

    if (player->x > SCREEN_WIDTH) {
        player->x = 0;
    }

    if (player->y < 0) {
        player->y = SCREEN_HEIGHT;
    }

    if (player->y > SCREEN_HEIGHT) {
        player->y = 0;
    }
}