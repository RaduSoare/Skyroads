#include"Skyroads.h"
#include "Platform.h"
#include "LabCamera.h"

#include <vector>
#include <string>
#include <iostream>
#include <math.h>

#include <Core/Engine.h>

using namespace std;

Skyroads::Skyroads(){}

Skyroads::~Skyroads(){}


void Skyroads::Init()
{
	// Retine coordonatele initiale ale camerei
	xCamera = xCameraInitial;
	yCamera = yCameraInitial;
	zCamera = zCameraInitial;

	// Seteaza camera
	camera = new Camera();
	camera->Set(glm::vec3(xCamera, yCamera, zCamera), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	// Retine coordonatele initiale ale jucatorului
	xPlayerInitial = camera->GetTargetPosition()[0];
	yPlayerInitial = camera->GetTargetPosition()[1];
	zPlayerInitial = camera->GetTargetPosition()[2];

	// Deseneaza tipurile de platforme
	drawer.DrawPlatform('o', &meshes);
	drawer.DrawPlatform('g', &meshes);
	drawer.DrawPlatform('r', &meshes);
	drawer.DrawPlatform('y', &meshes);
	drawer.DrawPlatform('p', &meshes);
	drawer.DrawPlatform('b', &meshes);

	//Deseneaza componentele fuelbar-ului
	drawer.DrawFuelBar(whiteColor, str_backgroundFuelbar, &meshes);
	drawer.DrawFuelBar(redColor, str_foregroundFuelbar, &meshes);

	

	platforms = allocatePlatforms();
	

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	
	{
		// initializeaza shaderele
		Shader *shader = new Shader("Skyroad");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

}

void Skyroads::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Skyroads::Update(float deltaTimeSeconds)
{
	// Randeaza bara de combustibil
	RenderFuelbar(deltaTimeSeconds);

	{	
		int platformTouchedPerFrame = 0;

		for (int row = 0; row < numberOfPlatformsRows; row++) {
			for (int i = 0; i < platformsNumberPerRow; i++) {
				RenderPlatform(&platforms[row][i], deltaTimeSeconds, platforms[row]);
				if (checkPlayerOnPlatform(&platforms[row][i]) == true) {
					// Numara cate platforme au fost atinse intr-un frame
					platformTouchedPerFrame++;
				}

			}
		}

		// Consuma efectul platformei portocalii
		if (orangeEffectActive == true) {
			orangeEffectDuration -= deltaTimeSeconds * 10;
			// Readuce player-ul la viteza initiala dupa consumarea power-up-ului
			if (orangeEffectDuration <= 0) {
				orangeEffectActive = false;
				orangeEffectDuration = 50.f;
				platformSpeed /= 2;
			}
		}
		
		// Verifica daca jucatorul nu a atins nicio platforma intr-un frame
		if(platformTouchedPerFrame < 1 && endGame == false) {
			cout << "You fell off the platform" << endl;
			renderCameraTarget = true;
			endGame = true;
		}

		
		
	}

	// Consumarea combustibilului in functie de timp
	fuel -= deltaTimeSeconds * fuelConsumption;
	if (fuel <= 0.f && endGame == false) {
		cout << "Ran out of fuel." << endl;
		endGame = true;
	}

	// Updateaza pozitia jucatorului in functie de pozitia camerei
	xPlayer = camera->GetTargetPosition()[0];
	yPlayer = camera->GetTargetPosition()[1];
	zPlayer = camera->GetTargetPosition()[2];


	// Cazul Third Person
	if (renderCameraTarget)
	{	
		// Verifica daca jocul inca e in desfasurare
		if (endGame == false) {
			modelMatrix = glm::mat4(1);
			
			// Deplasarea jucatorului
			modelMatrix *= Transform3D::Translate(xPlayer, yPlayer, zPlayer);
			
			// Animatie rotire
			angularSpeedPlayer += deltaTimeSeconds * 2;
			modelMatrix *= Transform3D::RotateOX(-angularSpeedPlayer);

			modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
		}
		// Animatie terminare joc
		else {
			modelMatrix = Transform3D::Translate(xPlayer, yPlayer, zPlayer);
			// Animatie cadere in gravitatie
			endGameGravity += deltaTimeSeconds * 5;
			modelMatrix *= Transform3D::Translate(0, -endGameGravity, 0);
			modelMatrix *= Transform3D::Scale(0.5f, 0.5f, 0.5f);
			// Cadere in gravitatie
			if (endGameGravity >= 3.f) {
				exit(1);
			}
		}
		/*
			Verifica daca efectul platformei portocalii este activ pentru a aplica functia de zgomot
			Cand efectul a trecut, bila se randeaza normal
		*/
		if (orangeEffectActive == true) {
			RenderSimpleMesh(meshes["sphere"], shaders["Skyroad"], modelMatrix);
		}
		else {
			RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
		}
		
	}

	
	
}
 
bool Skyroads::checkPlayerOnPlatform(Platform* platform) {
	
	// Caz third person
	if ((renderCameraTarget == true && camera->GetTargetPosition()[0] >= platform->xCoord && camera->GetTargetPosition()[0] <= platform->xCoord + platformWidth
		&& camera->GetTargetPosition()[2] >= platform->zCoord + platform->distance && camera->GetTargetPosition()[2] <= platform->zCoord + platform->distance + platform->length
		&& yPlayer <= yPlayerInitial) 
		|| 
		// Caz first person
		(renderCameraTarget == false && xCamera >= platform->xCoord && xCamera <= platform->xCoord + platformWidth
			&& zCamera >= platform->zCoord + platform->distance && zCamera <= platform->zCoord + platform->distance + platform->length
			&& yPlayer <= yPlayerInitial)) {
		// Crestere combustibil la coliziunea cu platforma verde
		if (platform->color == str_gPlatform) {
			fuel += fuelGainedOnGreen;
			if (fuel >= 100.f) {
				fuel = 100;
			}
		}
		// Scadere combustibil la coliziunea cu platforma galbena
		else if (platform->color == str_yPlatform) {
			fuel -= fuelLostOnYellow;
		}
		// Terminare joc la coliziunea cu platforma rosie
		else if (platform->color == str_rPlatform && endGame == false) {
			cout << "You touched a red platform"<<endl;
			endGame = true;
			renderCameraTarget = true;
		}
		// Activare efect la coliziunea cu platforma portocalie
		else if (orangeEffectActive == false && platform->color == str_oPlatform) {
			orangeEffectActive = true;
			platformSpeed *= 2;
			
		}
		// Modifica culoarea platformei la coliziune
		platform->color = str_pPlatform;
		return true;
		
	}
	// Se considera ca jucatorul este in afara platformei doar cand se afla la nivelul acesteia
	if (goingUp == false && goingDown == false) {
		return false;
	}

	return true;
}

void Skyroads::FrameEnd(){}


void Skyroads::OnInputUpdate(float deltaTime, int mods)
{
	
	if (window->KeyHold(GLFW_KEY_W) && orangeEffectActive == false) {
		camera->MoveForward(deltaTime * cameraSpeed);
		zCamera -= deltaTime * cameraSpeed;
	}

	if (window->KeyHold(GLFW_KEY_S) && orangeEffectActive == false) {
		camera->MoveForward(-deltaTime * cameraSpeed);
		zCamera += deltaTime * cameraSpeed;
	}
	
	if (window->KeyHold(GLFW_KEY_A)) {
		camera->TranslateRight(-deltaTime * cameraSpeed);
		xCamera -= deltaTime * cameraSpeed;
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		camera->TranslateRight(deltaTime * cameraSpeed);
		xCamera += deltaTime * cameraSpeed;
	}

	// Animatie saritura
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
		
		if (( yPlayer > yPlayerInitial)) {
			camera->TranslateUpword(- deltaTime * jumpSpeed);
		}
		else {
			goingUp = false;
			goingDown = false;
			airTime = 0;
		}
	}
	

}

void Skyroads::OnKeyPress(int key, int mods)
{
	// Initiaza saritura
	if (key == GLFW_KEY_SPACE && goingDown == false && goingUp == false) {
		goingUp = true;
	}
	// Modificare tip camera
	if (key == GLFW_KEY_C)
	{
		renderCameraTarget = !renderCameraTarget;
	}
}

void Skyroads::OnKeyRelease(int key, int mods){}

void Skyroads::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY){}

void Skyroads::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods){}

void Skyroads::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods){}

void Skyroads::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY){}

void Skyroads::OnWindowResize(int width, int height){}

Platform* Skyroads::allocatePlatform(int platformSide) {

	Platform* platforms = new Platform[platformsNumberPerRow];

	// Aloca prima platforma sub player
	platforms[0] = Platform(xCameraInitial - platformWidth / 2 + platformSide, zPlayerInitial - maxPlatformLength * 2, maxPlatformLength * 2, str_pPlatform);

	// Retine pozitia ultimei platforme alocate pentru a evita suprapunerea acestora
	float lastZ = zPlayerInitial - maxPlatformLength * 2;

	// Aloca restul platformelor la coordonate random
	for (int i = 1; i < platformsNumberPerRow; i++) {
		int currentLength = getRandom(minPlatformLength, maxPlatformLength);
		platforms[i] = Platform(xCameraInitial - platformWidth / 2 + platformSide, lastZ - spaceBetweenPlatform - currentLength, currentLength, generateRandomColor());
		lastZ = lastZ - spaceBetweenPlatform - platforms[i].length;

	}

	return platforms;

}

// Aloca matricea de platforme : fiecare side reprezinta codificarea unui rand
Platform** Skyroads::allocatePlatforms() {
	int side = 0;
	Platform** platforms = new Platform*[numberOfPlatformsRows];
	for (int row = 0; row < numberOfPlatformsRows; row++) {
		switch (row) {
		case 0:
			side = -2;
			break;
		case 1:
			side = -1;
			break;
		case 2:
			side = 0;
			break;
		case 3:
			side = 1;
			break;
		case 4:
			side = 2;
			break;
		}
		platforms[row] = allocatePlatform(side);
	}
	return platforms;
}

void Skyroads::RenderFuelbar(float deltaTimeSeconds) {
	glm::mat3 modelMatrixBgnd = Transform2D::Translate(xCameraInitial - 2.2f, yCamera);
	glm::mat3 modelMatrixFgnd = Transform2D::Translate(xCameraInitial - 2.2f, yCamera);
	// Scalarea barii de combustibil odata cu scaderea acestuia
	modelMatrixFgnd *= Transform2D::Scale(fuel / 100, 1);
	RenderMesh2D(meshes[str_foregroundFuelbar], shaders["VertexColor"], modelMatrixFgnd);
	RenderMesh2D(meshes[str_backgroundFuelbar], shaders["VertexColor"], modelMatrixBgnd);

}

void Skyroads::RenderPlatform(Platform* platform, float deltaTimeSeconds, Platform* platformRow) {

	modelMatrix = Transform3D::Translate(platform->xCoord, 0.75f, platform->zCoord);
	if (endGame == false) {
		platform->distance += deltaTimeSeconds * platformSpeed;
	}

	modelMatrix *= Transform3D::Translate(0, 0, platform->distance);
	modelMatrix *= Transform3D::Scale(1, 1, platform->length);


	// Verificare daca platforma a ajuns in spatele camerei

	if (platform->isInGame == true && platform->zCoord + platform->distance >= zCamera + platform->length) {

		platform->isInGame = false;
		float lastPlatform = 0;
		for (int i = 0; i < platformsNumberPerRow; i++) {
			if (platformRow[i].isInGame == true && platformRow[i].zCoord + platformRow[i].distance < lastPlatform) {
				// calculeaza coordonatele celei mai indepartate platforme
				// pentru a genera urmatoarea platforma, dupa aceasta
				lastPlatform = platformRow[i].zCoord + platformRow[i].distance;
			}
		}
		// Updatare proprietati platforma noua
		platform->length = getRandom(minPlatformLength, maxPlatformLength);
		//platform->xCoord = xCameraInitial + getRandom(-2, 2);
		platform->zCoord = lastPlatform - platform->length - spaceBetweenPlatform;
		platform->color = generateRandomColor();
		platform->isInGame = true;
		platform->distance = 0;


	}

	RenderMesh(meshes[platform->color], shaders["VertexColor"], modelMatrix);
}

void Skyroads::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
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

void Skyroads::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	int locationModel = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(locationModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int locationView = glGetUniformLocation(shader->program, "View");
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(locationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));


	int locationProjection = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(locationProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	
	// Trimite timpul pentru a realiza functia de zgomot
	float time = fmod(Engine::GetElapsedTime(), 1.f);

	int locationTime = glGetUniformLocation(shader->program, "Time");
	glUniform1f(locationTime, time);

	// Deseneaza obiectul
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

/*
	Genereaza random culori in functie de probabilitatea de aparitie
		blue -> 65%
		yellow -> 10%
		green -> 10%
		orange -> 10%
		red -> 5%
*/
const char* Skyroads::generateRandomColor() {
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
	else {
		return str_bPlatform;
	}

}

int Skyroads::getRandom(int min, int max) {
	return rand() % (max - min + 1) + min;
}