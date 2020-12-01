#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>

#include "LabCamera.h"
#include "Transform3D.h"
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
	int getRandom(int min, int max);
	const char* generateRandomColor();
	Platform* allocatePlatforms();
	void RenderPlatform(Platform* platform, float deltaTimeSeconds);
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

	const char* str_rPlatform = "redPlatform";
	const char* str_yPlatform = "yellowPlatform";
	const char* str_oPlatform = "orangePlatform";
	const char* str_gPlatform = "greenPlatform";
	const char* str_pPlatform = "purplePlatform";
	const char* str_bPlatform = "bluePlatform";

	float xCameraInitial = 0, yCameraInitial = 2, zCameraInitial = 3.5f;
	float xCamera = 0, yCamera = 0, zCamera = 0;

	float xPlayerInitial, yPlayerInitial, zPlayerInitial;
	float xPlayer, yPlayer, zPlayer;
	

	float angularSpeedPlayer = 0;

	bool goingUp = false, goingDown = false;

	int platformsNumber = 10;
	float platformSpeed = 4.f;
	Platform* centerPlatforms;
	int minPlatformLength = 3;
	int maxPlatformLength = 6;
	float spaceBetweenPlatform = 2.f;
	float platformWidth = 1.f;

	float xCenterPlatforms;
	float xLeftPlatforms;
	float xRightPlatforms;
};