#include <stdlib.h>
#include "power.h"

power* power_create(){
    power *new_power = (power*) malloc(sizeof(power));
    if (!new_power) return NULL;
    new_power->timer = 0;
    new_power->shots = NULL;
    return new_power;
}

hadouken* power_shot(unsigned short x, unsigned short y, unsigned char trajectory, power *hand){
    hadouken *new_hadouken = hadouken_create(x, y - 40, trajectory, hand->shots);
    if (!new_hadouken) return NULL;
    return new_hadouken;
}

void power_destroy(power *element){
    hadouken *sentinel;
    for (hadouken *index = element->shots; index != NULL; index = sentinel){
        sentinel = (hadouken*) index->next;
        hadouken_destroy(index);
    }
    free(element);
}

