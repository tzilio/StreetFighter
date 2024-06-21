#ifndef __FIGHTER__
#define __FIGHTER__

#include "joystick.h"
#include "power.h"
#include "display.h"

#define FIGHTER_STEP 5
#define GRAVITY 300
#define JUMP_SPEED -250.0	

#define IDLE 0
#define WALK 1
#define PUNCH 2
#define KICK 3
#define CROUCH 4
#define JUMP 5
#define JUMP_FWD 6
#define JUMP_BCK 7
#define CROUCH_PUNCH 8
#define CROUCH_KICK 9
#define JUMPING_PUNCH 10
#define JUMPING_KICK 11
#define HADOUKEN 12
#define ELECTROCUTED 13
#define WINNER 14
#define KNOCKED_DOWN 15
#define ELECTRIC_BALL 16
#define DEFENSE 17
#define CROUCH_DEFENSE 18

#define ATTACK_NONE 0
#define ATTACK_PUNCH 1
#define ATTACK_KICK 2
#define ATTACK_DOWN_PUNCH 3
#define ATTACK_DOWN_KICK 4
#define ATTACK_JUMPING_PUNCH 5
#define ATTACK_JUMPING_KICK 6
#define ATTACK_HADOUKEN 7

#define STATE_NORMAL 0
#define STATE_DEFENSIVE 1
#define STATE_AGGRESSIVE 2

#define CONTINUE 0
#define P1_WINNER 1
#define P2_WINNER 2
#define NEW_ROUND1 3
#define NEW_ROUND2 4
#define NEW_ROUND_DRAW 5
#define ROUND_WINNER 6

typedef struct {
    int side;    
    int width;
    int height;
    int front;

    int original_height;

    int health;
    float stamina;

    float x;
    float y;
    
    float speed_x;
    float speed_y;

    int isJumping;
    int isDown; 
    int isDefending;  
    int isElectrocuted;
    int isKnockedDown;
    int isWinner;
    int isTired;

    int state;
    int attack;
    int rounds_won;

    joystick* control;
    power* hand;
} fighter;


// FIGHTER INITIALIZATIONS
fighter* spawn_fighter(unsigned char width, unsigned char height, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char front);
void fighter_destroy(fighter *element);


// HANDLE INPUT
void handle_player_input(ALLEGRO_KEYBOARD_STATE* key_state, fighter *player, int *movement, int key_up, int key_down, int key_left, int key_right, int key_defense, int key_punch, int key_kick, int key_hadouken);
void handle_bot_input(fighter* p1, fighter* p2, int* movement);
void handle_input(ALLEGRO_KEYBOARD_STATE* key_state, fighter *player_1, fighter *player_2, int *m1, int *m2);


// FIGHTER STAMINA
void reduce_stamina(fighter *element, float amount);

// RESET 
void fighter_new_round(fighter *element, int x, int y);
void reset_fighter(fighter *element);


// FIGHTER/COMPUTER MOVEMENT
// check collision with bounds or player2
unsigned char collision_2D(fighter *element_first, fighter *element_second);

// check range of attack
bool isInRange(fighter *attacker, fighter *defender, int attack);

// given the keyboard input, move
void try_move_fighter(fighter *element, fighter *other, int steps, int direction);
void update_position(fighter *player_1, fighter *player_2, float time);
void fighter_move(fighter *element, char steps, unsigned char trajectory);

// gravity and collision with player 
void update_fighter(fighter *element, fighter *other, float time, unsigned short ground_level, unsigned short bounds, unsigned short ceil);

// computer agressiveness attacks 
void determine_state(fighter *p);
int choose_attack(fighter* p, int* movement);


// FIGHTER HADOUKEN
void fighter_hadouken(fighter *element);
void update_hadouken(fighter *player, fighter *opponent);
unsigned char check_hit (fighter *killer, fighter *victim);




#endif

