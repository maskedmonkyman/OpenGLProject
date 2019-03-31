#include "Shader.h"

GLuint compileShader(GLuint type, std::string source)
{
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		{
			using namespace std;

			cout << "Failed to compile shader: " << endl;
			cout << message << endl;
		}
		delete message;
	}

	return id;
}

GLuint createShader(const std::string vertexShader, const std::string fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vertShader = compileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fragShader = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetProgramInfoLog(program, length, &length, message);
		{
			using namespace std;

			cout << "Failed to link shader: " << endl;
			cout << message << endl;
		}
		delete message;
	}

	glValidateProgram(program);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

std::string readFromFile(std::string path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cout << "failed to open file: " << path.c_str() << std::endl;
		return "";
	}

	//move the stream from 0 to file end
	file.seekg(0, std::fstream::end);
	//get the size from the stream
	size_t sizeBytes = file.tellg();
	//set the stream back to the start of the file
	file.seekg(0);
	//make a string of that size
	std::string result(sizeBytes, 0);
	//read from the stream to the file
	file.read(&result.front(), sizeBytes);
	return result;
}
