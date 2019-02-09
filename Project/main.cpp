#include "libs.h"

Vertex vertices[]={
	glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(1.0f, 0.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.0f, 0.f, -1.f), 
	glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.0f, 0.f, -1.f), 
	glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f), glm::vec3(0.0f, 0.f, -1.f), 
	
	glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(1.f, 1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.0f, 0.f, -1.f)
};

unsigned nrOfVertices=sizeof(vertices)/sizeof(Vertex);

GLuint indices[]={
	0, 1, 2,
	0, 2, 3
};

unsigned nrOfIndices=sizeof(indices)/sizeof(GLuint);

void updateInput(GLFWwindow* window){
	if(glfwGetKey(window, GLFW_KEY_ESCAPE)==GLFW_PRESS){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void updateInput(GLFWwindow* window, glm::vec3& position,  glm::vec3& rotation,  glm::vec3& scale){
	if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS){
		position.z-=0.01f;
	}
	if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS){
		position.z+=0.01f;
	}
	if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS){
		position.x-=0.01f;
	}
	if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS){
		position.x+=0.01f;
	}
	if(glfwGetKey(window, GLFW_KEY_Q)==GLFW_PRESS){
		rotation.y-=1.f;
	}
	if(glfwGetKey(window, GLFW_KEY_E)==GLFW_PRESS){
		rotation.y+=1.f;
	}
	if(glfwGetKey(window, GLFW_KEY_Z)==GLFW_PRESS){
		scale+=0.1f;
	}
	if(glfwGetKey(window, GLFW_KEY_X)==GLFW_PRESS){
		scale-=0.1f;
	}
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
	const int WINDOW_HEIGHT=640;
	const int WINDOW_WIDTH=480;

	int framebufferWidth=0;
	int framebufferHeight=0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
    glfwMakeContextCurrent(window);
    

	glewExperimental=GL_TRUE;

	if(glewInit()!=GLEW_OK){
		std::cout<<"ERROR::MAIN.CPP::GLEW_INIT_FAILED"<<"\n";
		glfwTerminate();

	}
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Shader core_program("vertex_core.glsl", "fragment_core.glsl");


	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//GLuint attribLoc=getAttribLocation(core_program, "vertex_position");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
    // render loop
    // -----------
	Texture texture0("Images/fig.png", GL_TEXTURE_2D, 0);
	Texture texture1("Images/LG01.png", GL_TEXTURE_2D, 1);

	Material material0(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), texture0.getTextureUnit(), texture1.getTextureUnit());
	material0.sendToShader(core_program);

	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);

	glm::mat4 ModelMatrix(1.f);
	ModelMatrix=glm::translate(ModelMatrix, position);
	ModelMatrix=glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix=glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix=glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix=glm::scale(ModelMatrix, scale);
    
	glm::vec3 camPosition(0.f, 0.f, 1.f);
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);
	glm::mat4 ViewMatrix(1.f);

	ViewMatrix=glm::lookAt(camPosition, camPosition+camFront, worldUp);

	float fov=90.f;
	float nearPlane=0.1f;
	float farPlane=1000.f;

	glm::mat4 ProjectionMatrix(1.f);
	ProjectionMatrix=glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth)/framebufferHeight, nearPlane, farPlane);

	glm::vec3 lightPos0(0.f, 0.f, 2.f);


	core_program.setMat4fv(ModelMatrix, "ModelMatrix");
	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	
	core_program.setVec3f(lightPos0, "lightPos0");
	core_program.setVec3f(camPosition, "cameraPos");
	
	while (!glfwWindowShouldClose(window))
    {
		// render
        // ------
		glfwPollEvents();
		updateInput(window, position, rotation, scale);
		updateInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT);

		
		core_program.set1i(texture0.getTextureUnit(), "texture0");
		core_program.set1i(texture1.getTextureUnit(), "texture1");

		ModelMatrix=glm::mat4(1.f);
		ModelMatrix=glm::translate(ModelMatrix, position);
		ModelMatrix=glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix=glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix=glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix=glm::scale(ModelMatrix, scale);

		core_program.setMat4fv(ModelMatrix, "ModelMatrix");
		glfwGetFramebufferSize(window, &framebufferWidth,  &framebufferHeight);
		ProjectionMatrix=glm::perspective(glm::radians(fov), static_cast<float>(framebufferWidth)/framebufferHeight, nearPlane, farPlane);
		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");
		core_program.use();

		texture0.bind();
		texture1.bind();

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
		glFlush();
        
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}



