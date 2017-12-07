#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

#include "shader.h"
#include "camera.h"
#include "model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// Setting
GLuint SCREEN_WIDTH = 800;
GLuint SCREEN_HEIGHT = 600;

// Camera
Camera camera(glm::vec3(0.0f, 1.0f, 4.0f));
GLfloat lastX = SCREEN_WIDTH / 2.0f;
GLfloat lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

// Timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Lighting position
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Model position

std::vector<glm::vec3> modelPos = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

// Shader Control Bit
GLuint controlBit = 1;

int main(int argc, char* argv[])
{
	// GLFW initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW window creation
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Texture", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Enalble depth test
	glEnable(GL_DEPTH_TEST);

	// Build and compile shader program
	//Shader phongShader("resources/shaders/phong.vert", "resources/shaders/phong.frag");
	//Shader toonShader("resources/shaders/toon.vert", "resources/shaders/toon.frag");
	//Shader gouraudShader("resources/shaders/gouraud.vert", "resources/shaders/gouraud.frag");
	//Shader goochShader("resources/shaders/gooch.vert", "resources/shaders/gooch.frag");
	//Shader blinnphongShader("resources/shaders/blinnphong.vert", "resources/shaders/blinnphong.frag");
	Shader teapotShader("resources/shaders/teapot.vert", "resources/shaders/teapot.frag");

	// Load models
	Model teapot("resources/models/teapot.obj");

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Per-frame time logic
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		processInput(window);

		// Clear render buffer 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Move light source position
		//lightPos.x = 1.0f + glm::sin(glfwGetTime()) * 2.0f;
		//lightPos.y = glm::sin(glfwGetTime() / 2.0f) * 1.0f;
		//lightPos.z = -1.0f + glm::sin(glfwGetTime());

		// Model/view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		for (std::vector<glm::vec3>::iterator it = modelPos.begin(); it != modelPos.end(); ++it)
		{
			glm::mat4 model;
			model = glm::translate(model, *it);
			model = glm::scale(model, glm::vec3(0.6f, 1.4f, 1.0f));
			model = glm::rotate(model, (GLfloat)(0.4 * glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat3 NormalMatrix = glm::transpose(glm::inverse(model));

			// Enable shader before setting uniforms
			teapotShader.use();
			teapotShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
			teapotShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
			teapotShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
			teapotShader.setFloat("material.shininess", 32.0);
			teapotShader.setVec4("light.lightVector", glm::vec4(lightPos, 1.0f)); // Determine the property of light - directional or point(omni)
			teapotShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
			teapotShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
			teapotShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
			//teapotShader.setVec3("lightPos", lightPos);
			teapotShader.setVec3("viewPos", camera.Position);
			teapotShader.setInt("controlBit", controlBit);

			// Pass projection, view and model matrix to phong shader
			teapotShader.setMat4("projection", projection);
			teapotShader.setMat4("view", view);
			teapotShader.setMat4("model", model);
			teapotShader.setMat3("NormalMatrix", NormalMatrix);

			// Render the loaded model
			teapot.Draw(teapotShader);
		}
		
		
		//toonShader.use();
		//toonShader.setVec3("lightPos", lightPos);
		//toonShader.setMat4("projection", projection);
		//toonShader.setMat4("view", view);
		//toonShader.setMat4("model", model);
		//teapot.Draw(toonShader);
		
		//gouraudShader.use();
		//gouraudShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		//gouraudShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		//gouraudShader.setVec3("lightPos", lightPos);
		//gouraudShader.setVec3("viewPos", camera.Position);
		//gouraudShader.setMat4("projection", projection);
		//gouraudShader.setMat4("view", view);
		//gouraudShader.setMat4("model", model);
		//teapot.Draw(gouraudShader);

		//phongShader.use();
		//phongShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		//phongShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		//phongShader.setVec3("lightPos", lightPos);
		//phongShader.setVec3("viewPos", camera.Position);
		//phongShader.setMat4("projection", projection);
		//phongShader.setMat4("view", view);
		//phongShader.setMat4("model", model);
		//teapot.Draw(phongShader);

		//goochShader.use();
		//goochShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		//goochShader.setVec3("coolColor", 159.0f / 255, 148.0f / 255, 255.0f / 255);
		//goochShader.setVec3("warmColor", 255.0f / 255, 75.0f / 255, 75.0f / 255);
		//goochShader.setVec3("lightPos", lightPos);
		//goochShader.setMat4("projection", projection);
		//goochShader.setMat4("view", view);
		//goochShader.setMat4("model", model);
		//teapot.Draw(goochShader);

		//blinnphongShader.use();
		//blinnphongShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		//blinnphongShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		//blinnphongShader.setVec3("lightPos", lightPos);
		//blinnphongShader.setVec3("viewPos", camera.Position);
		//blinnphongShader.setMat4("projection", projection);
		//blinnphongShader.setMat4("view", view);
		//blinnphongShader.setMat4("model", model);
		//teapot.Draw(blinnphongShader);

		// Swap buff
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyborad(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyborad(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyborad(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyborad(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		controlBit = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		controlBit = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		controlBit = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		controlBit = 4;
	}
}
