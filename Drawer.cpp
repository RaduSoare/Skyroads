#include "Drawer.h"
#include "Colors.h"

using namespace std;

Drawer::Drawer()
{

}

void Drawer::DrawFuelBar(glm::vec3 color, const char* fuelbarComponent,
	std::unordered_map<std::string, Mesh*>* meshes) {

	vector<VertexFormat> verticesFuelbar = {
		VertexFormat(glm::vec3(0, 0, 0), color),
		VertexFormat(glm::vec3(1.f, 0, 0),  color),
		VertexFormat(glm::vec3(1.f, 0.2f, 0),  color),
		VertexFormat(glm::vec3(0, 0.2f, 0),  color)
	};
	vector<unsigned short> indicesFuelbar = {
		0, 1, 3,
		2, 3, 1
	};
	Mesh *fuelbar = CreateMesh(fuelbarComponent, verticesFuelbar, indicesFuelbar, meshes);
	fuelbar->SetDrawMode(GL_TRIANGLES);
}

void Drawer::DrawPlatform(char platformType, std::unordered_map<std::string, Mesh*>* meshes) {

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
	};

	vector<unsigned short> indices =
	{
		0, 1, 2,
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

	};

	Mesh* platform = CreateMesh(meshName, vertices, indices, meshes);

}

Mesh* Drawer::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices,
	std::unordered_map<std::string, Mesh*>* meshes)
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
	(*meshes)[name] = new Mesh(name);
	(*meshes)[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	(*meshes)[name]->vertices = vertices;
	(*meshes)[name]->indices = indices;
	return (*meshes)[name];
}