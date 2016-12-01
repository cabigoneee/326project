/*********************************************************
FILE : submit.cpp (csci3260 2016-2017 Assignment 2)
*********************************************************/
/*********************************************************
Student Information
Student ID: 1155064634
Student Name: Ho Kwan Hi Marcus Kuncoro
*********************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using glm::vec3;
using glm::vec2;
using glm::mat4;

float x_delta = 1.0f;
int x_press_num = 0;
float y_delta = 1.0f;
int y_press_num = 0;
float z_delta = 1.0f;
int z_press_num = 0;
int xrotate = 0;
int yrotate = 0;
int zrotate = 0;
int autorotate;
int skin = 0;

bool autoRotate = true;
float focus = 0.0f;
bool smile = true;
float diffuseBrightnessUnit = 0.5f;

//Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 20.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = 800 / 2.0;
GLfloat lastY = 600 / 2.0;


GLint programID;
GLuint textureID = NULL;
// Could define the Vao&Vbo and interaction parameter here
GLuint vao_cube, vbo_cube, texture_cube;
GLuint VAO_Jeep, VBO_Jeep, UV_Jeep, Normal_Jeep, drawSize_Jeep, texture_Jeep, texture_Jeep2, carskin1, carskin2;
GLuint VAO_PlanetA, VBO_PlanetA, UV_PlanetA, Normal_PlanetA, drawSize_PlanetA, texture_PlanetA;
GLuint VAO_PlanetB, VBO_PlanetB, UV_PlanetB, Normal_PlanetB, drawSize_PlanetB, texture_PlanetB;
GLuint VAO_PlanetC, VBO_PlanetC, UV_PlanetC, Normal_PlanetC, drawSize_PlanetC, texture_PlanetC;

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
		);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void keyboard(unsigned char key, int x, int y)
{
	//TODO: Use keyboard to do interactive events and animation
	if (key == 'a')
	{
		x_press_num -= 1;
	}
	if (key == 'd')
	{
		x_press_num += 1;
	}
	if (key == 'w')
	{
		y_press_num += 1;
	}
	if (key == 's')
	{
		y_press_num -= 1;
	}
	if (key == 'q')
	{
		z_press_num -= 1;
	}
	if (key == 'e')
	{
		z_press_num += 1;
	}
	if (key == 'y')
	{
		yrotate += 1;
	}
	if (key == 't')
	{
		if (autoRotate) { autoRotate = false; }
		else if (!autoRotate) { autoRotate = true; };
	}
	if (key == 'c')
	{
		skin += 1;
	}
	if (key == 'v')
	{
		if (smile) { smile = false; }
		else if (!smile) { smile = true; };
	}
	if (key == 'u')
	{
		if (diffuseBrightnessUnit <= 0.0f) { }
		else { diffuseBrightnessUnit -= 0.05f; }
	}
	if (key == 'i')
	{
		if (diffuseBrightnessUnit <= 2.5f) { diffuseBrightnessUnit += 0.05f; }
		
	}
	
	if (key == 'r')
	{
		x_press_num = 0; y_press_num = 0; z_press_num = 0;
		xrotate = 0; yrotate = 0; zrotate = 0;
		cameraPos = glm::vec3(0.0f, 10.0f, 20.0f);
		focus = 0;
		autoRotate = true; autorotate = 0;
		diffuseBrightnessUnit = 0.5f;
	}

}

void SpecialInput(int key, int x, int y)
{
	GLfloat cameraSpeed = 1.0f;
	if (key == GLUT_KEY_UP)
		cameraPos += cameraSpeed * cameraFront;
	if (key == GLUT_KEY_DOWN)
		cameraPos -= cameraSpeed * cameraFront;
	if (key == GLUT_KEY_LEFT)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (key == GLUT_KEY_RIGHT)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


bool firstMouse = true;
void PassiveMouse(int xpos, int ypos)
{
	//TODO: Use Mouse to do interactive events and animation
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
	) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}

	return true;
}

GLuint loadBMP_custom(const char * imagepath) {

	printf("Reading image %s\n", imagepath);

	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	unsigned char * data;

	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0; }

	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return 0; }

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (imageSize == 0)    imageSize = width*height * 3;
	if (dataPos == 0)      dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);


	GLuint textureID;
	//TODO: Create one OpenGL texture and set the texture parameter	
	// Create one OpenGL texture
	glGenTextures(1, &textureID);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// OpenGL has now copied the data. Free our own version
	delete[] data;

	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void bindCubeObj()
{
	const GLfloat cube[] =
	{
		//back
		+0.5f, +0.5f, -0.5f, //0
		+0.0f, +1.0f, +0.0f,
		-0.5f, +0.5f, -0.5f, //1
		+0.0f, +1.0f, +0.0f,
		-0.5f, -0.5f, -0.5f, //2
		+0.0f, +1.0f, +0.0f,
		+0.5f, -0.5f, -0.5f, //3
		+0.0f, +1.0f, +0.0f,

		//front
		+0.5f, +0.5f, +0.5f, //4
		+1.0f, +0.0f, +1.0f,
		-0.5f, +0.5f, +0.5f, //5
		+1.0f, +0.0f, +1.0f,
		-0.5f, -0.5f, +0.5f, //6
		+1.0f, +0.0f, +1.0f,
		+0.5f, -0.5f, +0.5f, //7
		+1.0f, +0.0f, +1.0f,

		//right
		-0.5f, +0.5f, +0.5f, //8
		+0.0f, +0.0f, +1.0f,
		-0.5f, -0.5f, +0.5f, //9
		+0.0f, +0.0f, +1.0f,
		-0.5f, -0.5f, -0.5f, //10
		+0.0f, +0.0f, +1.0f,
		-0.5f, +0.5f, -0.5f, //11
		+0.0f, +0.0f, +1.0f,

		//left
		+0.5f, +0.5f, +0.5f, //12
		+1.0f, +1.0f, +0.0f,
		+0.5f, -0.5f, +0.5f, //13
		+1.0f, +1.0f, +0.0f,
		+0.5f, -0.5f, -0.5f, //14
		+1.0f, +1.0f, +0.0f,
		+0.5f, +0.5f, -0.5f, //15
		+1.0f, +1.0f, +0.0f,

		//top
		+0.5f, +0.5f, +0.5f, //16
		+1.0f, +0.0f, +0.0f,
		+0.5f, +0.5f, -0.5f, //17
		+1.0f, +0.0f, +0.0f,
		-0.5f, +0.5f, -0.5f, //18
		+1.0f, +0.0f, +0.0f,
		-0.5f, +0.5f, +0.5f, //19
		+1.0f, +0.0f, +0.0f,

		//bottom
		+0.5f, -0.5f, +0.5f, //20
		+0.0f, +1.0f, +1.0f,
		+0.5f, -0.5f, -0.5f, //21
		+0.0f, +1.0f, +1.0f,
		-0.5f, -0.5f, -0.5f, //22
		+0.0f, +1.0f, +1.0f,
		-0.5f, -0.5f, +0.5f, //23
		+0.0f, +1.0f, +1.0f,

	};
	//GLuint vaoID;
	glGenVertexArrays(1, &vao_cube);
	glBindVertexArray(vao_cube);  //first VAO
	//GLuint vboID;
	glGenBuffers(1, &vbo_cube);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	//glVertexAttribPointer attribute (the assigned number attribute, no of input per line, type(GL_FLOAT), GL_FALSE, no of inputs of each vertex, skip how many inputs for the first value)

	GLushort indices[] = { 0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23, };
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void bindJeepObj()
{
	// Read our .obj file
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	bool res = loadOBJ("obj/jeep.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &VAO_Jeep);
	glBindVertexArray(VAO_Jeep);

	glGenBuffers(1, &VBO_Jeep);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Jeep);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &UV_Jeep);
	glBindBuffer(GL_ARRAY_BUFFER, UV_Jeep);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &Normal_Jeep);
	glBindBuffer(GL_ARRAY_BUFFER, Normal_Jeep);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	drawSize_Jeep = vertices.size();
	// 1rst attribute buffer : vertices

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Jeep);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, UV_Jeep);
	glVertexAttribPointer(
		1, // attribute
		2, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	glEnableVertexAttribArray(1);

	
	glBindBuffer(GL_ARRAY_BUFFER, Normal_Jeep);
	glVertexAttribPointer(
	2, // attribute
	3, // size
	GL_FLOAT, // type
	GL_FALSE, // normalized?
	0, // stride
	(void*)0 // array buffer offset
	);
	glEnableVertexAttribArray(2);
	
	
	glDrawArrays(GL_TRIANGLES, 0, drawSize_Jeep);
	

	glBindVertexArray(0); // Unbind VAO*
};

void bindPlanetAObj()
{
	// Read our .obj file
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	bool res = loadOBJ("obj/planet.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &VAO_PlanetA);
	glBindVertexArray(VAO_PlanetA);

	glGenBuffers(1, &VBO_PlanetA);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_PlanetA);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &UV_PlanetA);
	glBindBuffer(GL_ARRAY_BUFFER, UV_PlanetA);
	 glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &Normal_PlanetA);
	glBindBuffer(GL_ARRAY_BUFFER, Normal_PlanetA);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	drawSize_PlanetA = vertices.size();
	// 1rst attribute buffer : vertices

	glBindBuffer(GL_ARRAY_BUFFER, VBO_PlanetA);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, UV_PlanetA);
	glVertexAttribPointer(
		1, // attribute
		2, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	glEnableVertexAttribArray(1);

	
	glBindBuffer(GL_ARRAY_BUFFER, Normal_PlanetA);
	glVertexAttribPointer(
	2, // attribute
	3, // size
	GL_FLOAT, // type
	GL_FALSE, // normalized?
	0, // stride
	(void*)0 // array buffer offset
	);
	glEnableVertexAttribArray(2);
	

	glDrawArrays(GL_TRIANGLES, 0, drawSize_PlanetA);

	glBindVertexArray(0); // Unbind VAO*
};

void bindPlanetBObj()
{
	// Read our .obj file
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	bool res = loadOBJ("obj/planet.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &VAO_PlanetB);
	glBindVertexArray(VAO_PlanetB);

	glGenBuffers(1, &VBO_PlanetB);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_PlanetB);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &UV_PlanetB);
	glBindBuffer(GL_ARRAY_BUFFER, UV_PlanetB);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &Normal_PlanetB);
	glBindBuffer(GL_ARRAY_BUFFER, Normal_PlanetB);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	drawSize_PlanetB = vertices.size();
	// 1rst attribute buffer : vertices

	glBindBuffer(GL_ARRAY_BUFFER, VBO_PlanetB);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, UV_PlanetB);
	glVertexAttribPointer(
		1, // attribute
		2, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, Normal_PlanetB);
	glVertexAttribPointer(
		2, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	glEnableVertexAttribArray(2);


	glDrawArrays(GL_TRIANGLES, 0, drawSize_PlanetB);

	glBindVertexArray(0); // Unbind VAO*
};

void bindPlanetCObj()
{
	// Read our .obj file
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	bool res = loadOBJ("obj/planet.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &VAO_PlanetC);
	glBindVertexArray(VAO_PlanetC);

	glGenBuffers(1, &VBO_PlanetC);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_PlanetC);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &UV_PlanetC);
	glBindBuffer(GL_ARRAY_BUFFER, UV_PlanetC);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &Normal_PlanetC);
	glBindBuffer(GL_ARRAY_BUFFER, Normal_PlanetC);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	drawSize_PlanetC = vertices.size();
	// 1rst attribute buffer : vertices

	glBindBuffer(GL_ARRAY_BUFFER, VBO_PlanetC);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, UV_PlanetC);
	glVertexAttribPointer(
		1, // attribute
		2, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, Normal_PlanetC);
	glVertexAttribPointer(
		2, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);
	glEnableVertexAttribArray(2);


	glDrawArrays(GL_TRIANGLES, 0, drawSize_PlanetC);

	glBindVertexArray(0); // Unbind VAO*
};


void loadTexture()
{
	texture_cube = loadBMP_custom("awesome_face.bmp");
	texture_Jeep = loadBMP_custom("jeep_texture.bmp");
	texture_Jeep2 = loadBMP_custom("smileyface.bmp");
	texture_PlanetA = loadBMP_custom("tiles.bmp");
	texture_PlanetB = loadBMP_custom("texture/earth.bmp");
	texture_PlanetC = loadBMP_custom("smileyface.bmp");
	carskin1 = loadBMP_custom("carskin1.bmp");
	carskin2 = loadBMP_custom("carskin2.bmp");
}


void sendDataToOpenGL()
{
	//TODO:
	//Load objects and bind to VAO & VBO
	//Load texture

	bindCubeObj();
	//bindJeepObj();
	bindPlanetAObj();
	bindPlanetBObj();
	bindPlanetCObj();

	loadTexture();

}

void light()
{	
	//eye position
	GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	vec3 eyePositionWorld = cameraPos + cameraFront;
	glUniform3fv(eyePositionUniformLocation, 1, &eyePositionWorld[0]);	
	//Light source
	GLint LightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	vec3 lightPosition(0.0f + x_press_num * x_delta, 5.0f + y_press_num * y_delta, 1.0f + z_press_num * z_delta);
	glUniform3fv(LightPositionUniformLocation, 1, &lightPosition[0]);
	//ambient light is the background light in the environment
	//with a light source(diffuse light), ambient light refills areas that light source does not reach
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambient");
	glm::vec3 ambient(0.1f, 0.1f, 0.1f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambient[0]);
	GLint diffuseBrightnessUniformLocation = glGetUniformLocation(programID, "diffuseBrightness");
	glm::vec3 diffuseBrightness(diffuseBrightnessUnit, diffuseBrightnessUnit, diffuseBrightnessUnit);
	glUniform3fv(diffuseBrightnessUniformLocation, 1, &diffuseBrightness[0]);
	//GLint SpecularBrightnessUniformLocation = glGetUniformLocation(programID, "SpecularBrightness");
	//glm::vec3 SpecularBrightness(1.0f, 1.0f, 1.0f);
	//glUniform3fv(SpecularBrightnessUniformLocation, 1, &SpecularBrightness[0]);
}

void rendercube()
{
	//glBindVertexArray(vaoID);
	//glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	mat4 ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 rotationMatrix = glm::scale(mat4(), vec3(1, 1, 1));
	rotationMatrix = glm::translate(rotationMatrix, vec3(0, 5, 1));
	mat4 projectionMatrix = glm::perspective(60.0f + focus,
		(GLfloat)800 / (GLfloat)600, 0.1f, 10.0f);

	mat4 fullTransformMatrix = projectionMatrix * ViewMatrix * rotationMatrix; // care that the order is crucial

	GLint fullTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1,
		GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1,
		GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "rotationMatrix"), 1,
		GL_FALSE, &rotationMatrix[0][0]);

	//Create sampler will be used in Shader
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_cube);
	glUniform1i(TextureID, 0);
	TextureID = glGetUniformLocation(programID, "myTextureSampler2"); //needa use the same texture to myTextureSampler2
	glActiveTexture(GL_TEXTURE1);									  //or else the texture we used earlier in jeep will overwrite the plane here
	glBindTexture(GL_TEXTURE_2D, texture_cube);
	glUniform1i(TextureID, 1);

	//glDrawArrays(GL_TRIANGLES, 0, 6); //third argument: no of vertex
	glBindVertexArray(vao_cube);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

void renderJeep()
{
	mat4 ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 rotationMatrix = glm::rotate(mat4(), (yrotate * 0.3922222f + autorotate * 0.3922222f * 0.005f), vec3(0, 1, 0));;
	mat4 projectionMatrix = glm::perspective(45.0f + focus,
		(GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

	if (autoRotate)
	{
		autorotate++;
		//rotationMatrix = glm::rotate(mat4(), yrotate * 0.3922222f, vec3(0, 1, 0));;
	}

	mat4 fullTransformMatrix = projectionMatrix * ViewMatrix * rotationMatrix; // care that the order is crucial
	mat4 modelTransormMatrix = ViewMatrix * rotationMatrix;

	GLint fullTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1,
		GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1,
		GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "rotationMatrix"), 1,
		GL_FALSE, &rotationMatrix[0][0]);

	GLuint TextureID;
	GLuint *skin_ptr;
	if (skin % 3 == 0){ skin_ptr = &texture_Jeep; }
	else if (skin % 3 == 1){ skin_ptr = &carskin1; }
	else { skin_ptr = &carskin2; }
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *skin_ptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	glUniform1i(TextureID, 0);
	
	
	if (smile) {
		TextureID = glGetUniformLocation(programID, "myTextureSampler2");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture_Jeep2);
		glGenerateMipmap(GL_TEXTURE_2D);
		glUniform1i(TextureID, 1);
	}
	else {
		TextureID = glGetUniformLocation(programID, "myTextureSampler2");
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *skin_ptr);
		glGenerateMipmap(GL_TEXTURE_2D);
		glUniform1i(TextureID, 1);
	}
	

	glBindVertexArray(VAO_Jeep);
	glDrawArrays(GL_TRIANGLES, 0, drawSize_Jeep);
}


void renderPlanetA()
{
	mat4 ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 rotationMatrix = glm::scale(mat4(), vec3(1, 1, 1));
	rotationMatrix = glm::translate(rotationMatrix, vec3(8, 14, 0));
	rotationMatrix = glm::rotate(rotationMatrix, 0 * 0.3922222f, vec3(1, 0, 0));;
	rotationMatrix = glm::rotate(rotationMatrix, 0 * 0.3922222f, vec3(0, 1, 0));;
	rotationMatrix = glm::rotate(rotationMatrix, 0 * -0.3922222f, vec3(0, 0, 1));;
	mat4 projectionMatrix = glm::perspective(45.0f + focus,
		(GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

	mat4 fullTransformMatrix = projectionMatrix * ViewMatrix * rotationMatrix; // care that the order is crucial
	mat4 modelTransormMatrix = ViewMatrix * rotationMatrix;

	GLint fullTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1,
		GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1,
		GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "rotationMatrix"), 1,
		GL_FALSE, &rotationMatrix[0][0]);

	//Create sampler will be used in Shader
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_PlanetA);
	glUniform1i(TextureID, 0);
	TextureID = glGetUniformLocation(programID, "myTextureSampler2"); //needa use the same texture to myTextureSampler2
	glActiveTexture(GL_TEXTURE1);									  //or else the texture we used earlier in jeep will overwrite the plane here
	glBindTexture(GL_TEXTURE_2D, texture_PlanetA);
	glUniform1i(TextureID, 1);

	glBindVertexArray(VAO_PlanetA);
	glDrawArrays(GL_TRIANGLES, 0, drawSize_PlanetA);
}

void renderPlanetB()
{
	mat4 ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 rotationMatrix = glm::translate(mat4(), vec3(-7, 10, 0));
	rotationMatrix = glm::scale(rotationMatrix, vec3(1.2, 1.2, 1.2));
	//rotationMatrix = glm::translate(rotationMatrix, vec3(-10, 20, 0));
	rotationMatrix = glm::rotate(rotationMatrix, 0 * 0.3922222f, vec3(1, 0, 0));;
	rotationMatrix = glm::rotate(rotationMatrix, 0 * 0.3922222f, vec3(0, 1, 0));;
	rotationMatrix = glm::rotate(rotationMatrix, 0 * -0.3922222f, vec3(0, 0, 1));;
	mat4 projectionMatrix = glm::perspective(45.0f + focus,
		(GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

	mat4 fullTransformMatrix = projectionMatrix * ViewMatrix * rotationMatrix; // care that the order is crucial
	mat4 modelTransormMatrix = ViewMatrix * rotationMatrix;

	GLint fullTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1,
		GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1,
		GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "rotationMatrix"), 1,
		GL_FALSE, &rotationMatrix[0][0]);

	//Create sampler will be used in Shader
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_PlanetB);
	glUniform1i(TextureID, 0);
	TextureID = glGetUniformLocation(programID, "myTextureSampler2"); //needa use the same texture to myTextureSampler2
	glActiveTexture(GL_TEXTURE1);									  //or else the texture we used earlier in jeep will overwrite the plane here
	glBindTexture(GL_TEXTURE_2D, texture_PlanetB);
	glUniform1i(TextureID, 1);

	glBindVertexArray(VAO_PlanetB);
	glDrawArrays(GL_TRIANGLES, 0, drawSize_PlanetB);
}

void renderPlanetC()
{
	mat4 ViewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 rotationMatrix = glm::scale(mat4(), vec3(0.3, 0.3, 0.3));
	rotationMatrix = glm::translate(rotationMatrix, vec3(-8, 20, 0));
	rotationMatrix = glm::rotate(rotationMatrix, 0 * 0.3922222f, vec3(1, 0, 0));;
	rotationMatrix = glm::rotate(rotationMatrix, 0 * 0.3922222f, vec3(0, 1, 0));;
	rotationMatrix = glm::rotate(rotationMatrix, 0 * -0.3922222f, vec3(0, 0, 1));;
	mat4 projectionMatrix = glm::perspective(45.0f + focus,
		(GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);

	mat4 fullTransformMatrix = projectionMatrix * ViewMatrix * rotationMatrix; // care that the order is crucial
	mat4 modelTransormMatrix = ViewMatrix * rotationMatrix;

	GLint fullTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1,
		GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1,
		GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "rotationMatrix"), 1,
		GL_FALSE, &rotationMatrix[0][0]);

	//Create sampler will be used in Shader
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_PlanetC);
	glUniform1i(TextureID, 0);
	TextureID = glGetUniformLocation(programID, "myTextureSampler2"); //needa use the same texture to myTextureSampler2
	glActiveTexture(GL_TEXTURE1);									  //or else the texture we used earlier in jeep will overwrite the plane here
	glBindTexture(GL_TEXTURE_2D, texture_PlanetC);
	glUniform1i(TextureID, 1);

	glBindVertexArray(VAO_PlanetC);
	glDrawArrays(GL_TRIANGLES, 0, drawSize_PlanetC);
}



void paintGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//TODO:
	//Set lighting information, such as position and color of lighting source
	//Set transformation matrix
	//Bind different texture
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(0.0, 0.0, 0.0, 0.0) for black background, while all 1 is white

	light();

	glBindVertexArray(vao_cube);
	rendercube();
	//glBindVertexArray(VAO_Jeep);
	//renderJeep();
	glBindVertexArray(VAO_PlanetA);
	renderPlanetA();
	glBindVertexArray(VAO_PlanetB);
	renderPlanetB();
	glBindVertexArray(VAO_PlanetC);
	renderPlanetC();

	glFlush();
	glutPostRedisplay();
}

void initializedGL(void)
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	installShaders();
	sendDataToOpenGL();	
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutInitWindowSize(800, 600);
	glutCreateWindow("Hi again! Cabigon desu!!");

	initializedGL();
	glutDisplayFunc(paintGL);
	glutPassiveMotionFunc(PassiveMouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialInput);

	glutMainLoop();

	return 0;
}