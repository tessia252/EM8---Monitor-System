#include "buffer.h"
void initBuffer(Buffer *b){
	b->head = 0;
	b->tail = 0;
	b->count = 0;
}
int isBufferEmpty(Buffer *b){
	return (b->count == 0);
}
int isBufferFull(Buffer *b){
	return (b->count == BUFFER_SIZE);
}
int pushBuffer(Buffer *b, float Value){
	if (isBufferFull(b)){
		b->head = ((b->head)+1) % BUFFER_SIZE;// ghi de du lieu cu neu buffer full
	}
	else{
		b->count++;
	}
	b->data[b->tail] = Value;
	b->tail = (b->tail+1) % BUFFER_SIZE;
	return 1;	
}
int popBuffer(Buffer *b,float *Value){
	if (isBufferEmpty(b)){
		return 0; // khong co du lieu de lay
	}
	*Value = b->data[b->head];
	b->head = ((b->head)+1) % BUFFER_SIZE;
	b->count--;
	return 1;
}
