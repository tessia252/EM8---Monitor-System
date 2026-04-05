#include "processing.h" 
#include "buffer.h"
int isValidData(float value){
	if(value <= -9999.0f){
		return 0;
	}
	return 1;
}
int checkThreshold(Sensor s, float value){
	if(value > s.threshold){
		return 1;
	}
	return 0;
}
float filterData(Buffer *b){
	if(b->count == 0){
		return 0.0f;
	}
	float sum  = 0.0f;
	int count  = b->count;
	
	for(int i = 0; i < count; i++){
		int index = (b->head + i) % BUFFER_SIZE;
		sum += b->data[index]; 
	}
	return sum / (float)count;
}
