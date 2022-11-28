#include <fstream>
#include <sstream>
#include <iostream>

// Always include GLAD before GLFW or anything else that requires OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders.hpp"

//** Public **//
Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexCode, fragmentCode;

	//* Read the shaders from file
	try {
		std::ifstream vertexShaderFile, fragmentShaderFile;

		//* Try reading the file contents
		// Set the ifstreams to throw exceptions for failbit (logical error) and badbit (read error)
		// failbit is set for example if the file does not exist or one tries to read beyond the end of a file
		// badbit is set if an error while reading occurs that causes the loss of the stream's integrity
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// Open the files
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);

		// Store the file contents
		std::stringstream vertexShaderStream, fragmentShaderStream;
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
		std::cout << "Error: Shader could not successfully read file\n" << std::endl;
	}

	// int success stores a boolean-like int value for determining the success of the compilation
	// See down below for while we can't use a boolean for this
	unsigned int vertexID, fragmentID;
	int success;

	//* Compile the vertex shader
	// Create an empty vertex shader and temporarily store its assigned ID
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	// As unfortunate as it is, there is no way to circumvent storing a const char* temporarily since you cannot give the address of a temporary object
	const char* vertexCodeChar = vertexCode.c_str();
	// Copy the file-read code into the shader
	// First argument is the previously assigned shader ID
	// Second argument is the number of elements that are given; we are handing one const char* array, so in this case 1
	// Third argument is a pointer to the data
	// 4th argument is the length of the string. Passing nullptr means that the string is null-terminated
	// which automatically is the case if you read a file via stream the way it is done above
	glShaderSource(vertexID, 1, &vertexCodeChar, nullptr);
	// Compiles the shader from source
	glCompileShader(vertexID);

	//* Verify the success of the compilation
	// Returns a parameter from a shader
	// First argument is the shader's assigned ID, second argument the requested parameter, third argument a pointer to store the parameter's value in
	// In this case we want to know if the compilation was successful. Returns 1 for success and 0 for failure
	// Other parameters than GL_COMPILE_STATUS will return different values than 0 or 1 which is why OpenGL expects you to pass an int instead of a bool
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];

		// Fetches the information log for a shader
		// First argument is the shader's assigned ID, second argument the length of the array that will store the info log
		// This is C-style bad memory handling, actually. Be sure not to mess this up cause you can get all kinds of errors
		// if the char array that you pass as 4th argument has a different length than what you passed as 2nd argument
		// Third argument is a pointer to store the info log's actual length in. Since we don't need this info, we simply pass nullptr
		// 4th argument is the char array to store the info log in.
		glGetShaderInfoLog(vertexID, 512, nullptr, infoLog);
		std::cout << "Error: Shader vertex compilation failed\n" << infoLog << "\n" << std::endl;
	}

	//* Do the same for the fragment shader
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentCodeChar = fragmentCode.c_str();
	glShaderSource(fragmentID, 1, &fragmentCodeChar, nullptr);
	glCompileShader(fragmentID);

	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];

		glGetShaderInfoLog(fragmentID, 512, nullptr, infoLog);
		std::cout << "Error: Shader fragment compilation failed\n" << infoLog << "\n" << std::endl;
	}

	//* Create Shader Program and save its ID in shaderProgramID
	// Create an empty shader program and store its assigned ID
	shaderProgramID = glCreateProgram();
	// Attach both the compiled vertex and the compiled fragment shader to the program
	glAttachShader(shaderProgramID, vertexID);
	glAttachShader(shaderProgramID, fragmentID);
	// Link the shaders together. If an issue occurs here, it's most likely connected to vertex / fragment shader compilation,
	// but there can also be cases where both shaders compile successfully, but linking still fails, thus the extra error catching
	glLinkProgram(shaderProgramID);

	//* Verify the success of the linking
	// This function works just the same as glGetShaderiv() that we used above
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];

		// This function works just the same as glGetShaderInfoLog() that we used above
		glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog);
		std::cout << "Error: Shader program linking failed\n" << infoLog << "\n" << std::endl;
	}

	// Delete the (uncompiled) shaders as they're no longer needed since the compiled shaders are already stored in the linked shader program
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

void Shader::use() const {
	// Tell OpenGL to use the shader program associated with the given ID (= the shader itself that calls this function)
	glUseProgram(shaderProgramID);
}

void Shader::setBool(const std::string& name, bool value) const {
	// glUniformxxx() sets a uniform in the shader
	// First argument is the location of the uniform, second argument the value of the uniform
	// Note that there is no glUniform1b in OpenGL. It treats bools like ints
	glUniform1i(
		// glGetUniformLocation() returns a uniform's location
		// First argument is the shader program's id, second argument a const char* containing the name of the uniform
		glGetUniformLocation(shaderProgramID, name.c_str()),
		(int)value);
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

unsigned int Shader::getShaderProgramID() {
	return shaderProgramID;
}