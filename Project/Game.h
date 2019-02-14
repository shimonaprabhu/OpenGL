#pragma once
#include "libs.h"

enum shader_enums{SHADER_CORE_PROGRAM=0};
enum texture_enums{TEX_PUSHEEN0=0, TEX_CONTAINER1};
enum material_enums{MAT_1=0};
enum mesh_enums{MESH_QUAD=0};


class Game{
private:
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

	glm::mat4 ViewMatrix;
	glm::vec3 camPosition;
	glm::vec3 worldUp;
	glm::vec3 camFront;
	glm::mat4 ProjectionMatrix;
	float fov;
	float nearPlane;
	float farPlane;

	std::vector<Shader*> shaders;
	std::vector<Texture*> textures;
	std::vector<Material*> materials;
	std::vector<Mesh*> meshes;
	std::vector<glm::vec3*> lights;
	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW();
	void initOpenGLOptions();
	void initMatrices();
	void initShaders();
	void initTextures();
	void initMaterials();
	void initMeshes();
	void initLights();
	void initUniforms();
	void updateUniforms();
public:
	Game(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable);
	virtual ~Game();
	int getWindowShouldClose();
	void setWindowShouldClose();
	void update();
	void render();

	static void framebuffer_size_callback(GLFWwindow* window, int fbW, int fbH);
	static void updateInput(GLFWwindow* window);
	static void updateInput(GLFWwindow* window, Mesh &mesh);
};