#pragma once
#include <Component/SimpleScene.h>
#include <Core/Engine.h>

//#include "Colors.h"

class Drawer
{
public:
	Drawer();
	void DrawPlatform(char platformType, std::unordered_map<std::string, Mesh*>* meshes);
	void DrawFuelBar(glm::vec3 color, const char* fuelbarComponent, std::unordered_map<std::string, Mesh*>* meshes);
	Mesh* CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices,
		std::unordered_map<std::string, Mesh*>* meshes);

protected:
	float platformWidth = 1.f;
};

