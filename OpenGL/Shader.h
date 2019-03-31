#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "GL/glew.h"

std::string readFromFile(std::string);
GLuint compileShader(GLuint type, std::string source);
GLuint createShader(const std::string vertexShader, const std::string fragmentShader);
