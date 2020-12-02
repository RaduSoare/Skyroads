#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>

#include "LabCamera.h"
#include "Transform3D.h"
#include "Transform2D.h"
#include "Platform.h"


class Tema2 : public SimpleScene 
{
public:
	Tema2();
	~Tema2();

	void Init() override;

	

private:
	Mesh* CreateMesh(const char *name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;


	void drawPlatform(char platformType);
	void Tema2::drawFuelBar(glm::vec3 color, const char* fuelbarComponent);

	int getRandom(int min, int max);
	const char* generateRandomColor();
	Platform* allocatePlatform(int platformSide);
	Platform** allocatePlatforms();
	void RenderPlatform(Platform* platform, float deltaTimeSeconds, Platform* platformRow);
	void RenderFuelbar(float deltaTimeSeconds);
	bool checkPlayerOnPlatform(Platform* platform);

	
	

protected:
	Camera *camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget = true;
	glm::mat4 modelMatrix;
	bool endGame = false;
	float endGameGravity = 0;

	float cameraSpeed = 2.0f;
	float jumpSpeed = 1.5f;

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

	int numberOfPlatformsRows = 3;

	float fuel = 100;
	float fuelConsumption = 5.f;
	float fuelLostOnYellow = 10.f;
	float fuelGainedOnGreen = 20.f;
};