#include "fighter_visuals.h"

// COUNT FRAMES FOR EACH MOVEMENT
int frame_counter(int movement) {
    switch (movement) {
        case IDLE:
            return 3;
            break;
        case WALK:
            return 5;
            break;
        case JUMP:
            return 7;
            break;
        case KICK:
            return 4;
            break;
        case PUNCH:
            return 3;
            break;
        case JUMP_FWD:
            return 8;
            break;
        case JUMP_BCK:
            return 8;
            break;
        case CROUCH:
            return 1;
            break;
        case CROUCH_PUNCH:
            return 3;
            break;
        case CROUCH_KICK:
            return 3;
            break;
        case JUMPING_PUNCH:
            return 3;
            break;
        case JUMPING_KICK:
            return 3;
            break;
        case HADOUKEN:
            return 4;
            break;
        case ELECTROCUTED:
            return 4;
            break;
        case WINNER:
            return 9;
            break;
        case KNOCKED_DOWN:
            return 9;
            break;
        case ELECTRIC_BALL:
            return 1;
            break;
        case DEFENSE:
            return 1;
            break;
        case CROUCH_DEFENSE:
            return 1;
            break;
        default:
            return -1000;
            break;
    }   
}

// CHARACTER ANIMATIONS
void fighter_winner(fighter *player, int *movement) {
    if (player->isWinner)
        *movement = WINNER;
    if (player->isKnockedDown)
        *movement = KNOCKED_DOWN;
}

void fighter_electric(fighter *player, int *movement) {
    if (player->isElectrocuted)
        *movement = ELECTROCUTED;   
}

void fighter_jump(fighter* player, fighter* opponent, int* movement) {
        
    if (player->isJumping == 1) {
        *movement = JUMP;
    } else if (player->isJumping == 2 && (player->x > opponent->x)) {
        *movement = JUMP_FWD;
    } else if (player->isJumping == 3 && (player->x > opponent->x)) {
        *movement = JUMP_BCK;
    } else if (player->isJumping == 2) {
        *movement = JUMP_BCK;
    } else if (player->isJumping == 3) {
        *movement = JUMP_FWD;
    }
    if ((player->isJumping) && (!player->isTired)) {
        reduce_stamina(player, 5.0);
        player->isTired = 1; // Marca a flag
    }
}

void fighter_attack(fighter* player, fighter* opponent, int* movement, int* alreadyDamaged, int* frame, int* maxFrame) {
    if (player->stamina < 10.0) return;
    
    int damage = 30; // Dano padrão

    if (*movement == JUMP || *movement == JUMP_BCK || *movement == JUMP_FWD) {
        if (player->attack == ATTACK_PUNCH) {
            *movement = JUMPING_PUNCH;
        } else if (player->attack == ATTACK_KICK) {
            *movement = JUMPING_KICK;
        }
    } else if (*movement == CROUCH || *movement == CROUCH_DEFENSE) {
        if (player->attack == ATTACK_PUNCH) {
            *movement = CROUCH_PUNCH;
        } else if (player->attack == ATTACK_KICK) {
            *movement = CROUCH_KICK;
        } else {
            *alreadyDamaged = 0;
            return;        
        }
    } else {
        if (player->attack == ATTACK_KICK) {
            *movement = KICK;
        } else if (player->attack == ATTACK_PUNCH) {
            *movement = PUNCH;
        } else if (player->attack == ATTACK_HADOUKEN && player->hand->timer == 0) {
            *movement = HADOUKEN;             
        } else {
            *alreadyDamaged = 0;
            return; // Saia da função se não houver movimento de ataque
        }
    }

    // Verifique se o movimento está dentro do alcance e se o oponente não foi danificado ainda
    if (*frame == *maxFrame - 1 && isInRange(player, opponent, *movement) && !(*alreadyDamaged)) {
        *alreadyDamaged = 1;
        // Reduza o dano pela metade se o jogador estiver defendendo
        if (opponent->isDefending) {
            damage /= 2;
        }
        // Inflija o dano ao oponente
        if (opponent->health >= damage) {
            opponent->health -= damage;
        } else {
            opponent->health = 0;
        }
        *movement = IDLE;
        reduce_stamina(player, 10.0);
    }
}

void update_player(fighter* player, fighter* opponent, int* frame, int* maxFrame, int* movement, 
                   int* previous_movement, int timer_count, int* alreadyDamaged, int *new_round) {

    fighter_jump(player, opponent, movement);
    fighter_attack(player, opponent, movement, alreadyDamaged, frame, maxFrame);
    fighter_electric(player, movement);
    fighter_winner(player, movement); 

    if (timer_count % 10 == 0) {
        (*frame)++;
        if (*movement == HADOUKEN && *frame == *maxFrame && player->stamina > 20.0 && player->hand->timer == 0) {
            joystick_hadouken(player->control);
            reduce_stamina(player, 20.0);
        }
        if (*movement == ELECTROCUTED && *frame == *maxFrame) player->isElectrocuted = 0;
        if (*movement == WINNER && *frame == *maxFrame) *new_round = 1;
        if (*frame >= *maxFrame) {
            if (player->isJumping) *frame = *maxFrame - 1; 
            else *frame = 0; 
            player->attack = 0; 
        }
    }

    if (*movement != CROUCH && *movement != CROUCH_DEFENSE) {
        player->isDown = 0;  
        player->height = player->original_height;   
    }

    // Se o movimento mudou, reinicia o frame e atualiza o maxFrame
    if (*movement != *previous_movement) {
        *frame = 0;
        *maxFrame = frame_counter(*movement);
    }
    *previous_movement = *movement;
}


// CHARACTER/POWER DRAWINGS
void draw_fighter(ALLEGRO_BITMAP* p, fighter *player, int current, int movement, int inverted) {
    if (inverted) al_draw_bitmap_region(p, 112*current, 112*movement, 112, 112, player->x - 80, player->y - 72, inverted);  
    else al_draw_bitmap_region(p, 112*current, 112*movement, 112, 112, player->x - 30, player->y - 72, inverted);
}

void draw_hadouken(ALLEGRO_BITMAP *h, int current, int movement, int inverted, float cx, float cy) {
    if (inverted) al_draw_bitmap_region(h, 112*current, 112*movement, 112, 112, cx - 80, cy - 60, inverted);  
    else al_draw_bitmap_region(h, 112*current, 112*movement, 112, 112, cx - 20, cy - 60, inverted);
}

