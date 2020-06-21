#include "../include/main.h"
#include "../include/player.h"
#include "../include/explosion.h"
#include "../include/asteroid.h"
#include "../include/bullet.h"
#include "../include/hud.h"
#include "../lib/stds/include/background.h"
#include "../lib/stds/include/draw.h"

static int16_t stage_reset_timer;
static int16_t enemy_spawn_timer;
static background_t* background;

static void init_scene(void);
static void draw(void);
static void tick(void);
static void cleanup_stage(void);
static void reset_stage(void);

static void spawn_debris(entity_t*);
static void spawn_asteroids(uint16_t);
static void bullet_hit_asteroid(entity_t*);
static void asteroid_hit_player(entity_t*);

static void update_explosions(void);
static void update_entities(void);
static void update_debris(void);

static void draw_explosions(void);
static void draw_entities(void);
static void draw_debris(void);


// Barebones game. This is the minimum amount of code
// necessary to run a window.
int 
main(int argc, char* argv[]) {
  init_game("Standards C Library", SCREEN_WIDTH, SCREEN_HEIGHT);
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
static void 
init_scene(void) {
  app.delegate.tick = tick;
  app.delegate.draw = draw;

  memset(&stage, 0, sizeof(stage_t));
  app.texture_tail = &app.texture_head;
  stage.explosion_tail = &stage.explosion_head;
  stage.entity_tail = &stage.entity_head;
  stage.debris_tail = &stage.debris_head;
  stage_reset_timer = FPS * 3;
  background = init_background("res/img/bg/background.jpg");

  init_player();
  spawn_asteroids(random_int(6, 12));
}


/*
 *
 */
static void 
tick(void) {
  if (app.game_state == PAUSED) {
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
    spawn_asteroids(random_int(6, 12));
    enemy_spawn_timer = FPS * random_int(20, 30);
  }
}


/*
 *
 */
static void 
draw(void) {
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
static void 
reset_stage(void) {
  entity_t *e;
  debris_t *d;

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Reset started.\n");
  while(stage.debris_head.next) {
    d = stage.debris_head.next;
    stage.debris_head.next = d->next;
    free(d);
  }

  while (stage.entity_head.next) {
    e = stage.entity_head.next;
    stage.entity_head.next = e->next;
    free(e);
  }

  stage.debris_tail = &stage.debris_head;
  stage.entity_tail = &stage.entity_head;

  init_player();
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Reinitialization complete.\n");
  enemy_spawn_timer = 0;
  stage_reset_timer = FPS * 3;
}


/*
 *
 */
static void 
cleanup_stage(void) {
  background_die(background);
  entity_t *e;
  debris_t *d;

  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Reset started.\n");
  while(stage.debris_head.next) {
    d = stage.debris_head.next;
    stage.debris_head.next = d->next;
    free(d);
  }

  while (stage.entity_head.next) {
    e = stage.entity_head.next;
    stage.entity_head.next = e->next;
    free(e);
  }

  free(player);  
}


/*
 *
 */
static void 
spawn_asteroids(uint16_t n) {
  entity_t *a;

  int32_t posOffset = 100;
  int32_t squareDim = 200;
  
  for (int i = 0; i < n; i++) {

    // Continuously generate a point that isn't within a square
    // around the player to prevent instant deaths upon start or 
    // restart.
    int squareX = (int) (player->x - posOffset);
    int squareY = (int) (player->y - posOffset);
    int width = squareX + squareDim;
    int height = squareY + squareDim;

    float a_x;
    float a_y;
    do {
      a_x = random_float(0, SCREEN_WIDTH);
      a_y = random_float(0, SCREEN_HEIGHT);
    } while ((a_x >= squareX && a_y <= width) || (a_y >= squareY && a_y <= height));

    a = add_asteroid(a_x, a_y);
    stage.entity_tail->next = a;
    stage.entity_tail = a;
  }
}


/*
 *
 */
static void 
spawn_debris(entity_t *a) {
  debris_t *d;
  int x, y, w, h;

  w = a->w / 4;
  h = a->h / 4;

  for (y = 0; y <= h * 2; y += h) {
    for (x = 0; x <= w * 2; x += w) {
      d = malloc(sizeof(debris_t));
      memset(d, 0, sizeof(debris_t));
      stage.debris_tail->next = d;
      stage.debris_tail = d;

      d->x = a->x + a->w / 2;
      d->y = a->y + a->h / 2;
      d->dx = random_float(-5, 5);
      d->dy = random_float(-5, 5);
      d->life = FPS * 5;
      d->texture = a->animation->current_texture;

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
static void 
update_explosions(void) {
  entity_t *e;
  entity_t *prev;

  prev = &stage.explosion_head;

  for (e = stage.explosion_head.next; e != NULL; e = e->next) {
    explosion_update(e);

    if (e->flags & DEATH_MASK) {
      if (e == stage.explosion_tail) {
        stage.explosion_tail = prev;
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
static void 
update_entities(void) {
  entity_t *e;
  entity_t *prev;

  prev = &stage.entity_head;

  for (e = stage.entity_head.next; e != NULL; e = e->next) {
    if (e->flags & DEATH_MASK) {
      if (e == stage.entity_tail) {
        stage.entity_tail = prev;
      }
      prev->next = e->next;
      free(e);
      e = prev;
    }

    prev = e;

    if (e->id_flags & ID_BULLET_MASK) {
      bullet_update(e);
      bullet_hit_asteroid(e);
    }

    if (e->id_flags & ID_ASTEROID_MASK) {
      asteroid_update(e);
      asteroid_hit_player(e);
    }
  }
}


/*
 *
 */
static void 
update_debris(void) {
  debris_t *d;
  debris_t *prev;

  prev = &stage.debris_head;

  for (d = stage.debris_head.next; d != NULL; d = d->next) {
    d->x += d->dx;
    d->y += d->dy;

    d->dy += 0.1f;

    if (--d->life <= 0) {
      if (d == stage.debris_tail) {
        stage.debris_tail = prev;
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
static void 
draw_explosions(void) {
  entity_t *e;

  for (e = stage.explosion_head.next; e != NULL; e = e->next) {
    explosion_draw(e);
  }
}


/*
 *
 */
static void 
draw_entities(void) {
  entity_t *e;
  entity_t *prev;

  prev = &stage.entity_head;

  for (e = stage.entity_head.next; e != NULL; e = e->next) {
    if (e->id_flags & ID_BULLET_MASK) {
      bullet_draw(e);
    }

    if (e->id_flags & ID_ASTEROID_MASK) {
      asteroid_draw(e);
    }
  }
}


/*
 *
 */
static void 
draw_debris(void) {
  debris_t *d;

  for (d = stage.debris_head.next; d != NULL; d = d->next) {
    blit_rect(d->texture, &d->rect, d->x, d->y);
  }
}


/*
 *
 */
static void 
bullet_hit_asteroid(entity_t *b) {
  entity_t *a;

  for (a = stage.entity_head.next; a != NULL; a = a->next) {
    if (a->id_flags & ID_ASTEROID_MASK) {
      bool collided = collision((int32_t) a->x, (int32_t) a->y, a->w, a->h, 
                                (int32_t) b->x, (int32_t) b->y, b->w, b->h);

      if (collided) {
        a->health--;
        if (a->health == 0) {
          a->flags |= DEATH_MASK;
          entity_t *e;
          e = add_explosion(LARGE, a->x - a->w * 1.5f, a->y - a->h * 1.5f);
          stage.explosion_tail->next = e;
          stage.explosion_tail = e;
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
static void 
asteroid_hit_player(entity_t *a) {
  if (player->flags & DEATH_MASK) {
    return;
  }

  bool collided = collision((int32_t) a->x, (int32_t) a->y, a->w, a->h,
                            (int32_t) player->x, (int32_t) player->y, player->w, player->h);

  if (collided) {
    player->flags |= DEATH_MASK;

    entity_t *e;
    e = add_explosion(LARGE, player->x - a->w * 2.0f, player->y - a->h * 2.0f);
    stage.explosion_tail->next = e;
    stage.explosion_tail = e;    
  }
}