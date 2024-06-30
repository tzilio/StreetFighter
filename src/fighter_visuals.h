#ifndef __FIGHTER_VISUALS__
#define __FIGHTER_VISUALS__

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

//FIGHTER ANIMATIONS
void fighter_winner(fighter *player, int *movement);
void fighter_electric(fighter *player, int *movement);
void fighter_jump(fighter* player, fighter* opponent, int* movement);
void fighter_attack(fighter* player, fighter* opponent, int* movement, int* alreadyDamaged);
void update_player(fighter* player, fighter* opponent, int* frame, int* maxFrame, int* movement, int* previous_movement, int timer_count, int* alreadyDamaged, int *new_round);


// CHARACTER/POWER ANIMATIONS
void draw_fighter(ALLEGRO_BITMAP* p, fighter *player, int current, int movement, int inverted);
void draw_hadouken(ALLEGRO_BITMAP *h, int current, int movement, int inverted, float cx, float cy);
int frame_counter(int movement);

#endif
