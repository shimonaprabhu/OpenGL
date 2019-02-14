#include "Game.h"


void Game::initGLFW(){
	if(glfwInit()==GLFW_FALSE){
		std::cout<<"ERROR::GLFW_INIT_FAILED"<<"\n";
		glfwTerminate();
	}
}


void Game::initWindow(const char* title, bool resizable){
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->GL_VERSION_MINOR);
    glfwWindowHint(GLFW_RESIZABLE, resizable);
	this->window = glfwCreateWindow(this->WINDOW_HEIGHT, this->WINDOW_WIDTH, title, NULL, NULL);
	if(this->window==nullptr){
		std::cout<<"ERROR::GLFW_WINDOW_INIT_FAILED"<<"\n";
		glfwTerminate();
	}
	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
	glfwSetFramebufferSizeCallback(this->window, Game::framebuffer_size_callback);
	glfwMakeContextCurrent(this->window);
}


void Game::initGLEW(){
	glewExperimental=GL_TRUE;
	if(glewInit()!=GLEW_OK){
		std::cout<<"ERROR::MAIN.CPP::GLEW_INIT_FAILED"<<"\n";
		glfwTerminate();
	}
}

void Game::initOpenGLOptions(){
	glewExperimental=GL_TRUE;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

void Game::initMatrices(){
	this->ViewMatrix=glm::mat4(1.f);
	this->ViewMatrix=glm::lookAt(this->camPosition, this->camPosition+this->camFront, this->worldUp);
	this->ProjectionMatrix=glm::mat4(1.f);
	this->ProjectionMatrix=glm::perspective(glm::radians(this->fov), static_cast<float>(this->framebufferWidth)/this->framebufferHeight, this->nearPlane, this->farPlane);
	}


void Game::initShaders(){
	this->shaders.push_back(new Shader(this->GL_VERSION_MAJOR, this->GL_VERSION_MINOR, "vertex_core.glsl", "fragment_core.glsl"));
}

void Game::initTextures(){
	
	this->textures.push_back(new Texture("Images/fig.png", GL_TEXTURE_2D));
	this->textures.push_back(new Texture("Images/LG01.png", GL_TEXTURE_2D));
}

void Game::initMaterials(){
	this->materials.push_back(new Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 1));
	
}

void Game::initMeshes(){
	this->meshes.push_back(new Mesh(&Quad(), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(2.f)));
}

void Game::initLights(){
	this->lights.push_back(new glm::vec3 (0.f, 0.f, 1.f));
}

void Game::initUniforms(){
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ViewMatrix, "ViewMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(*this->lights[0], "lightPos0");
	this->shaders[SHADER_CORE_PROGRAM]->setVec3f(this->camPosition, "cameraPos");
}
	
void Game::updateUniforms(){
		glfwGetFramebufferSize(this->window, &this->framebufferWidth,  &this->framebufferHeight);
		ProjectionMatrix=glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth)/framebufferHeight, nearPlane, farPlane);
		this->shaders[SHADER_CORE_PROGRAM]->setMat4fv(ProjectionMatrix, "ProjectionMatrix");
}

Game::Game(const char* title, const int WINDOW_WIDTH, const int WINDOW_HEIGHT, const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR, bool resizable):WINDOW_WIDTH(WINDOW_WIDTH), WINDOW_HEIGHT(WINDOW_HEIGHT), GL_VERSION_MAJOR(GL_VERSION_MAJOR), GL_VERSION_MINOR(GL_VERSION_MINOR){
	this->window=nullptr;
	this->framebufferHeight=this->WINDOW_HEIGHT;
	this->framebufferWidth=this->WINDOW_WIDTH;
	
	this->camPosition=glm::vec3(0.f, 0.f, 1.f);
	this->worldUp=glm::vec3(0.f, 1.f, 0.f);
	this->camFront=glm::vec3(0.f, 0.f, -1.f);
	this->fov=90.f;
	this->nearPlane=0.1f;
	this->farPlane=1000.f;

	this->initGLFW();
	this->initWindow(title, resizable);
	this->initGLEW();
	this->initOpenGLOptions();
	this->initMatrices();
	this->initShaders();
	this->initTextures();
	this->initMaterials();
	this->initMeshes();
	this->initLights();
	this->initUniforms();
	}

Game::~Game(){
	glfwDestroyWindow(this->window);
	glfwTerminate();

	for(size_t i=0; i<this->shaders.size();i++)
		delete this->shaders[i];
	for(size_t i=0; i<this->textures.size();i++)
		delete this->textures[i];
	for(size_t i=0; i<this->materials.size();i++)
		delete this->materials[i];
	for(size_t i=0; i<this->meshes.size();i++)
		delete this->meshes[i];
	for(size_t i=0; i<this->lights.size();i++)
		delete this->lights[i];
	}


int Game::getWindowShouldClose(){
	return glfwWindowShouldClose(this->window);
}

void Game::setWindowShouldClose(){
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void Game::update(){
		glfwPollEvents();
}


void Game::render(){
		updateInput(window, *this->meshes[MESH_QUAD]);
		updateInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);
		this->updateUniforms();
		this->materials[MAT_1]->sendToShader(*this->shaders[SHADER_CORE_PROGRAM]);
		this->shaders[SHADER_CORE_PROGRAM]->use();

		this->textures[TEX_PUSHEEN0]->bind(0);
		this->textures[TEX_CONTAINER1]->bind(1);

		this->meshes[MESH_QUAD]->render(this->shaders[SHADER_CORE_PROGRAM]);
		glfwSwapBuffers(window);
		glFlush();
        
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
    
    }

void Game::framebuffer_size_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

void Game::updateInput(GLFWwindow* window, Mesh &mesh){
	if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS){
		mesh.move(glm::vec3(0.f, 0.f, -0.01f));
	}
	if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS){
		mesh.move(glm::vec3(0.f, 0.f, 0.01f));
	}
	if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS){
		mesh.move(glm::vec3(-0.01f, 0.f, 0.f));
	}
	if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS){
		mesh.move(glm::vec3(0.01f, 0.f, 0.f));
	}
	if(glfwGetKey(window, GLFW_KEY_Q)==GLFW_PRESS){
		mesh.rotate(glm::vec3(0.f, -1.f, 0.f));
	}
	if(glfwGetKey(window, GLFW_KEY_E)==GLFW_PRESS){
		mesh.rotate(glm::vec3(0.f, 1.f, 0.f));
	}
	if(glfwGetKey(window, GLFW_KEY_Z)==GLFW_PRESS){
		mesh.scaleUp(glm::vec3(1.f));
	}
	if(glfwGetKey(window, GLFW_KEY_X)==GLFW_PRESS){
		mesh.scaleUp(glm::vec3(-1.f));
	}
}

void Game::updateInput(GLFWwindow* window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
