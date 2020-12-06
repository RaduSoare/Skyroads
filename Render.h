#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include "LabCamera.h"
#include "Platform.h"
#include "GameProperties.h"
#include "Transform3D.h"
#include "Transform2D.h"

class Render {

public:
	Render();

	void Render::RenderPlatform(Platform* platform, float deltaTimeSeconds, Platform* platformRow);
	void Render::RenderSimpleMesh(Camera* camera, Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, 
		glm::mat4 projectionMatrix);
	void Render::RenderMesh(Camera* camera, Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, 
		glm::mat4 projectionMatrix)

};