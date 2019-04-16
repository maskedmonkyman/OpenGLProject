#include <iostream>
#include <vector>
#include "Shader.h"
#include "Quad.h"
#include "playground.hpp"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/ext.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "GLM/gtx/transform.hpp"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

GLFWwindow* init();

int main()
{
	//startup
	GLFWwindow* window = init();
	if (!window)
		return -1;

	using namespace glm;
	
	GLuint shader = createShader(
		readFromFile("res/vertex.shader"),	
		readFromFile("res/fragment.shader")
	);

	glUseProgram(shader);

	//the camaras projection matrix
	mat4 projectionMatrix = ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f);
	//perspective(90.0f, 4.0f / 3.0f, 0.1f, 500.0f);
	//view matrix based on the look at direction from (0,1) -> (0,0) with
	mat4 viewMatrix = lookAt(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	//set our const uniforms
	glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, false, value_ptr(projectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewMatrix"), 1, false, value_ptr(viewMatrix));

	//set up the square buffers
	Quad::initBuffers(shader);

	//main loop
	//circleLoop(window);
	//gridLoop(window);
	//testLoop(window);
	collisionTestLoop(window);

	Quad::destroyBuffers();

	//cleanup and exit
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}

GLFWwindow* init()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		return nullptr;
	}

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "hello openGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		return nullptr;
	}

	return window;
}