#ifndef BUFFER_H
#define BUFFER_H
#include "common.h"
void initBuffer(Buffer *b);
int pushBuffer(Buffer *b,float Value);
int popBuffer(Buffer *b, float *Value);
int isBufferFull(Buffer *b);
int isBufferEmpty(Buffer *b);
#endif
