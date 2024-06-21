#ifndef __POWER__ 																											
#define __POWER__																											
																															
#include "hadouken.h"

#define POWER_COOLDOWN 100																									
																															
typedef struct {																											
    unsigned char timer;																									
    hadouken *shots;																										
} power;																														
																															
power* power_create();																										
hadouken* power_shot(unsigned short x, unsigned short y, unsigned char trajectory, power *gun);								
void power_destroy(power *element);																							
																																																													
#endif																														

