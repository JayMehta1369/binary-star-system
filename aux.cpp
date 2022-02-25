#include <cmath>
#include "aux.h"

const float G = 39.62545507;

float dist_P (float a, float A, float b, float B) {

	float ans = sqrt((pow((a-A),2))+(pow((b-B),2)));
	return ans;	
}

float pos_Mx (float time) {

	float ans = cos((sqrt(G))*(time)/2);
	return ans;
}

float pos_My (float time) {

	float ans = sin((sqrt(G))*(time)/2);
	return ans;
}

float acceleration (float x, float X1, float X2, float r1, float r2) {

	float ans = (-1*G)*(((x-X1)/(pow(r1,3)))+((x-X2)/(pow(r2,3))));
	return ans;
}
