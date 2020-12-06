#pragma once
#include "Platform.h"

using namespace std;

bool endGame = false;
float endGameGravity = 0;

float cameraSpeed = 2.0f;
float jumpSpeed = 1.5f;
float airTime = 0;

glm::vec3 redColor = glm::vec3(1, 0, 0);
glm::vec3 orangeColor = glm::vec3(1, 0.65f, 0);
glm::vec3 yellowColor = glm::vec3(1, 1, 0);
glm::vec3 greenColor = glm::vec3(0, 1, 0);
glm::vec3 purpleColor = glm::vec3(0.5f, 0, 0.5f);
glm::vec3 blueColor = glm::vec3(0, 0, 1.f);
glm::vec3 whiteColor = glm::vec3(1.f, 1.f, 1.f);

const char* str_rPlatform = "redPlatform";
const char* str_yPlatform = "yellowPlatform";
const char* str_oPlatform = "orangePlatform";
const char* str_gPlatform = "greenPlatform";
const char* str_pPlatform = "purplePlatform";
const char* str_bPlatform = "bluePlatform";

const char* str_backgroundFuelbar = "backgroundFuelbar";
const char* str_foregroundFuelbar = "foregroundFuelbar";

float xCameraInitial = 0, yCameraInitial = 2, zCameraInitial = 3.5f;
float xCamera = 0, yCamera = 0, zCamera = 0;

float xPlayerInitial, yPlayerInitial, zPlayerInitial;
float xPlayer, yPlayer, zPlayer;


float angularSpeedPlayer = 0;

bool goingUp = false, goingDown = false;

const int platformsNumberPerRow = 10;
float platformSpeed = 4.f;

Platform** platforms;
int minPlatformLength = 3;
int maxPlatformLength = 6;
float spaceBetweenPlatform = 2.f;
float platformWidth = 1.f;

int xCenterPlatforms = 0;
int xLeftPlatforms = -1;
int xRightPlatforms = 1;

int numberOfPlatformsRows = 5;

float fuel = 100;
float fuelConsumption = 5.f;
float fuelLostOnYellow = 10.f;
float fuelGainedOnGreen = 20.f;

float orangeEffectActive = false;
float orangeEffectDuration = 30.f;
