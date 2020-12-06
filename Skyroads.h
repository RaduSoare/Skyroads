#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>

#include "LabCamera.h"
#include "Transform3D.h"
#include "Transform2D.h"
#include "Platform.h"
#include "Drawer.h"




class Skyroads : public SimpleScene 
{
public:
	Skyroads();
	~Skyroads();

	void Init() override;

	

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;
	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix);
	void RenderPlatform(Platform* platform, float deltaTimeSeconds, Platform* platformRow);
	void RenderFuelbar(float deltaTimeSeconds);

	Platform* allocatePlatform(int platformSide);
	Platform** allocatePlatforms();
	
	bool checkPlayerOnPlatform(Platform* platform);

	int getRandom(int min, int max);
	const char* generateRandomColor();

	
	

protected:
	// Game properties
	Camera *camera;
	Drawer drawer;
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;
	bool renderCameraTarget = true;
	bool endGame = false;
	bool goingUp = false;
	bool goingDown = false;
	
	// Animations speed properties
	float endGameGravity = 0;
	float cameraSpeed = 2.0f;
	float jumpSpeed = 1.5f;
	float airTime = 0;
	float angularSpeedPlayer = 0;

	// Platforms properties
	Platform** platforms;
	int minPlatformLength = 3;
	int maxPlatformLength = 6;
	float spaceBetweenPlatform = 2.f;
	float platformWidth = 1.f;
	float platformSpeed = 4.f;
	int numberOfPlatformsRows = 5;
	const int platformsNumberPerRow = 10;

	// Coordinates
	float xCameraInitial = 0, yCameraInitial = 2, zCameraInitial = 3.5f;
	float xCamera = 0, yCamera = 0, zCamera = 0;

	float xPlayerInitial, yPlayerInitial, zPlayerInitial;
	float xPlayer, yPlayer, zPlayer;

	// PowerUps - Fuel
	float fuel = 100;
	float fuelConsumption = 5.f;
	float fuelLostOnYellow = 10.f;
	float fuelGainedOnGreen = 20.f;

	float orangeEffectActive = false;
	float orangeEffectDuration = 30.f;

	const char* str_backgroundFuelbar = "backgroundFuelbar";
	const char* str_foregroundFuelbar = "foregroundFuelbar";
	
	
	// Colors
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



	
	

	



	


	
};