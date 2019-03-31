#include <iostream>
#include "Shader.h"
#include "Square.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLM/glm.hpp"
#include "GLM/ext.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "GLM/gtx/transform.hpp"

#define ERROR_BREAK(x) if (x) __debugbreak();
#define GL_ERROR_CHECK(x) glClearError();x;ERROR_BREAK(glLogError());

class RedSquare
{
public:
	glm::mat4 modelMatrix;
	glm::vec2 pos;
	glm::vec2 scale;
	float rotation;
	GLuint buffer;

	RedSquare() : pos(glm::vec2(0.0f, 0.0f)), scale(glm::vec2(1.0f, 1.0f)), rotation(0.0f)
	{
		const float halfSize = 0.5f;
		const float square[8] =
		{
			 halfSize,  halfSize,
			-halfSize,  halfSize,
			-halfSize, -halfSize,
			 halfSize, -halfSize
		};

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), &square, GL_STATIC_DRAW);
	}

	RedSquare(GLuint buffer) : pos(glm::vec2(0.0f, 0.0f)), scale(glm::vec2(1.0f, 1.0f)), rotation(0.0f)
	{
		this->buffer = buffer;
	}

	void draw(GLuint shader)
	{
		using namespace glm;
		modelMatrix = mat4(1.0f);
		modelMatrix = translate(modelMatrix, vec3(pos.x * 1.3, pos.y, 0));
		modelMatrix = glm::scale(modelMatrix, vec3(scale.x, scale.y,0));
		modelMatrix = rotate(modelMatrix, radians(rotation), vec3(0, 0, 1));

		glUniformMatrix4fv(glGetUniformLocation(shader, "modelMatrix"), 1, false, value_ptr(modelMatrix));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		glDrawArrays(GL_QUADS, 0, 4);
	}

	~RedSquare()
	{}
};

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

GLFWwindow* init();
std::string readFromFile(std::string);
GLuint compileShader(GLuint type, std::string source);
GLuint createShader(const std::string vertexShader, const std::string fragmentShader);

void glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool glLogError()
{
	bool hadError = false;
	while (GLenum error = glGetError())
	{
		hadError = true;
		std::cout << "openGL Error: " << error << std::endl;
	}
	return hadError;
}

int main()
{
	//startup
	GLFWwindow* window = init();
	if (!window)
		return -1;

	using namespace glm;

	GLuint shader = createShader
	(
		readFromFile("res/vertex.shader"),	
		readFromFile("res/fragment.shader")
	);

	glUseProgram(shader);

	//orthogonal projection matrix
	const float screenRatio = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
	const float orthoWIdth = 1.0f;

	//the camaras projection matrix
	mat4 projectionMatrix = ortho(0.0f, (float)WINDOW_HEIGHT, (float)WINDOW_WIDTH, 0.0f); 
	//perspective(90.0f, 4.0f / 3.0f, 0.1f, 500.0f);
	//view matrix based on the look at direction from (0,1) -> (0,0) with
	mat4 viewMatrix = lookAt(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, false, value_ptr(projectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewMatrix"), 1, false, value_ptr(viewMatrix));

	Square::initBuffers(shader);
	Square mySquare = Square(vec2(1), 0, vec2(64));

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mySquare.draw();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

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

	if (glewInit() != GLEW_OK)
	{
		return nullptr;
	}

	return window;
}