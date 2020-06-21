#include "../include/hud.h"

static void check_paused(void);
static void draw_paused(void);


void
draw_HUD(void) {
    check_paused();
    
    if (app.game_state == PAUSED) {
        draw_paused();
    }
}


/*
 *
 */
static void 
check_paused(void) {
    if (app.keyboard[SDL_SCANCODE_ESCAPE]) {
        app.game_state = (app.game_state == PAUSED) ? RUNNING : PAUSED;
        app.keyboard[SDL_SCANCODE_ESCAPE] = 0;
    }
}


/*
 *
 */
static void 
draw_paused(void) {
    SDL_Rect screen;
    screen.x = 0;
    screen.y = 0;
    screen.w = SCREEN_WIDTH;
    screen.h = SCREEN_HEIGHT;

    draw_rect(&screen, 0, 0, 0, 128, true);
    int32_t fw, fh;
    get_string_size("PAUSED", "res/fonts/nes.ttf", 24, &fw, &fh);
    draw_text(SCREEN_WIDTH / 2 - fw / 2, SCREEN_HEIGHT / 2, 0xff, 0xff, 0xff, "res/fonts/nes.ttf", 24, "PAUSED");
}