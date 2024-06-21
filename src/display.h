#ifndef __STREET_FIGHTER__
#define __STREET_FIGHTER__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>

#include "fighter.h"
#include "joystick.h"

#define X_SCREEN 800
#define Y_SCREEN 336

#define GROUND_LEVEL 0

#define UP_1 23
#define DOWN_1 19
#define LEFT_1 1
#define RIGHT_1 4

#define UP_2 84
#define DOWN_2 85
#define LEFT_2 82
#define RIGHT_2 83

#define NUM_FRAMES 8  

#define MENU 0
#define BGND 1
#define FGHT 2
#define GAME 3
#define EXIT 4
#define PAUSE 5

#define MENU_SINGLE_PLAYER 0
#define MENU_MULTIPLAYER 1
#define MENU_EXIT 2

#define MODE_SINGLE_PLAYER 0
#define MODE_MULTIPLAYER 1

#define FARM 0
#define TEMPLE 1

#define RYU 0
#define KEN 1
#define CHUN 2
#define ZANGIEF 3

#define PLAYER_1 0
#define PLAYER_2 1

#define ROUND_1 0
#define ROUND_2 1
#define FINAL_ROUND 2
#define FIGHT 3
#define YOU_WIN 4
#define YOU_LOSE 5
#define KO 6
#define TIMER_OVER 7


#define NUM_SPRITES 10

// ANIMATED BACKGROUND STRUCT
typedef struct {
    ALLEGRO_BITMAP *frames[NUM_FRAMES];
    int current_frame;
    float frame_duration;
    double last_frame_update;
} background;


// MENU
void start_menu(ALLEGRO_FONT* font, int selected_option);
int menu_input(ALLEGRO_EVENT event, int *selected_option);

// PAUSE
int draw_pause(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue);

// CHARACTER SELECTION
void draw_fighter_selection(ALLEGRO_BITMAP *sprite_sheet, int sprite_index, int x, int y, int sprite_dimension, ALLEGRO_COLOR color);
void choose_fighter(ALLEGRO_FONT *font, int selected_option1, int selected_option2, ALLEGRO_BITMAP *heads, bool done1, bool done2);
void start_fight(ALLEGRO_BITMAP *heads, int selected_fighter1, int selected_fighter2);
int fighters_menu(ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_BITMAP** player1_sheet, ALLEGRO_BITMAP** player2_sheet);

// BACKGROUND SELECTION
void init_animated_background(background *bg, float frame_rate, const char *dir);
void update_animated_background(background *bg);
void draw_animated_background(background *bg);
void destroy_animated_background(background *bg);
void show_background_options(background *bg, ALLEGRO_FONT* font, int selected_option);
int choose_background(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE *queue, int *selected_option);

// BETWEEN ROUNDS
void draw_writings(ALLEGRO_BITMAP *writings, int selected_word1, int selected_word2);   
void display_winner(ALLEGRO_DISPLAY* disp, ALLEGRO_FONT* font, int winner);
void show_rounds(ALLEGRO_BITMAP *writings, int *current_round);

// HUD (IN GAME DISPLAY)
void draw_scoreboard(ALLEGRO_FONT *font, int health1, int health2, int points1, int points2, int round, int countdown);
void draw_stamina(ALLEGRO_FONT *font, float stamina1, float stamina2);


#endif
