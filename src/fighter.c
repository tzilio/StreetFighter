#include <stdlib.h>
#include <stdio.h>
#include "fighter.h"
#include "fighter_visuals.h"


fighter* spawn_fighter(unsigned char width, unsigned char height, unsigned short x, unsigned short y, 
                                        unsigned short max_x, unsigned short max_y, unsigned char front){

    if ((x > max_x) || (y > max_y)) {
        fprintf(stderr, "Failed to allocate memory for player");                
        return NULL;
    }

    fighter *new_fighter = (fighter*) malloc(sizeof(fighter));
    if (!new_fighter) {
        fprintf(stderr, "Failed to allocate memory for player");        
        return NULL;
    }

    new_fighter->width = width;
    new_fighter->height = height;
    new_fighter->front = front;
    
    new_fighter->original_height = height;

    new_fighter->health = 1000;
    new_fighter->stamina = 100.0;

    new_fighter->x = x;
    new_fighter->y = y;

    new_fighter->speed_x = 0;       
    new_fighter->speed_y = 0;

    new_fighter->attack = 0;

    new_fighter->isJumping = 1;
    new_fighter->isDown = 0;
    new_fighter->isDefending = 0;

    new_fighter->isElectrocuted = 0;
    new_fighter->isKnockedDown = 0;
    new_fighter->isWinner = 0;
    new_fighter->isTired = 1;


    new_fighter->rounds_won = 0;

    new_fighter->hand = power_create();
    new_fighter->control = joystick_create();

    return new_fighter;
}


void fighter_destroy(fighter *element){
    power_destroy(element->hand); 
    joystick_destroy(element->control);
    free(element);
}

// HANDLE INPUT
void handle_player_input(ALLEGRO_KEYBOARD_STATE* key_state, fighter *player, int *movement, int key_up, int key_down, int key_left, int key_right, int key_defense, int key_punch, int key_kick, int key_hadouken) {
    // Movimentos do jogador
    if (al_key_down(key_state, key_up) && player->stamina > 5.0) {
        if (al_key_down(key_state, key_right)) {
            joystick_up_right(player->control);
            *movement = JUMP_FWD;
        } else if (al_key_down(key_state, key_left)) {
            joystick_up_left(player->control);
            *movement = JUMP_BCK;
        } else {
            joystick_up(player->control);
            *movement = JUMP;
        }
    } else if (al_key_down(key_state, key_down)) {
        joystick_down(player->control);
        *movement = CROUCH;
    } else if ((player->x + player->width - 0.1 < X_SCREEN) && (player->x - player->width + 0.1 > 0))  {
        if (al_key_down(key_state, key_left)) {
            joystick_left(player->control);
            *movement = WALK;
        } else if (al_key_down(key_state, key_right)) {
            joystick_right(player->control);
            *movement = WALK;
        }
    } else {
        *movement = IDLE;
    }

    // Defesa do jogador
    if (al_key_down(key_state, key_defense)) {
        player->isDefending = 1;
        if (*movement == CROUCH) {
            *movement = CROUCH_DEFENSE;
        } else {
            *movement = DEFENSE;
        }
    } else {
        player->isDefending = 0;
    }

    // Ataques do jogador
    if (al_key_down(key_state, key_punch))
        player->attack = ATTACK_PUNCH;
    else if (al_key_down(key_state, key_kick))
        player->attack = ATTACK_KICK;
    else if (al_key_down(key_state, key_hadouken))
        player->attack = ATTACK_HADOUKEN;
    else if (!al_key_down(key_state, key_up) && !al_key_down(key_state, key_down)
            && !al_key_down(key_state, key_left) && !al_key_down(key_state, key_right)
            && !al_key_down(key_state, key_defense))
        *movement = IDLE;

}

void handle_bot_input(fighter* p1, fighter* p2, int* movement) {
    int distance = p1->x - p2->x;
    determine_state(p2); // Determine the state of the bot

    if (p2->state == STATE_DEFENSIVE) {
        // Defensive mode, tries to move away from player 1 and crouch
        if (distance > 0) {
            *movement = WALK;
            joystick_left(p2->control);
        } else {
            *movement = WALK;
            joystick_right(p2->control);
        }

        if (abs(distance) < 100 && p2->attack == 0) {
            int action_choice = rand() % 10; // Randomly choose between crouching and attacking
            if (action_choice == 0) {
                *movement = CROUCH;
                p2->isDown = 1;
            } else if (action_choice == 1){
                choose_attack(p2, movement);
            }
        }
        if (abs(distance) <= 100 && p2->attack == 0) {
            int action_attack = rand() % 50;
            if (action_attack == 0) 
                choose_attack(p2, movement);        
        }

    } else if (p2->state == STATE_AGGRESSIVE) {
        // Aggressive mode, tries to get close and attack player 1
        if (abs(distance) > 45 && p2->attack == 0) {
            if (distance > 0) {
                *movement = WALK;
                joystick_right(p2->control);
            } else {
                *movement = WALK;
                joystick_left(p2->control);
            }
        } 
        if (abs(distance) <= 65 && p2->attack == 0) {
            // Attack if close enough
            choose_attack(p2, movement);
        }

    } else {
        // Normal state, default behavior
        if (abs(distance) > 60) {
            if (distance > 0) {
                *movement = WALK;
                joystick_right(p2->control);
            } else {
                *movement = WALK;
                joystick_left(p2->control);
            }
        } else {
            *movement = IDLE;
            reset_fighter(p2);
        }
        
        if (abs(distance) <= 70 && p2->attack == 0) {
            choose_attack(p2, movement);
        }
    }
}

void handle_input(ALLEGRO_KEYBOARD_STATE* key_state, fighter *player_1, fighter *player_2, int *m1, int *m2) {
    al_get_keyboard_state(key_state);    
    
    handle_player_input(key_state, player_1, m1, ALLEGRO_KEY_W, ALLEGRO_KEY_S, ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_E, ALLEGRO_KEY_R, ALLEGRO_KEY_F, ALLEGRO_KEY_C);
        
    if (!player_2) return;    
    handle_player_input(key_state, player_2, m2, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_O, ALLEGRO_KEY_P, ALLEGRO_KEY_L, ALLEGRO_KEY_M);
}

// ATRIBUTES RESET
void reset_fighter(fighter *element) {
    element->control->right = 0;
    element->control->left = 0;	
    element->control->up = 0;
    element->control->down = 0;
    element->control->up_left = 0;
    element->control->up_right = 0;
    element->control->hadouken = 0;
}


void fighter_new_round(fighter* player, int x, int y) {
    player->x = x;
    player->y = y;
    player->health = 1000;
    player->attack = 0;
    player->isKnockedDown = 0;
    player->isWinner = 0;
    player->isElectrocuted = 0;
    player->stamina = 100.0;

}

void reduce_stamina(fighter *element, float amount) {
   if (element->stamina > amount) element->stamina -= amount;
   else element->stamina = 0;
}

// PLAYER/COMPUTER MOVEMENTS -----------------------------------------------------
// RANGE TO ATTACK
bool isInRange(fighter *attacker, fighter *defender, int attack) {
    int attack_radius; // raio do círculo de ataque

    switch (attack) {
        case PUNCH:
            attack_radius = 20; // Raio do círculo de um soco
            break;
        case KICK:
            attack_radius = 30; // Raio do círculo de um chute
            break;
        case CROUCH_PUNCH:
            attack_radius = 15;
            break;
        case CROUCH_KICK:
            attack_radius = 25;
            break;
        case JUMPING_PUNCH:
            attack_radius = 25;
            break;
        case JUMPING_KICK:
            attack_radius = 35;
            break;
        default:
            return false; // Ataque inválido
    }

    if (defender->isDown) attack_radius -= 5;

    // Calcula a distância entre os centros dos dois personagens
    double distance = sqrt(pow(attacker->x - defender->x, 2) + pow(attacker->y - defender->y, 2));

    // Verifica se a distância é menor ou igual à soma dos raios dos círculos
    bool in_range = distance <= (attack_radius + 61); // 61 é a largura do atacante

    return in_range;
}

// CHECK COLLISION WITH BOUNDS OR OPPONENT
unsigned char collision_2D(fighter *element_first, fighter *element_second) {
    // Determine the edges of the bounding boxes using width and height
    float left1 = element_first->x - element_first->width / 2;
    float right1 = element_first->x + element_first->width / 2;
    float top1 = element_first->y - element_first->height / 2;
    float bottom1 = element_first->y + element_first->original_height / 2;

    float left2 = element_second->x - element_second->width / 2;
    float right2 = element_second->x + element_second->width / 2;
    float top2 = element_second->y - element_second->height / 2;
    float bottom2 = element_second->y + element_second->original_height / 2;

    // Check for horizontal (lateral) overlap
    bool horizontal_overlap = (left1 < right2 && right1 > left2);
    // Check for vertical overlap
    bool vertical_overlap = (top1 < bottom2 && bottom1 > top2);

    // If both overlaps exist, there is a collision
    if (horizontal_overlap && vertical_overlap) {
        // Determine if the collision is more lateral or vertical
        float horizontal_distance = fmin(right1 - left2, right2 - left1);
        float vertical_distance = fmin(bottom1 - top2, bottom2 - top1);

        if (horizontal_distance < vertical_distance) {
            return 1; // Lateral collision
        } else {
            return 2; // Vertical collision
        }
    }

    // No collision
    return 0;
}


void update_fighter(fighter *element, fighter *other, float time, unsigned short ground_level, unsigned short bounds, unsigned short ceil) {

    // Atualiza a posição do elemento se não houver colisão
    if (!collision_2D(element, other)) {
        // Aplica gravidade
        element->speed_y += GRAVITY * time;

        // Salva a posição anterior
        float previous_y = element->y;
        float previous_x = element->x;

        // Atualiza a posição do elemento
        element->x += element->speed_x;
        element->y += element->speed_y * time;

        // Verifica se o elemento atingiu o chão
        if (element->y >= ground_level) {
            element->y = ground_level;
            element->isJumping = 0;
            element->speed_y = 0;
            element->speed_x = 0;
            element->isTired = 0;
        }
        if (element->y <= ceil) { 
            element->y = ceil;
        }

        // Verifica se o elemento atingiu os limites laterais
        if (element->x + element->width >= bounds) {
            element->x = bounds - element->width;
            element->speed_x = 0;
        } else if (element->x - element->width <= 0) {
            element->x = element->width;
            element->speed_x = 0;
        }

        if (other->isDown && collision_2D(element, other)) {
            if (element->x > other->x) element->x = other->x + other->width;
            else element->x = other->x - other->width;
        }

        // Verifica colisão após a atualização das posições
        if (collision_2D(element, other)) {  
            if ((collision_2D(element, other) == 2 && !(element->isJumping && other->isJumping))) element->isJumping = 0;
            element->x = previous_x;
            element->y = previous_y;
            element->speed_x = 0;
            element->speed_y = 0;
        }
    }
}

void fighter_move(fighter *element, char steps, unsigned char trajectory) {
    switch (trajectory) {
        case 0:  // Move left    
        if (!element->isJumping)
                element->x -= steps * FIGHTER_STEP;
            break;

        case 1: // Move Right
        if (!element->isJumping) 
                element->x += steps * FIGHTER_STEP;
            break;

        case 2: // Move Up 
        if (!element->isJumping) {
                element->isJumping = 1;
                element->speed_y = JUMP_SPEED;
            }
            break;

        case 3: // Move Down
        if (!element->isJumping) {
                element->height = 40;  
                element->isDown = 1;
            }
            break;

        case 4: //Move Up left 
         if (!element->isJumping) {
                element->isJumping = 2;
                element->speed_y = JUMP_SPEED;
                element->speed_x = -steps;
            }
            break; 
    
        case 5: // Move Up Right
        if (!element->isJumping) {
                element->isJumping = 3;
                element->speed_y = JUMP_SPEED;
                element->speed_x = steps;
            }
            break;     
               
        default:
            break;           
    }
}


void try_move_fighter(fighter *element, fighter *other, int steps, int direction) {
    fighter_move(element, steps, direction);
    if (collision_2D(element, other))
        fighter_move(element, -steps, direction);
}


void update_position(fighter *player_1, fighter *player_2, float time){

    //Player 1
    if (player_1->control->left)
        try_move_fighter(player_1, player_2, 1, 0);
    
    if (player_1->control->right)
        try_move_fighter(player_1, player_2, 1, 1); 
    
    if (player_1->control->up)
        try_move_fighter(player_1, player_2, 1, 2);
    
    if (player_1->control->down)
        try_move_fighter(player_1, player_2, 1, 3);

    if (player_1->control->up_left) 
        try_move_fighter(player_1, player_2, 1, 4);

    if (player_1->control->up_right)
        try_move_fighter(player_1, player_2, 1, 5);
   
    // Player 2
    if (player_2->control->left)
        try_move_fighter(player_2, player_1, 1, 0);
    
    if (player_2->control->right)
        try_move_fighter(player_2, player_1, 1, 1);
    
    if (player_2->control->up)
        try_move_fighter(player_2, player_1, 1, 2);

    if (player_2->control->down)
        try_move_fighter(player_2, player_1, 1, 3);

    if (player_2->control->up_left) 
        try_move_fighter(player_2, player_1, 1, 4);

    if (player_2->control->up_right)
        try_move_fighter(player_2, player_1, 1, 5);

    //Hadouken
	if (player_1->control->hadouken)
		if (!player_1->hand->timer){
			fighter_hadouken(player_1); 
			player_1->hand->timer = POWER_COOLDOWN;
		} 

	if (player_2->control->hadouken){
		if (!player_2->hand->timer){
			fighter_hadouken(player_2);
			player_2->hand->timer = POWER_COOLDOWN;
		}
	}

    update_hadouken(player_1, player_2);
    update_hadouken(player_2, player_1);

    update_fighter(player_1, player_2, time, Y_SCREEN - player_1->original_height/2, X_SCREEN, 0);
    update_fighter(player_2, player_1, time, Y_SCREEN - player_2->original_height/2, X_SCREEN, 0); 
}



// COMPUTER MOVEMENTS
void determine_state(fighter *p) {
    if (p->health > 750) {
        p->state = STATE_AGGRESSIVE;
    } else if (p->health > 250) {
        p->state = STATE_NORMAL;
    } else {
        p->state = STATE_DEFENSIVE;
    }
}

int choose_attack(fighter* p, int* movement) {

    int attack_choice = rand() % 6; // Aleatoriamente escolhe entre os ataques
    switch (attack_choice) {
        case 0:
            *movement = PUNCH;
            p->attack = ATTACK_PUNCH;
            break;
        case 1:
            *movement = KICK;
            p->attack = ATTACK_KICK;
            break;
        case 2:
            *movement = JUMPING_PUNCH;
            p->attack = ATTACK_JUMPING_PUNCH;
            joystick_up(p->control);
            break;
        case 3:
            *movement = JUMPING_KICK;
            p->attack = ATTACK_JUMPING_KICK;
            joystick_up(p->control);
            break;
        case 4:
            *movement = CROUCH_PUNCH;
            p->attack = ATTACK_DOWN_PUNCH;
            joystick_down(p->control);
            p->isDown = 1;
            break;
        case 5:
            *movement = CROUCH_KICK;
            p->attack = ATTACK_DOWN_KICK;
            joystick_down(p->control);
            p->isDown = 1;
            break;
        default:
            *movement = IDLE;
            break;
    }
    return *movement;
}



// FIGHTER HADOUKEN
void fighter_hadouken(fighter *element) {
    hadouken *h;
    if (!element->front) h = power_shot(element->x - element->height/2, element->y, element->front, element->hand);
    else if (element->front == 1) h = power_shot(element->x + element->height/2, element->y, element->front, element->hand);
    if (h) element->hand->shots = h;
}

unsigned char check_hit (fighter *killer, fighter *victim) {
    for (hadouken *index = killer->hand->shots; index != NULL; index = (hadouken*) index->next)
        if ((index->x >= victim->x - victim->width/2) && (index->x <= victim->x + victim->width/2 ) &&
		   (index->y >= victim->y - victim->height/2) && (index->y <= victim->y + victim->height/2)) {
            if (victim->health > 90) victim->health -= 90;
            else victim->health = 0;
            victim->isElectrocuted = 1;
            return 1;
        }
    victim->isElectrocuted = 0;
    return 0;   
}

void update_hadouken(fighter *player, fighter *opponent) {
    hadouken *previous = NULL;   
    for (hadouken *index = player->hand->shots; index != NULL;) {
        if (!index->trajectory) index->x -= HADOUKEN_MOVE;
        else if (index->trajectory == 1) index->x += HADOUKEN_MOVE; 

        if ((index->x > X_SCREEN) || check_hit(player, opponent)) {
            if (previous) {
                previous->next = index->next;
                hadouken_destroy(index);
                index = (hadouken*) previous->next;            
            } else {
                player->hand->shots = (hadouken*) index->next;
                hadouken_destroy(index);
                index = player->hand->shots;
            }    
        } else {
            previous = index;
            index = (hadouken*) index->next;
        }        
    }
}


