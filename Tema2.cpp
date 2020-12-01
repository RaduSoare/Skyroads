#include"Tema2.h"
#include"Platform.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}


void Tema2::Init()
{

	xCamera = xCameraInitial;
	yCamera = yCameraInitial;
	zCamera = zCameraInitial;

	camera = new Camera();
	camera->Set(glm::vec3(xCamera, yCamera, zCamera), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	xPlayerInitial = camera->GetTargetPosition()[0];
	yPlayerInitial = camera->GetTargetPosition()[1];
	zPlayerInitial = camera->GetTargetPosition()[2];

	drawPlatform('o');
	drawPlatform('g');
	drawPlatform('r');
	drawPlatform('y');
	drawPlatform('p');
	drawPlatform('b');

	centerPlatforms = allocatePlatforms();
	

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

}

const char* Tema2::generateRandomColor() {
	int colorProbability = getRandom(0, 100);

	if (colorProbability < 65) {
		return str_bPlatform;
	}
	else if (colorProbability >= 65 && colorProbability < 75) {
		return str_yPlatform;
	}
	else if (colorProbability >= 75 && colorProbability < 85) {
		return str_oPlatform;
	}
	else if (colorProbability >= 85 && colorProbability < 95) {
		return str_gPlatform;
	}
	else if (colorProbability >= 95 && colorProbability <= 100) {
		return str_rPlatform;
	}
	
	
}

Platform* Tema2::allocatePlatforms() {
	Platform* centerPlatforms = new Platform[platformsNumber];
	
	centerPlatforms[0] = Platform(xCameraInitial - platformWidth / 2, zPlayerInitial - maxPlatformLength * 2, maxPlatformLength * 2, generateRandomColor());
	
	float lastX = xCameraInitial, lastZ = zPlayerInitial - maxPlatformLength * 2;

	for (int i = 1; i < platformsNumber	; i++) {
		int currentLength = getRandom(minPlatformLength, maxPlatformLength);
		centerPlatforms[i] = Platform(xCameraInitial + getRandom(-2, 2), lastZ - spaceBetweenPlatform - currentLength, currentLength, generateRandomColor());
		lastZ = lastZ - spaceBetweenPlatform - centerPlatforms[i].length;

	}

	return centerPlatforms;

}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	{
		int platformTouchedPerFrame = 0;
		for (int i = 0; i < platformsNumber; i++) {
			RenderPlatform(&centerPlatforms[i], deltaTimeSeconds);
			if (checkPlayerOnPlatform(&centerPlatforms[i]) == true) {
				platformTouchedPerFrame++;
			}
			
		} 
		if(platformTouchedPerFrame < 1) {
			//cout << "picat" << endl;
			endGame = true;
		}

		
		
	}

	
	if (renderCameraTarget)
	{
		xPlayer = camera->GetTargetPosition()[0];
		yPlayer = camera->GetTargetPosition()[1];
		zPlayer = camera->GetTargetPosition()[2];

		if (endGame == false) {
			modelMatrix = glm::mat4(1);
			modelMatrix *= Transform3D::Translate(xPlayer, yPlayer, zPlayer);
			
			// animatie rotire
			angularSpeedPlayer += deltaTimeSeconds * 2;
			modelMatrix *= Transform3D::RotateOX(-angularSpeedPlayer);

			modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
			
		}
		// Animatie terminare joc
		else {
			modelMatrix = Transform3D::Translate(xPlayer, yPlayer, zPlayer);
			endGameGravity += deltaTimeSeconds * 5;
			modelMatrix *= Transform3D::Translate(0, -endGameGravity, 0);
			modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
			cout << endGameGravity << endl;
			if (endGameGravity >= 3.f) {
				exit(1);
			}
		}
		
		
		

		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
	}

	
	
}

void Tema2::RenderPlatform(Platform* platform, float deltaTimeSeconds) {
	

	
	
	if (endGame == false) {
		modelMatrix = Transform3D::Translate(platform->xCoord, 0.75f, platform->zCoord);
		platform->distance += deltaTimeSeconds * platformSpeed;
		modelMatrix *= Transform3D::Translate(0, 0, platform->distance);
		modelMatrix *= Transform3D::Scale(1, 1, platform->length);
	}
	else {
		modelMatrix = Transform3D::Translate(platform->xCoord, 0.75f, platform->zCoord);
		modelMatrix *= Transform3D::Translate(0, 0, platform->distance);
		modelMatrix *= Transform3D::Scale(1, 1, platform->length);
	}
	
	

	/*
	 Verificare daca platforma a ajuns in spatele camerei
	*/
	if (platform->isInGame == true && platform->zCoord + platform->distance >=zCamera + platform->length) {
		// third person case
		
		platform->isInGame = false;
		int lastPlatform = 0;
		for (int i = 0; i < platformsNumber; i++) {
			if (centerPlatforms[i].isInGame == true &&  centerPlatforms[i].zCoord  + centerPlatforms[i].distance < lastPlatform) {
				lastPlatform = centerPlatforms[i].zCoord + centerPlatforms[i].distance;
			}
		}
		
		platform->length = getRandom(minPlatformLength, maxPlatformLength);
		platform->xCoord = xCameraInitial + getRandom(-2, 2);
		platform->zCoord = lastPlatform - platform->length - spaceBetweenPlatform;
		platform->color = generateRandomColor();
		platform->isInGame = true;
		platform->distance = 0;
		
		
	} 


	RenderMesh(meshes[platform->color], shaders["VertexColor"], modelMatrix);
}

bool Tema2::checkPlayerOnPlatform(Platform* platform) {
	
	
	// caz first person
	/*renderCameraTarget == false && (xCamera >= platform->xCoord && xCamera <= platform->xCoord + platformWidth
		&& zCamera >= platform->zCoord + platform->distance && zCamera <= platform->zCoord + platform->distance + platform->length
		&& yCamera <= yCameraInitial)
		// caz third person
		||*/
	if ((renderCameraTarget == true 
		&& camera->GetTargetPosition()[0] >= platform->xCoord && camera->GetTargetPosition()[0] <= platform->xCoord + platformWidth
		&& camera->GetTargetPosition()[2] >= platform->zCoord + platform->distance && camera->GetTargetPosition()[2] <= platform->zCoord + platform->distance + platform->length
		&& yPlayer <= yPlayerInitial) ) {
		platform->color = str_pPlatform;
		return true;
		
	}
	if (goingUp == false && goingDown == false) {
		return false;
	}
	return true;
	
	
	
}



void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}


float airTime = 0;
void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	

		// TODO : translate the camera forward
	if (window->KeyHold(GLFW_KEY_W)) {
		
		camera->MoveForward(deltaTime * cameraSpeed);
		zCamera -= deltaTime * cameraSpeed;
	}
	
	

	if (window->KeyHold(GLFW_KEY_A)) {
		// TODO : translate the camera to the left
		camera->TranslateRight(-deltaTime * cameraSpeed);
		xCamera -= deltaTime * cameraSpeed;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		// TODO : translate the camera backwards
		camera->MoveForward(-deltaTime * cameraSpeed);
		zCamera += deltaTime * cameraSpeed;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		// TODO : translate the camera to the right
		camera->TranslateRight(deltaTime * cameraSpeed);
		xCamera += deltaTime * cameraSpeed;
	}

	if (window->KeyHold(GLFW_KEY_Q)) {
		// TODO : translate the camera down
		camera->TranslateUpword(-deltaTime * cameraSpeed);
		yCamera -= deltaTime * cameraSpeed;
	}

	if (window->KeyHold(GLFW_KEY_E)) {
		// TODO : translate the camera up
		camera->TranslateUpword(deltaTime * cameraSpeed);
		yCamera += deltaTime * cameraSpeed;
	}

	if (goingUp) {
		airTime += deltaTime * jumpSpeed;
		if (airTime <= 1.f) {
			camera->TranslateUpword(deltaTime * jumpSpeed);
		}
		else {
			goingUp = false;
			goingDown = true;
		}
	}
	else if (goingDown) {
		
		if ((renderCameraTarget == true &&  yPlayer > yPlayerInitial) 
			|| (renderCameraTarget == false && yCamera > yCameraInitial)) {
			camera->TranslateUpword(- deltaTime * jumpSpeed);
		}
		else {
			goingUp = false;
			goingDown = false;
			airTime = 0;
		}
	}
	

}

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE && goingDown == false && goingUp == false) {
		goingUp = true;
	}
	if (key == GLFW_KEY_T)
	{
		renderCameraTarget = !renderCameraTarget;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
			camera->RotateFirstPerson_OY(-sensivityOY * deltaX);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation spee
			camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
			camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
		}

	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}

void Tema2::drawPlatform(char platformType) {

	glm::vec3 color;
	const char* meshName = "";

	switch (platformType)
	{
	case 'r':
		color = redColor;
		meshName = str_rPlatform;
		break;
	case 'y':
		color = yellowColor;
		meshName = str_yPlatform;
		break;
	case 'o':
		color = orangeColor;
		meshName = str_oPlatform;
		break;
	case 'g':
		color = greenColor;
		meshName = str_gPlatform;
		break;
	case 'p':
		color = purpleColor;
		meshName = str_pPlatform;
		break;
	case 'b':
		color = blueColor;
		meshName = str_bPlatform;
		break;
	default:
		break;
	}
	
	
	vector<VertexFormat> vertices
	{
		VertexFormat(glm::vec3(0, 0,  0), color),
		VertexFormat(glm::vec3(platformWidth, 0,  0), color),
		VertexFormat(glm::vec3(0, 0,  1), color),
		VertexFormat(glm::vec3(platformWidth, 0,  1), color),
		VertexFormat(glm::vec3(0, 0.5f,  0), color),
		VertexFormat(glm::vec3(platformWidth, 0.5f,  0), color),
		VertexFormat(glm::vec3(0, 0.5f,  1), color),
		VertexFormat(glm::vec3(platformWidth, 0.5f,  1), color)
		// TODO: Complete the information for the cube
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,	// indices for first triangle
		1, 3, 2,
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4
		// indices for second triangle
		// TODO: Complete indices data
	};

	Mesh* platform = CreateMesh(meshName, vertices, indices);

}

Mesh* Tema2::CreateMesh(const char *name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// TODO: Create the VBO and bind it
	unsigned int VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	// TODO: Crete the IBO and bind it
	unsigned int IBO = 0;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data
	// It will be learned later, when GLSL shaders will be introduced
	// For the moment just think that each property value from our vertex format needs to be send to a certain channel
	// in order to know how to receive it in the GLSL vertex shader

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// TODO: Unbind the VAO
	glBindVertexArray(0);
	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	return meshes[name];
}

int Tema2::getRandom(int min, int max) {
	return rand() % (max - min + 1) + min;
}