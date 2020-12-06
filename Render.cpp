#include "Render.h"

using namespace std;

Render::Render()
{

}

void Render::RenderPlatform(Platform* platform, float deltaTimeSeconds, Platform* platformRow) {

	glm::mat4 modelMatrix = Transform3D::Translate(platform->xCoord, 0.75f, platform->zCoord);
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

void Render::RenderSimpleMesh(Camera* camera, Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::mat4 projectionMatrix)
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

void Render::RenderMesh(Camera* camera, Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, glm::mat4 projectionMatrix)
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