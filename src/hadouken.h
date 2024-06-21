#ifndef __HADOUKEN__ 																																
#define __HADOUKEN__																															

#define HADOUKEN_MOVE 5

typedef struct {																																
	unsigned short x;																															
	unsigned short y;																															
	unsigned char trajectory;												
	struct hadouken *next; 																														
} hadouken;																																		

hadouken* hadouken_create(unsigned short x, unsigned short y, unsigned char trajectory, hadouken *next);												
void hadouken_move(hadouken *elements);																													
void hadouken_destroy(hadouken *element);																					

#endif			
