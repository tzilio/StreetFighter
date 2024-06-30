#include "display.h"

// MENU

void start_menu(ALLEGRO_FONT* font, int selected_option) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    ALLEGRO_BITMAP* logo = al_load_bitmap("./assets/start/logo.png");

    al_draw_bitmap(logo, 0, 0, 0); 

    ALLEGRO_COLOR default_color = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR selected_color = al_map_rgb(255, 255, 0);

    al_draw_text(font, selected_option == MENU_SINGLE_PLAYER ? selected_color : default_color,
                 X_SCREEN / 2, Y_SCREEN - 100, ALLEGRO_ALIGN_CENTER, "Single-Player");

    al_draw_text(font, selected_option == MENU_MULTIPLAYER ? selected_color : default_color,
                 X_SCREEN / 2, Y_SCREEN - 75, ALLEGRO_ALIGN_CENTER, "Multi-Player");

    al_draw_text(font, selected_option == MENU_EXIT ? selected_color : default_color,
                 X_SCREEN / 2, Y_SCREEN - 50, ALLEGRO_ALIGN_CENTER, "Exit Game");

    al_flip_display();
}


int menu_input(ALLEGRO_EVENT event, int *selected_option) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (event.keyboard.keycode) {
            case ALLEGRO_KEY_DOWN:
                *selected_option = (*selected_option == MENU_EXIT) ? MENU_SINGLE_PLAYER : *selected_option + 1;
                break;
            case ALLEGRO_KEY_UP:
                *selected_option = (*selected_option == MENU_SINGLE_PLAYER) ? MENU_EXIT : *selected_option - 1;
                break;
            case ALLEGRO_KEY_ENTER:
                return (*selected_option == MENU_SINGLE_PLAYER || *selected_option == MENU_MULTIPLAYER) ? BGND : EXIT;
            case ALLEGRO_KEY_ESCAPE:
                return EXIT;
        }
    }
    return MENU;
}



// PAUSE 
int draw_pause(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE* queue) {
    int selected_option = 1; 
    bool done = false;

    al_draw_filled_rectangle(X_SCREEN * 0.4, Y_SCREEN * 0.25, X_SCREEN * 0.6, Y_SCREEN * 0.6, al_map_rgb(0, 0, 0));

    ALLEGRO_COLOR color_unselected = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR color_selected = al_map_rgb(255, 255, 0);

    al_draw_text(font, selected_option == 0 ? color_selected : color_unselected,
                    X_SCREEN / 2, Y_SCREEN / 3, ALLEGRO_ALIGN_CENTER, "Exit");
    al_draw_text(font, selected_option == 1 ? color_selected : color_unselected,
                    X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "Resume");

    al_flip_display();

    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        // Processa eventos
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_DOWN:
                    selected_option = 1 - selected_option; // Alterna entre as opções
                    break;
                case ALLEGRO_KEY_ENTER:
                    done = true; // Sai do loop ao pressionar ENTER
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    done = true; // Sai do loop ao pressionar ESCAPE
                    break;
            }

            al_draw_filled_rectangle(X_SCREEN * 0.4, Y_SCREEN * 0.25, X_SCREEN * 0.6, Y_SCREEN * 0.6, al_map_rgb(0, 0, 0));

            al_draw_text(font, selected_option == 0 ? color_selected : color_unselected,
                         X_SCREEN / 2, Y_SCREEN / 3, ALLEGRO_ALIGN_CENTER, "Exit");
            al_draw_text(font, selected_option == 1 ? color_selected : color_unselected,
                         X_SCREEN / 2, Y_SCREEN / 2, ALLEGRO_ALIGN_CENTER, "Resume");

            al_flip_display();
        }
    }

    return selected_option == 0 ? EXIT : GAME; // Retorna a opção selecionada
}

// CHARACTER SELECTION

void draw_fighter_selection(ALLEGRO_BITMAP *sprite_sheet, int sprite_index, int x, int y,
                             int sprite_dimension, ALLEGRO_COLOR color) {
    int border_thickness = 3;
    al_draw_rectangle(x, y, x + sprite_dimension, y + sprite_dimension, color, border_thickness);
    al_draw_bitmap_region(sprite_sheet, sprite_index * sprite_dimension, 288, 64, 350, x, y, 0);
}

void choose_fighter(ALLEGRO_FONT *font, int selected_option1, int selected_option2, ALLEGRO_BITMAP *heads,
                                                                                   bool done1, bool done2) {

    al_clear_to_color(al_map_rgb(0, 0, 0));
    ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
    ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

    int sprite_dimensions = 64;
    int start_x = (X_SCREEN - (2 * 32 + 10)) / 2;
    int start_y = (Y_SCREEN - (2 * 32 + 10)) / 2 - 50;

    al_draw_bitmap_region(heads, 0, 0, 272, 48, 275, 0, 0);    

    al_draw_bitmap_region(heads, 112 * selected_option1, 176, 112, 112, X_SCREEN / 8, Y_SCREEN - 250, 0);
    
    al_draw_bitmap_region(heads, 112 * selected_option2, 176, 112, 112, 600, Y_SCREEN - 250, 0);

    draw_fighter_selection(heads, 0, start_x, start_y, sprite_dimensions,
                            selected_option1 == 0 ? blue : (selected_option2 == 0 ? red : white));

    draw_fighter_selection(heads, 1, start_x + sprite_dimensions, start_y, sprite_dimensions,
                            selected_option1 == 1 ? blue : (selected_option2 == 1 ? red : white));

    draw_fighter_selection(heads, 2, start_x, start_y + sprite_dimensions + 3, sprite_dimensions,
                            selected_option1 == 2 ? blue : (selected_option2 == 2 ? red : white));

    draw_fighter_selection(heads, 3, start_x + sprite_dimensions, start_y + sprite_dimensions + 3,
         sprite_dimensions, selected_option1 == 3 ? blue : (selected_option2 == 3 ? red : white));

    if (done1) al_draw_text(font, blue, X_SCREEN / 8 + 50, Y_SCREEN - 120, ALLEGRO_ALIGN_CENTER, "Ready Player 1");

    if (done2) al_draw_text(font, red, 655, Y_SCREEN - 120, ALLEGRO_ALIGN_CENTER, "Ready Player 2");

    al_flip_display();
}

void start_fight(ALLEGRO_BITMAP *heads, int selected_fighter1, int selected_fighter2) {
    al_clear_to_color(al_map_rgb(0, 0, 0));     

    al_draw_bitmap_region(heads, 512, 352, 112, 112, 350, 100, 0);
    al_draw_bitmap_region(heads, 112 * selected_fighter1, 64, 112, 112, X_SCREEN / 8, Y_SCREEN - 250, 0);
    al_draw_bitmap_region(heads, 112 * selected_fighter2, 64, 112, 112, 600, Y_SCREEN - 250, 1);
    al_flip_display();    

    al_rest(2.0);
}

int fighters_menu(ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_BITMAP** player1_sheet, ALLEGRO_BITMAP** player2_sheet) {
    int selected_option1 = RYU;
    int selected_option2 = KEN;
    bool done1 = 0;
    bool done2 = 0;

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);
    ALLEGRO_BITMAP *heads = al_load_bitmap("./assets/fighters/heads.png");

    al_draw_bitmap_region(heads, 0, 0, 272, 48, 275, 50, 0);    

    while (!done1 || !done2) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        choose_fighter(font, selected_option1, selected_option2, heads, done1, done2);
        al_clear_to_color(al_map_rgb(0, 0, 0));

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            al_destroy_bitmap(heads);         
           return EXIT;
        }
    
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    if (selected_option2 == ZANGIEF && selected_option1 != KEN)
                        selected_option2 = KEN;
                    else if (selected_option2 == CHUN && selected_option1 != RYU)
                        selected_option2 = RYU;
                    break;
                case ALLEGRO_KEY_DOWN:
                    if (selected_option2 == KEN && selected_option1 != ZANGIEF)
                        selected_option2 = ZANGIEF;
                    else if (selected_option2 == RYU && selected_option1 != CHUN)
                        selected_option2 = CHUN;
                    break;
                case ALLEGRO_KEY_LEFT:
                    if (selected_option2 == KEN && selected_option1 != RYU)
                        selected_option2 = RYU;
                    else if (selected_option2 == ZANGIEF && selected_option1 != CHUN)
                        selected_option2 = CHUN;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    if (selected_option2 == RYU && selected_option1 != KEN)
                        selected_option2 = KEN;
                    else if (selected_option2 == CHUN && selected_option1 != ZANGIEF)
                        selected_option2 = ZANGIEF;
                    break;
                case ALLEGRO_KEY_W:
                    if (selected_option1 == ZANGIEF && selected_option2 != KEN)
                        selected_option1 = KEN;
                    else if (selected_option1 == CHUN && selected_option2 != RYU)
                        selected_option1 = RYU;
                    break;
                case ALLEGRO_KEY_S:
                    if (selected_option1 == KEN && selected_option2 != ZANGIEF)
                        selected_option1 = ZANGIEF;
                    else if (selected_option1 == RYU && selected_option2 != CHUN)
                        selected_option1 = CHUN;
                    break;
                case ALLEGRO_KEY_A:
                    if (selected_option1 == KEN && selected_option2 != RYU)
                        selected_option1 = RYU;
                    else if (selected_option1 == ZANGIEF && selected_option2 != CHUN)
                        selected_option1 = CHUN;
                    break;
                case ALLEGRO_KEY_D:
                    if (selected_option1 == RYU && selected_option2 != KEN)
                        selected_option1 = KEN;
                    else if (selected_option1 == CHUN && selected_option2 != ZANGIEF)
                        selected_option1 = ZANGIEF;
                    break;
                case ALLEGRO_KEY_ENTER:
                    done2 = true;
                    break;
                case ALLEGRO_KEY_SPACE:
                    done1 = true;
                    break;
            }
        }
    }
    choose_fighter(font, selected_option1, selected_option2, heads, done1, done2);
    al_rest(1.0);
    start_fight(heads, selected_option1, selected_option2);
    al_destroy_bitmap(heads); // Liberar a memória alocada para o bitmap.

    if (selected_option1 == RYU) *player1_sheet = al_load_bitmap("./assets/fighters/ryu.png");
    else if (selected_option1 == KEN) *player1_sheet = al_load_bitmap("./assets/fighters/ken.png");
    else if (selected_option1 == CHUN) *player1_sheet = al_load_bitmap("./assets/fighters/chun_li.png");
    else if (selected_option1 == ZANGIEF) *player1_sheet = al_load_bitmap("./assets/fighters/zangief.png");

    if (selected_option2 == RYU) *player2_sheet = al_load_bitmap("./assets/fighters/ryu.png");
    else if (selected_option2 == KEN) *player2_sheet = al_load_bitmap("./assets/fighters/ken.png");
    else if (selected_option2 == CHUN) *player2_sheet = al_load_bitmap("./assets/fighters/chun_li.png");
    else if (selected_option2 == ZANGIEF) *player2_sheet = al_load_bitmap("./assets/fighters/zangief.png");

    return GAME;
}

// BACKGROUND SELECTION

void init_animated_background(background *bg, float frame_rate, const char *dir) {
    bg->current_frame = 0;
    bg->frame_duration = 1.0 / frame_rate;
    bg->last_frame_update = 0;
    for (int i = 0; i < NUM_FRAMES; i++) {
        char filename[200];
        snprintf(filename, sizeof(filename), "./assets/background/%s/frame_%d.png", dir, i);
        bg->frames[i] = al_load_bitmap(filename);
        if (!bg->frames[i]) {
            fprintf(stderr, "Failed to load frame %d from %s\n", i, filename);
            exit(-1);
        }
    }
}

void update_animated_background(background *bg) {
    double current_time = al_get_time();
    if (current_time - bg->last_frame_update >= bg->frame_duration) {
        bg->current_frame = (bg->current_frame + 1) % NUM_FRAMES; 
        bg->last_frame_update = current_time;
    }
}

void draw_animated_background(background *bg) {
    al_draw_bitmap(bg->frames[bg->current_frame], 0, 0, 0);
}

void destroy_animated_background(background *bg) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (bg->frames[i]) {
            al_destroy_bitmap(bg->frames[i]);
        }
    }
}

void show_background_options(background *bg, ALLEGRO_FONT* font, int selected_option) {
    update_animated_background(bg);    
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_animated_background(bg);

    ALLEGRO_COLOR default_color = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR selected_color = al_map_rgb(255, 255, 0);

    al_draw_text(font, default_color, X_SCREEN / 2, 50, ALLEGRO_ALIGN_CENTER, "Choose fighting spot:");

    int thumb_width = 128;
    int thumb_height = 72;
    int spacing = 50;

    int farm_x = (X_SCREEN / 2) - thumb_width - spacing / 2;
    int farm_y = (Y_SCREEN / 3) - (thumb_height / 2);
    
    int temple_x = (X_SCREEN / 2) + spacing / 2;
    int temple_y = (Y_SCREEN / 3) - (thumb_height / 2);

    al_draw_text(font, selected_option == FARM ? selected_color : default_color,
                 farm_x + thumb_width / 2, farm_y + thumb_height + 10, ALLEGRO_ALIGN_CENTER, "AIRPORT LANE");
    al_draw_text(font, selected_option == TEMPLE ? selected_color : default_color,
                 temple_x + thumb_width / 2, temple_y + thumb_height + 10, ALLEGRO_ALIGN_CENTER, "MOTORCYCLE ALLEY");

    al_flip_display();
}


int choose_background(ALLEGRO_FONT* font, ALLEGRO_EVENT_QUEUE *queue, int *selected_option) {
    int done = 0;
        
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    background bg[2];
    init_animated_background(&bg[0], 24, "airport_lane");
    init_animated_background(&bg[1], 24, "motorcycle_alley");

    while (!done) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            update_animated_background(&bg[*selected_option]);
            show_background_options(&bg[*selected_option], font, *selected_option);
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_RIGHT:
                    *selected_option = (*selected_option == TEMPLE) ? FARM : TEMPLE;
                    break;
                case ALLEGRO_KEY_LEFT:
                    *selected_option = (*selected_option == FARM) ? TEMPLE : FARM;
                    break;
                case ALLEGRO_KEY_ENTER:
                    al_destroy_timer(timer);
                    done = 1;
                    destroy_animated_background(&bg[0]);
                    destroy_animated_background(&bg[1]);
                    return FGHT;
                case ALLEGRO_KEY_ESCAPE:
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    al_destroy_timer(timer);
                    destroy_animated_background(&bg[0]);
                    destroy_animated_background(&bg[1]);
                    return EXIT;
            }
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_timer(timer);
            destroy_animated_background(&bg[0]);
            destroy_animated_background(&bg[1]);
            return EXIT;
        }
    }

    destroy_animated_background(&bg[0]);
    destroy_animated_background(&bg[1]);
    
    return BGND;
}


// BETWEEN ROUNDS

void display_winner(ALLEGRO_DISPLAY* disp, ALLEGRO_FONT* font, int winner) {
    const char* winner_text;
    if (winner == NEW_ROUND1) winner_text = "Player 1 Wins Round!";
    if (winner == NEW_ROUND2) winner_text = "Player 2 Wins Round!";
    if (winner == NEW_ROUND_DRAW) winner_text = "It's a Draw!";
    if (winner == P1_WINNER) winner_text = "Player 1 Wins!";
    if (winner == P2_WINNER) winner_text = "Player 2 Wins!";

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(font, al_map_rgb(255, 255, 255), al_get_display_width(disp) / 2, al_get_display_height(disp) / 2, ALLEGRO_ALIGN_CENTER, winner_text);
    al_flip_display();
}

void show_rounds(ALLEGRO_BITMAP *writings, int *current_round) {
    int round_word = (*current_round == 1) ? ROUND_1 : (*current_round == 2) ? ROUND_2 : FINAL_ROUND;       
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_writings(writings, round_word, FIGHT);
    al_flip_display();
}


void draw_writings(ALLEGRO_BITMAP *writings, int selected_word1, int selected_word2) {
    al_draw_bitmap_region(writings, 0, 160 * selected_word1, 480, 160, X_SCREEN / 6, Y_SCREEN - 250, 0);
    al_draw_bitmap_region(writings, 0, 160 * selected_word2, 480, 160, X_SCREEN / 1.5, Y_SCREEN - 250, 0);
}


// HUD (IN GAME DISPLAY)

void draw_scoreboard(ALLEGRO_FONT *font, int health1, int health2, int points1, int points2, int round, int countdown) {
    int x = X_SCREEN;
    
    int size1 = ((x/2) - ((health1*100)/x));
    int size2 = ((x/2) + ((health2*100)/x));

    char score1[100], score2[100];
    sprintf(score1, "%d", health1);
    sprintf(score2, "%d", health2);

    char mid_text[100];
    sprintf(mid_text, "%d", countdown);
    int mid_text_x = x / 2 - al_get_text_width(font, mid_text) / 2;
    int mid_text_y = 25; 

    char current_round[100];
    sprintf(current_round, "Round %d", round);

    char points[100];
    sprintf(points, "%d x %d", points1, points2);

    al_draw_text(font, al_map_rgb(255, 0, 0), size1 - 50, 10, 0, score1);
    al_draw_text(font, al_map_rgb(0, 0, 255), size2 + 50, 10, 0, score2);

    al_draw_text(font, al_map_rgb(255, 255, 0), x/2-100000/x, mid_text_y, 0, current_round);
    al_draw_text(font, al_map_rgb(255, 255, 0), x/2+75000/x, mid_text_y, 0, points);

    al_draw_filled_rectangle(size1, 5, x / 2, 20, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(size2, 5, x / 2, 20, al_map_rgb(0, 0, 255));

    al_draw_text(font, al_map_rgb(255, 255, 0), mid_text_x, mid_text_y, 0, mid_text);
}

void draw_stamina(ALLEGRO_FONT *font, float stamina1, float stamina2) {
    int screen_width = X_SCREEN;    

    char p1_stamina[100];
    char p2_stamina[100];

    // Format the stamina strings
    sprintf(p1_stamina, "Player 1 Stamina: %.0f", stamina1);
    sprintf(p2_stamina, "Player 2 Stamina: %.0f", stamina2);

    // Define positions
    int bar_height = 5;
    int bar_width = 130;
    int margin = 10;

    int p1_x = margin;
    int p1_y = margin + 10;

    int p2_x = screen_width - bar_width - margin;
    int p2_y = margin + 10;

    // Draw stamina bars
    al_draw_text(font, al_map_rgb(255, 0, 0), p1_x, p1_y - 15, 0, p1_stamina);
    al_draw_text(font, al_map_rgb(0, 0, 255), p2_x - 40, p2_y - 15, 0, p2_stamina);

    al_draw_filled_rectangle(p1_x, p1_y, p1_x + (stamina1 / 100.0) * bar_width, p1_y + bar_height, al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(p2_x, p2_y, p2_x + (stamina2 / 100.0) * bar_width, p2_y + bar_height, al_map_rgb(0, 0, 255));
}



