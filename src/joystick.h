#ifndef __JOYSTICK__
#define __JOYSTICK__

typedef struct {
    unsigned char right;
    unsigned char left;
    unsigned char up;
    unsigned char down;
    unsigned char up_left;
    unsigned char up_right;
    unsigned char hadouken;
} joystick;

joystick* joystick_create();
void joystick_destroy(joystick *element);
void joystick_right(joystick *element);
void joystick_left(joystick *element);
void joystick_up(joystick *element);
void joystick_down(joystick *element);
void joystick_up_right(joystick *element);
void joystick_up_left(joystick *element); 
void joystick_hadouken(joystick *element);

#endif

