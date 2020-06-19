#include "../include/main.h"
#include "../include/player.h"
#include "../include/explosion.h"
#include "../include/asteroid.h"
#include "../include/bullet.h"
#include "../include/hud.h"
#include "../lib/stds/include/background.h"
#include "../lib/stds/include/draw.h"

static void init_scene(void);
static void draw(void);
static void tick(void);
static void cleanup_stage(void);
static void reset_stage(void);

static void spawn_debris(Entity*);
static void spawn_asteroids(uint16_t);
static void bullet_hit_asteroid(Entity*);
static void asteroid_hit_player(Entity*);

static void update_explosions(void);
static void update_entities(void);
static void update_debris(void);

static void draw_explosions(void);
static void draw_entities(void);
static void draw_debris(void);

static int16_t stage_reset_timer;
static int16_t enemy_spawn_timer;
static Background* background;

// Barebones game. This is the minimum amount of code
// necessary to run a window.
int main(int argc, char* argv[]) {
  initGame("Standards C Library", SCREEN_WIDTH, SCREEN_HEIGHT);
  init_scene();
  loop();

  atexit(cleanup_stage);
  return 0;
}

/*
 * Initializes the delegation structure to use the
 * tick and draw methods for rendering and updating
 * the game. These methods must be defined because the
 * delegation structure uses them in the loop function.
 *
 * Sprites and structs are also initalized here.
 */
static void init_scene(void) {
  app.delegate.tick = tick;
  app.delegate.draw = draw;

  memset(&stage, 0, sizeof(Stage));
  app.textureTail = &app.textureHead;
  stage.explosionTail = &stage.explosionHead;
  stage.entityTail = &stage.entityHead;
  stage.debrisTail = &stage.debrisHead;
  stage_reset_timer = FPS * 3;
  background = init_background("res/img/bg/background.jpg");

  init_player();
  spawn_asteroids(randomInt(6, 12));
}

/*
 *
 */
static void tick(void) {
  if (app.gameState == PAUSED) {
    return;
  }

  if (player->flags & DEATH_MASK) {
    if (--stage_reset_timer < 0) {
      reset_stage();
    }
  }

  background_update(background);
  player_update();
  update_explosions();
  update_entities();
  update_debris();

  if (--enemy_spawn_timer < 0) {
    spawn_asteroids(randomInt(6, 12));
    enemy_spawn_timer = FPS * randomInt(20, 30);
  }
}

/*
 *
 */
static void draw(void) {
  background_draw(background);
  player_draw();
  draw_explosions();
  draw_entities();
  draw_debris();
  draw_HUD();
}

/*
 *
 */
static void reset_stage(void) {
  Entity* e;
  Debris* d;

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Reset started.\n");
  while(stage.debrisHead.next) {
    d = stage.debrisHead.next;
    stage.debrisHead.next = d->next;
    free(d);
  }

  while (stage.entityHead.next) {
    e = stage.entityHead.next;
    stage.entityHead.next = e->next;
    free(e);
  }

  stage.debrisTail = &stage.debrisHead;
  stage.entityTail = &stage.entityHead;

  init_player();
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Reinitialization complete.\n");
  enemy_spawn_timer = 0;
  stage_reset_timer = FPS * 3;
}

static void cleanup_stage(void) {
  background_die(background);
  Entity* e;
  Debris* d;

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Reset started.\n");
  while(stage.debrisHead.next) {
    d = stage.debrisHead.next;
    stage.debrisHead.next = d->next;
    free(d);
  }

  while (stage.entityHead.next) {
    e = stage.entityHead.next;
    stage.entityHead.next = e->next;
    free(e);
  }

  free(player);  
}

/*
 *
 */
static void spawn_asteroids(uint16_t n) {
  Entity* a;
  
  for (int i = 0; i < n; i++) {
    a = add_asteroid(randomFloat(0, SCREEN_WIDTH), randomFloat(0, SCREEN_HEIGHT));
    stage.entityTail->next = a;
    stage.entityTail = a;
  }
}

/*
 *
 */
static void spawn_debris(Entity* a) {
  Debris* d;
  int x, y, w, h;

  w = a->w / 4;
  h = a->h / 4;

  for (y = 0; y <= h * 2; y += h) {
    for (x = 0; x <= w * 2; x += w) {
      d = malloc(sizeof(Debris));
      memset(d, 0, sizeof(Debris));
      stage.debrisTail->next = d;
      stage.debrisTail = d;

      d->x = a->x + a->w / 2;
      d->y = a->y + a->h / 2;
      d->dx = randomFloat(-5, 5);
      d->dy = randomFloat(-5, 5);
      d->life = FPS * 5;
      d->texture = a->animation->currentTexture;

      d->rect.x = x;
      d->rect.y = y;
      d->rect.w = w;
      d->rect.h = h;
    }  
  }
}

/*
 *
 */
static void update_explosions(void) {
  Entity* e;
  Entity* prev;

  prev = &stage.explosionHead;

  for (e = stage.explosionHead.next; e != NULL; e = e->next) {
    explosion_update(e);

    if (e->flags & DEATH_MASK) {
      if (e == stage.explosionTail) {
        stage.explosionTail = prev;
      }
      
      prev->next = e->next;
      explosion_die(e);
      e = prev;
    }

    prev = e;
  }
}

/*
 *
 */
static void update_entities(void) {
  Entity* e;
  Entity* prev;

  prev = &stage.entityHead;

  for (e = stage.entityHead.next; e != NULL; e = e->next) {
    if (e->flags & DEATH_MASK) {
      if (e == stage.entityTail) {
        stage.entityTail = prev;
      }
      prev->next = e->next;
      free(e);
      e = prev;
    }

    prev = e;

    if (e->idFlags & ID_BULLET_MASK) {
      bullet_update(e);
      bullet_hit_asteroid(e);
    }

    if (e->idFlags & ID_ASTEROID_MASK) {
      asteroid_update(e);
      asteroid_hit_player(e);
    }
  }
}

/*
 *
 */
static void update_debris(void) {
  Debris* d;
  Debris* prev;

  prev = &stage.debrisHead;

  for (d = stage.debrisHead.next; d != NULL; d = d->next) {
    d->x += d->dx;
    d->y += d->dy;

    d->dy += 0.1f;

    if (--d->life <= 0) {
      if (d == stage.debrisTail) {
        stage.debrisTail = prev;
      }

      prev->next = d->next;
      free(d);
      d = prev;
    }

    prev = d;
  }
}

/*
 *
 */
static void draw_explosions(void) {
  Entity* e;

  for (e = stage.explosionHead.next; e != NULL; e = e->next) {
    explosion_draw(e);
  }
}

/*
 *
 */
static void draw_entities(void) {
  Entity* e;
  Entity* prev;

  prev = &stage.entityHead;

  for (e = stage.entityHead.next; e != NULL; e = e->next) {
    if (e->idFlags & ID_BULLET_MASK) {
      bullet_draw(e);
    }

    if (e->idFlags & ID_ASTEROID_MASK) {
      asteroid_draw(e);
    }
  }
}

/*
 *
 */
static void draw_debris(void) {
  Debris* d;

  for (d = stage.debrisHead.next; d != NULL; d = d->next) {
    blitRect(d->texture, &d->rect, d->x, d->y);
  }
}

/*
 *
 */
static void bullet_hit_asteroid(Entity* b) {
  Entity* a;

  for (a = stage.entityHead.next; a != NULL; a = a->next) {
    if (a->idFlags & ID_ASTEROID_MASK) {
      bool collided = collision((int32_t) a->x, (int32_t) a->y, a->w, a->h, 
                                (int32_t) b->x, (int32_t) b->y, b->w, b->h);

      if (collided) {
        a->health--;
        if (a->health == 0) {
          a->flags |= DEATH_MASK;
          Entity* e;
          e = add_explosion(LARGE, a->x - a->w * 1.5f, a->y - a->h * 1.5f);
          stage.explosionTail->next = e;
          stage.explosionTail = e;
        } else {
          spawn_debris(a);
          swap_asteroid_animation(a);
        }

        b->flags |= DEATH_MASK;
        return;
      }
    } 
  }
}

/*
 *
 */
static void asteroid_hit_player(Entity* a) {
  if (player->flags & DEATH_MASK) {
    return;
  }

  bool collided = collision((int32_t) a->x, (int32_t) a->y, a->w, a->h,
                            (int32_t) player->x, (int32_t) player->y, player->w, player->h);

  if (collided) {
    player->flags |= DEATH_MASK;

    Entity* e;
    e = add_explosion(LARGE, player->x - a->w * 2.0f, player->y - a->h * 2.0f);
    stage.explosionTail->next = e;
    stage.explosionTail = e;    
  }
}