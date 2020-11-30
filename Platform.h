#pragma once
#include <Core/Engine.h>

#include <string>

using namespace std;

class Platform
{

public:
	Platform();
	Platform(float x, float z, int len, string col);


	float xCoord;
	float zCoord;
	int length;
	float distance;
	string color;
	bool isInGame;


};

