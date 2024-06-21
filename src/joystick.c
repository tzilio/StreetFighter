#include <stdlib.h>
#include "joystick.h"

joystick* joystick_create(){
    joystick *element = (joystick*) malloc (sizeof(joystick));
    element->right = 0;
    element->left = 0;
    element->up = 0;
    element->down = 0;
    element->up_left = 0;
    element->up_right = 0;
    element->hadouken = 0;
    return element;
}

void joystick_destroy(joystick *element){ 
    free(element);
}

void joystick_left(joystick *element){ 
    element->left = element->left ^ 1;
}

void joystick_right(joystick *element){ 
    element->right = element->right ^ 1;
}

void joystick_up(joystick *element){ 
    element->up = element->up ^ 1;
}

void joystick_down(joystick *element){ 
    element->down = element->down ^ 1;
}

void joystick_up_right(joystick *element) { 
    element->up_right = element->up_right ^ 1;
}
	
void joystick_up_left(joystick *element) { 
    element->up_left = element->up_left ^ 1;
}

void joystick_hadouken(joystick *element){ 
    element->hadouken = element->hadouken ^ 1;
}	
