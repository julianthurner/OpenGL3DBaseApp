// Disclaimer: I got most of this code from https://learnopengl.com which is a fantastic resource for starting out

#include <fstream>
#include <sstream>
#include <iostream>

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders.hpp"

//** Public **//
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	//* Read the shaders from file
	std::string vertexCode, fragmentCode;

	try {
		//* Try reading the file contents
		std::ifstream vertexShaderFile, fragmentShaderFile;

		// Ensure that ifstream objects can throw exceptions
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// Open the files
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		std::stringstream vertexShaderStream, fragmentShaderStream;

		// Store the file contents
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		// Close the files
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// Convert filestreams into strings
		// Note: std::stringstream.str() returns std::string
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
	}

	//* Compile the shaders and create a shader program from them
	// vertexID and fragmentID contain the IDs assigned to them by OpenGL
	// int success simply stores a boolean-like int value for determining the success of the compilation
	unsigned int vertexID, fragmentID;
	int success;
	char infoLog[512];

	// Compile vertex shader
	// Fetch the ID that OpenGL assigns our vertex shader
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	// As unfortunate as it is, there is no way to circumvent storing a const char* temporarily
	const char* vertexCodeChar = vertexCode.c_str();
	glShaderSource(vertexID, 1, &vertexCodeChar, NULL);
	glCompileShader(vertexID);

	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Compile fragment shader
	// Fetch the ID that OpenGL assigns our fragment shader
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	// As unfortunate as it is, there is no way to circumvent storing a const char*
	const char* fragmentCodeChar = fragmentCode.c_str();
	glShaderSource(fragmentID, 1, &fragmentCodeChar, NULL);
	glCompileShader(fragmentID);

	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
	}

	// Create Shader Program and save its ID in shaderProgramID
	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexID);
	glAttachShader(shaderProgramID, fragmentID);
	glLinkProgram(shaderProgramID);

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
	}

	// Delete the shaders as they're no longer needed
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

void Shader::use() {
	glUseProgram(shaderProgramID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}
void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
	glUniform2fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
	glUniform4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setMat2(const std::string& name, const glm::mat2& value) const {
	glUniformMatrix2fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
void Shader::setMat3(const std::string& name, const glm::mat3& value) const {
	glUniformMatrix3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}