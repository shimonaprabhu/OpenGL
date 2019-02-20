#pragma once

#include <iostream>
#include <vector>

#include "shader.h"
#include "texture.h"
#include "material.h"
#include "vertex.h"
#include "primitives.h"

class Mesh{

private:

	unsigned nrOfVertices;
	unsigned nrOfIndices;
	
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 ModelMatrix;

	void initVAO(Primitive* primitive){
		this->nrOfVertices=primitive->getNrOfVertices();
		this->nrOfIndices=primitive->getNrOfIndices();
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);	
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices*sizeof(Vertex), primitive->getVertices(), GL_STATIC_DRAW);
		if(this->nrOfIndices>0){
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices*sizeof(GLuint), primitive->getIndices(), GL_STATIC_DRAW);
		}
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);
		glBindVertexArray(0);
	}

	void initVAO(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices){
		this->nrOfVertices=nrOfVertices;
		this->nrOfIndices=nrOfIndices;
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);	
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices*sizeof(Vertex), vertexArray, GL_STATIC_DRAW);
		if(this->nrOfIndices>0){
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices*sizeof(GLuint), indexArray, GL_STATIC_DRAW);
		}
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);
		glBindVertexArray(0);
	}

	void updateUniforms(Shader* shader){
		shader->setMat4fv(this->ModelMatrix,"ModelMatrix");
	}

	void updateModelMatrix(){
		this->ModelMatrix=glm::mat4(1.f);
		this->ModelMatrix=glm::translate(this->ModelMatrix, this->position);
		this->ModelMatrix=glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix=glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix=glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix=glm::scale(this->ModelMatrix, this->scale);
	}

public:

	Mesh(Vertex* vertexArray, const unsigned& nrOfVertices, GLuint* indexArray, const unsigned& nrOfIndices, glm::vec3 position=glm::vec3(0.f), glm::vec3 rotation=glm::vec3(0.f), glm::vec3 scale=glm::vec3(1.f)){
		this->position=position;
		this->rotation=rotation;
		this->scale=scale;
		this->initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
		this->updateModelMatrix();
	}

	Mesh(Primitive* primitive, glm::vec3 position=glm::vec3(0.f), glm::vec3 rotation=glm::vec3(0.f), glm::vec3 scale=glm::vec3(1.f)){
		this->position=position;
		this->rotation=rotation;
		this->scale=scale;
		this->initVAO(primitive);
		this->updateModelMatrix();
	}
	~Mesh(){
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		if(this->nrOfIndices>0){
			glDeleteBuffers(1, &this->EBO);
		}
	}

	void update(){
	}

	void render(Shader* shader){
		this->updateModelMatrix();
		this->updateUniforms(shader);
		shader->use();
		glBindVertexArray(this->VAO);
		if(this->nrOfIndices==0)
			glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
		else
			glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
        
	}

	void setPosition(const glm::vec3 position){
		this->position=position;
	}
	void move(const glm::vec3 position){
		this->position+=position;
	}

	void setRotation(const glm::vec3 rotation){
		this->rotation=rotation;
	}

	void rotate(const glm::vec3 rotation){
		this->rotation+=rotation;
	}

	void setScale(const glm::vec3 scale){
		this->scale=scale;
	}

	void scaleUp(const glm::vec3 scale){
		this->scale+=scale;
	}
};