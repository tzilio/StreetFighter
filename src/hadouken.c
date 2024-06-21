#include <stdlib.h>
#include "hadouken.h"

hadouken* hadouken_create(unsigned short x, unsigned short y, unsigned char trajectory, hadouken *next) {    
    hadouken *new_hadouken = (hadouken*) malloc(sizeof(hadouken));
    if (!new_hadouken) return NULL;
    new_hadouken->x = x;
    new_hadouken->y = y;
    new_hadouken->trajectory = trajectory;
    new_hadouken->next = (struct hadouken*) next;
    return new_hadouken;
}

void hadouken_move(hadouken *elements) {
    for (hadouken *index = elements; index != NULL; index = (hadouken*) index->next) {
        if (!index->trajectory) index->x = index->x - HADOUKEN_MOVE;
        else index->x = index->x + HADOUKEN_MOVE;
    }
}

void hadouken_destroy(hadouken *element) {
    free(element);
}

