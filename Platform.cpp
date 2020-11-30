#include"Platform.h"

Platform::Platform(float x, float z, int len, string col) {
	xCoord = x;
	zCoord = z;
	length = len;
	color = col;
	distance = 0;
	isInGame = true;
	
}

Platform::Platform() {

}