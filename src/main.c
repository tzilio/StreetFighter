#include "display.h"
#include "fighter_visuals.h"

void print_game_controls() {
    printf("\n---------------- GAME CONTROLS ----------------\n");
    printf("\n\n----------PLAYER 1----------\n\n");
    printf("W - Jump\n");
    printf("S - Crouch\n");
    printf("A - Move Left\n");
    printf("D - Move Right\n");
    printf("E - Block\n");
    printf("R - Punch\n");
    printf("F - Kick\n");
    printf("C - Special Move\n");

     printf("\n\n----------PLAYER 2----------\n\n");
    printf("Up Arrow - Jump\n");
    printf("Down Arrow - Crouch\n");
    printf("Left Arrow - Move Left\n");
    printf("Right Arrow - Move Right\n");
    printf("O - Block\n");
    printf("P - Punch\n");
    printf("L - Kick\n");
    printf("M - Special Move\n");
}

int run_rounds(fighter *player_1, fighter *player_2, int *timer_over) {    
    if (player_1->rounds_won >= 2 && player_2->rounds_won != player_1->rounds_won) {
        player_1->isWinner = 1;        
        return P1_WINNER;
    }
    if (player_2->rounds_won >= 2 && player_1->rounds_won != player_2->rounds_won) {
        player_2->isWinner = 1;
        return P2_WINNER;
    }
        
    if (player_1->isWinner) return NEW_ROUND1;
    if (player_2->isWinner) return NEW_ROUND2;

    if (!(*timer_over)) {
        if (player_2->health <= 0 && !player_1->isWinner)  {
            player_2->health = 0; // Evita que a vida fique negativa
            player_1->rounds_won++;
            player_1->isWinner = 1;
            player_2->isKnockedDown = 1;
        }
        if (player_1->health <= 0 && !player_2->isWinner) {
            player_1->health = 0; // Evita que a vida fique negativa
            player_2->rounds_won++;
            player_2->isWinner = 1;
            player_1->isKnockedDown = 1;
        }
    } else {
        *timer_over = 0;
        if (player_1->health > player_2->health && !player_1->isWinner) {
            player_1->rounds_won++;
            player_1->isWinner = 1;
            return NEW_ROUND1;
        } else if (player_2->health > player_1->health && !player_2->isWinner) {
            player_2->rounds_won++;
            player_2->isWinner = 1;
            return NEW_ROUND2;
        } else {
            player_1->rounds_won++;
            player_2->rounds_won++;
            player_1->isWinner = 1;     
            player_2->isWinner = 1;      
            return NEW_ROUND_DRAW;    
        }
    }
    return CONTINUE;
}

void new_round(fighter *player_1, fighter *player_2, int *current_round, double *countdown, bool* round_start, int* round_counter) {

    fighter_new_round(player_1, player_1->width/2, Y_SCREEN - player_1->height / 2);
    fighter_new_round(player_2, X_SCREEN - player_2->width/2, Y_SCREEN - player_1->height / 2);
    (*current_round)++;
    *countdown = 90.0;
    *round_start = true;
    *round_counter = 0;
}


void run_game(ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_FONT* font_rounds, fighter* player_1, fighter* player_2,
              int* continue_play, int selected_option, ALLEGRO_BITMAP* fighter1_sheet, ALLEGRO_BITMAP* fighter2_sheet, ALLEGRO_BITMAP* writings, const char* filename) {

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    ALLEGRO_KEYBOARD_STATE key_state;

    double last_time = al_get_time();
    int running = 1;
    
    int pause = 0;

    int frame1 = 0, frame2 = 0;
    int maxFrame1 = 5, maxFrame2 = 5;

    int movement1 = 0, movement2 = 0;
    int previous_movement1 = 0, previous_movement2 = 0;

    int alreadyDamaged1 = 0;
    int alreadyDamaged2 = 0;

    int begin_new_round = 0;
    int round_status = 0;
    int current_round = 1;

    double countdown = 90.0;
    int timer_count = 0;
    int timer_over = 0;

    bool waiting_for_round_start = true;
    double round_start_time = al_get_time();
    const double round_wait_duration = 2.0;

    bool round_start = true;  // Flag to control the round start delay
    int round_counter = 0;    // Counter for the round start delay

    bool displaying_winner = false;
    double winner_display_start_time = 0;
    const double winner_display_duration = 3.0;

    bool displaying_pause = false;

    double pause_start_time = 0;

    background bg;
    init_animated_background(&bg, 24.0, filename);

    show_rounds(writings, &current_round);

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *continue_play = 0;
            break;
        }

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            displaying_pause = !displaying_pause; // Alterna o estado de exibição do menu de pausa
            if (displaying_pause) {
                pause_start_time = al_get_time(); // Marca o tempo quando a pausa começou
                pause = draw_pause(font, queue);
                if (pause == EXIT) {
                    *continue_play = 0;
                    running = !running; 
                } else {
                    displaying_pause = !displaying_pause;
                    double pause_end_time = al_get_time(); // Marca o tempo quando a pausa terminou
                    double paused_duration = pause_end_time - pause_start_time; // Calcula a duração da pausa
                    countdown += paused_duration; // Adiciona a duração da pausa ao countdown
                }
                continue;            
            }           
        }

        if (displaying_winner) {
            double current_time = al_get_time();
            if (current_time - winner_display_start_time >= winner_display_duration) {
                displaying_winner = false;
                if (round_status == P1_WINNER || round_status == P2_WINNER) {
                    *continue_play = 0;
                    break;
                } else if (round_status == NEW_ROUND1 || round_status == NEW_ROUND2 || round_status == NEW_ROUND_DRAW) {
                    new_round(player_1, player_2, &current_round, &countdown, &round_start, &round_counter);              
                    waiting_for_round_start = true;
                    round_start_time = al_get_time();
                    show_rounds(writings, &current_round);
                }
                continue;
            } else continue; 
        }

        round_status = run_rounds(player_1, player_2, &timer_over);
        if (round_status != CONTINUE && begin_new_round) { 
            begin_new_round = 0;  
            display_winner(disp, font_rounds, round_status);
            displaying_winner = true;
            winner_display_start_time = al_get_time();
            continue;
        }

        if (event.type == ALLEGRO_EVENT_TIMER) {
            double current_time = al_get_time();
            double resulting_time = current_time - last_time;
            last_time = current_time;

            update_animated_background(&bg);
            al_clear_to_color(al_map_rgb(0, 0, 0));
            draw_animated_background(&bg);

            al_get_keyboard_state(&key_state);

            timer_count++;

            if (!waiting_for_round_start) {
                if (selected_option == MODE_SINGLE_PLAYER) {
                    handle_input(&key_state, player_1, NULL, &movement1, NULL);
                    handle_bot_input(player_1, player_2, &movement2);
                } else {
                    handle_input(&key_state, player_1, player_2, &movement1, &movement2);
                }

                update_player(player_1, player_2, &frame1, &maxFrame1, &movement1, &previous_movement1, timer_count, &alreadyDamaged1, &begin_new_round);
                update_player(player_2, player_1, &frame2, &maxFrame2, &movement2, &previous_movement2, timer_count, &alreadyDamaged2, &begin_new_round);
                
                update_position(player_1, player_2, resulting_time);
            }

            // Check if waiting period is over
            if (waiting_for_round_start) {
                if (current_time - round_start_time >= round_wait_duration) waiting_for_round_start = false;
                else continue;
            }

            /*al_draw_filled_rectangle(player_1->x - player_1->width / 2, player_1->y - player_1->height / 2,
                player_1->x + player_1->width / 2, player_1->y + player_1->original_height / 2, al_map_rgb(255, 0, 0));

            al_draw_filled_rectangle(player_2->x - player_2->width / 2, player_2->y - player_2->height / 2,
                player_2->x + player_2->width / 2, player_2->y + player_2->original_height / 2, al_map_rgb(0, 0, 255)); */
          

            if (player_1->x > player_2->x) {
                draw_fighter(fighter1_sheet, player_1, frame1, movement1, 1);
                draw_fighter(fighter2_sheet, player_2, frame2, movement2, 0);
                player_1->front = 0;
                player_2->front = 1;
            } else {
                draw_fighter(fighter1_sheet, player_1, frame1, movement1, 0);
                draw_fighter(fighter2_sheet, player_2, frame2, movement2, 1);
                player_1->front = 1;
                player_2->front = 0;
            }

            for (hadouken *index = player_1->hand->shots; index != NULL; index = (hadouken*) index->next) {
                //al_draw_filled_circle(index->x, index->y , 10, al_map_rgb(255, 0, 0));	
                draw_hadouken(fighter1_sheet, 1, ELECTRIC_BALL, !player_1->front, index->x + 20, index->y);
            }
            if (player_1->hand->timer) player_1->hand->timer--;

            for (hadouken *index = player_2->hand->shots; index != NULL; index = (hadouken*) index->next) {
                //al_draw_filled_circle(index->x , index->y, 10, al_map_rgb(0, 0, 255));	
                draw_hadouken(fighter2_sheet, 1, ELECTRIC_BALL, !player_2->front, index->x - 20, index->y);
            }
            if (player_2->hand->timer) player_2->hand->timer--;  
                        
            draw_scoreboard(font, player_1->health, player_2->health, player_1->rounds_won, player_2->rounds_won, current_round, countdown);
            draw_stamina(font, player_1->stamina, player_2->stamina);

            al_flip_display();
            
            reset_fighter(player_1);
            reset_fighter(player_2);

            previous_movement1 = movement1;
            previous_movement2 = movement2;

            countdown -= resulting_time;
            if (player_1->stamina < 100.0) player_1->stamina += resulting_time * 4.0;
            if (player_2->stamina < 100.0) player_2->stamina += resulting_time * 4.0;
            if (countdown <= 0) {
                timer_over = 1;   
                round_status = run_rounds(player_1, player_2, &timer_over);
                display_winner(disp, font_rounds, round_status);
                displaying_winner = true;
                winner_display_start_time = al_get_time();
            }
        }
    }

    al_destroy_timer(timer);
    destroy_animated_background(&bg);
}

void cleanup(ALLEGRO_DISPLAY* disp, ALLEGRO_EVENT_QUEUE* queue, ALLEGRO_FONT* font, ALLEGRO_FONT* font_rounds,
             ALLEGRO_BITMAP* icon, ALLEGRO_BITMAP* player1_sheet, ALLEGRO_BITMAP* player2_sheet, ALLEGRO_BITMAP* writings,
             fighter* player_1, fighter* player_2) {

    if (icon) al_destroy_bitmap(icon);
    if (font) al_destroy_font(font);
    if (font_rounds) al_destroy_font(font_rounds);
    if (disp) al_destroy_display(disp);
    if (queue) al_destroy_event_queue(queue);
    if (player1_sheet) al_destroy_bitmap(player1_sheet);
    if (player2_sheet) al_destroy_bitmap(player2_sheet);
    if (writings) al_destroy_bitmap(writings);

    if (player_1) fighter_destroy(player_1);
    if (player_2) fighter_destroy(player_2);
}

int main() {
    if (!al_init() || !al_install_keyboard() || !al_init_primitives_addon() ||
        !al_init_font_addon() || !al_init_ttf_addon() || !al_init_image_addon()) {
        fprintf(stderr, "Failed to initialize Allegro or its addons!\n");
        return -1;
    }

    srand(time(NULL));

    ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    if (!queue || !disp) {
        fprintf(stderr, "Failed to create Allegro resources!\n");
        if (queue) al_destroy_event_queue(queue);
        if (disp) al_destroy_display(disp);
        return -1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    
    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!font) {
        fprintf(stderr, "Failed to load font!\n");
        cleanup(disp, queue, font, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        return -1;
    }

    ALLEGRO_FONT* font_rounds = al_load_ttf_font("./assets/font/font.ttf", 64, 0);
    if (!font_rounds) {
        fprintf(stderr, "Failed to load font!\n");
        cleanup(disp, queue, font, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        return -1;
    }

    ALLEGRO_BITMAP *icon = al_load_bitmap("./assets/start/icon.png");
    if (!icon) {
        fprintf(stderr, "Failed to load icon image!\n");
        cleanup(disp, queue, font, font_rounds, NULL, NULL, NULL, NULL, NULL, NULL);
        return -1;
    }

    print_game_controls();

    al_set_display_icon(disp, icon);

    fighter* player_1 = spawn_fighter(61, 92, 61, Y_SCREEN - 92, X_SCREEN, Y_SCREEN, 1);
    fighter* player_2 = spawn_fighter(61, 92, X_SCREEN - 61, Y_SCREEN - 92, X_SCREEN, Y_SCREEN, 0);

    int state = MENU;
    int menu_option = MENU_SINGLE_PLAYER;
    int background_option = 0;
    int continue_play = 1;
    char filename[100];

    ALLEGRO_BITMAP* player1_sheet = NULL;
    ALLEGRO_BITMAP* player2_sheet = NULL;

    ALLEGRO_BITMAP* writings = al_load_bitmap("./assets/fighters/writings.png");
    if (!writings) {
        fprintf(stderr, "Failed to load writings!\n");
        cleanup(disp, queue, font, font_rounds, icon, player1_sheet, player2_sheet, NULL, player_1, player_2);
        return -1;
    }

    start_menu(font, menu_option);

    while (state != EXIT) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        switch (state) {
            case MENU:
                start_menu(font, menu_option);
                state = menu_input(event, &menu_option);
                break;

            case BGND:
                state = choose_background(font, queue, &background_option);
                if (background_option == 0) strcpy(filename, "airport_lane");
                else if (background_option == 1) strcpy(filename, "motorcycle_alley");
                break;

            case FGHT:
                state = fighters_menu(font, queue, &player1_sheet, &player2_sheet);
                break;

            case GAME:
                run_game(disp, queue, font, font_rounds, player_1, player_2, &continue_play, menu_option, 
                         player1_sheet, player2_sheet, writings, filename);
                if (!continue_play) state = EXIT;
                if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    state = MENU;
                break;
    
            default:
                break;
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            state = EXIT;
        }
    }

    cleanup(disp, queue, font, font_rounds, icon, player1_sheet, player2_sheet, writings, player_1, player_2);

    return 0;
}
