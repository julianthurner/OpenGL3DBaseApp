#include <memory>

#include "resourceManager.hpp"
#include "render.hpp"

//** Private **//
unsigned int UBO_ID;

// Due to C++ immediately defining object declarations (which is very inflexible), we use smart pointers to store camera and floor plane
std::unique_ptr<Camera> cam;
std::unique_ptr<Plane> plane;

std::vector<Shader> shaders;
std::vector<Cube> cubes;
std::vector<std::vector<glm::vec3>> objectPositions;

void prepareShaders() {
	//* Prepare all needed shaders 
	shaders = {
		// [0] => Plane shader, [1] = Cube shader
		Shader("res/shaders/planeShader.vert", "res/shaders/planeShader.frag"),
		Shader("res/shaders/cubeShader.vert", "res/shaders/cubeShader.frag"),
	};

	//* Initialize the uniforms
	// Don't forget to activate a shader before setting uniforms
	shaders[1].use();
	// Note that we don't give the stored textureID that OpenGL assigned because those have nothing to do with this
	// This basically tells OpenGL "Use the first assigned texture (0) as uniform texture0, second one (1) as texture1, etc"
	// Note that OpenGL starts counting at 0, which is why we do the same in our shaders to reduce room for error
	shaders[1].setInt("texture0", 0);
	shaders[1].setInt("texture1", 1);

	//* Set initial blend value
	Render::updateBlendValue(shaders[1], 0.5f);

	//* Initialize a uniform buffer object (UBO) that manages uniforms across all shaders
	// Saving the need to set the same uniforms for each shader individually
	// We'll use this to share view matrix and projection matrix between shaders
	// First argument is the number of buffers to create, second argument a pointer to store the assigned ID in
	glGenBuffers(1, &UBO_ID);
	// Tells OpenGL we are currently working with this UBO
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_ID);
	// Here the actual data copying happens
	// First argument is the buffer type, second argument is the sice of the UBO (we are storing two 4x4 matrices = 2 * 64 bytes)
	// Third argument is a pointer to the data, but since we don't have any data yet, we give nullptr
	// 4th argument tells OpenGL how to handle the data. "Static draw" specifies that the data is set only once and used many times
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	// Link the UBO to binding point 0
	// The binding point is a kind of register that tells each shader where to look for the UBO that needs to be used
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO_ID);

	//* Configure the shaders to link to our UBO
	unsigned int shaderUniformBlockIndex;
	for (unsigned int i = 0; i < shaders.size(); i++) {
		// Get the shader's uniform block index that stores the "matrices" uniform block
		shaderUniformBlockIndex = glGetUniformBlockIndex(shaders[i].getShaderProgramID(), "matrices");
		// Link the shader's "matrices" uniform block index to binding point 0 (where we linked the UBO earlier)
		glUniformBlockBinding(shaders[i].getShaderProgramID(), shaderUniformBlockIndex, 0);

		// Now, when a shader wants to access a uniform contained in the "matrices" uniform block, it will fetch the data from
		// the UBO that is linked to binding point 0. Now every time we want to update the view matrix or projection matrix,
		// we'll update the UBO instead of the shaders. Since our UBO is linked to binding point 0, the shaders can then fetch the data from it
	}
}

void prepareObjects() {
	plane.reset(new Plane());
	
	cubes = {
		Cube("res/images/dummyImage1.png", "res/images/dummyImage2.png"),
		Cube("res/images/dummyImage3.png", "res/images/dummyImage4.png"),
		Cube("res/images/dummyImage5.png", "res/images/dummyImage6.png"),
	};

	objectPositions = {
		{ // [0] => Cube positions for cubes[0]
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f),
		},
		{ // [1] => Cube positions for cubes[1]
			glm::vec3(3.0f, 2.0f, 0.0f),
		},
		{ // [2] => Cube positions for cubes[2]
			glm::vec3(-2.0f, 0.5f, 0.5f),
		},
	};
}

//** Public **//
void ResourceManager::initialize(GLFWwindow& window) {
	// Initialize key settings
	Render::initialize(window);
	
	// Create a camera
	cam.reset(new Camera(
		glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		2.5f, 150.0f));

	// Prepare shaders and objects
	prepareShaders();
	prepareObjects();

	// Set initial view matrix and projection matrix
	// Note that since the matrices are stored in a uniform buffer object (UBO), this has to be done after shader creation
	// since the UBO is generated in prepareShaders() and cannot be filled with data if it doesn't exist yet
	cam->updateViewMatrix();
	cam->updateProjectionMatrix();
}

void ResourceManager::render() {
	// This clears the buffers
	Render::clearWindow();

	// Process plane
	shaders[0].use();
	plane->render();

	// Process cubes
	shaders[1].use();
	cubes[0].renderMultiple(shaders[1], objectPositions[0]);
	cubes[1].renderMultiple(shaders[1], objectPositions[1]);
	cubes[2].renderMultiple(shaders[1], objectPositions[2]);
}

Camera& ResourceManager::giveCamera() {
	return *cam;
}

const std::vector<Shader>& ResourceManager::giveShaders() {
	return shaders;
}

void ResourceManager::setViewMatrix(const glm::mat4& viewMatrix) {
	// Note that we aren't passing the updated matrix to a shader, but to our uniform buffer object (UBO) so that it gets updated across all shaders
	// Tells OpenGL we are currently working with this uniform buffer object (UBO)
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_ID);
	// Copy the data into the UBO
	// First argument is the buffer type
	// Second argument is the offset (view matrix occupies bytes 0-63, projection matrix 64-127)
	// Third argument is the data size (= 64 bytes) and 4th argument a pointer to the data
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &viewMatrix);
}

void ResourceManager::setProjectionMatrix(const glm::mat4& projectionMatrix) {
	// Note that we aren't passing the updated matrix to a shader, but to our uniform buffer object (UBO) so that it gets updated across all shaders
	// Tells OpenGL we are currently working with this uniform buffer object (UBO)
	glBindBuffer(GL_UNIFORM_BUFFER, UBO_ID);
	// Copy the data into the UBO
	// First argument is the buffer type
	// Second argument is the offset (view matrix occupies bytes 0-63, projection matrix 64-127)
	// Third argument is the data size (= 64 bytes) and 4th argument a pointer to the data
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &projectionMatrix);
}