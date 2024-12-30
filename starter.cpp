
#include <iostream>
#include "starter.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "std_image.h"
#include "Shader.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "Camera.h"
#include <filesystem>



using namespace std;

namespace fs = std::filesystem;

float blendamount = 0.2f;
glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


bool firstMouse = true;
float yaw = -90.0f;	
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// lighting
glm::vec3 lightPos(1.2f, 0.0f, 2.0f);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


#pragma region FUNCTION SIGNATURE

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// unsigned int loadTexture(const char* path);
//int TextureExecution();
// int Projection();
int Introlighting();

#pragma endregion

int main() {
	

	Introlighting();
	return 0;
}

int Introlighting() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MankonEngine", NULL, NULL);
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


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);



	Shader lightingShader("lightmap.vs", "lightmap.fs");
	Shader lightCubeShader("1.light_cube.vs", "1.light_cube.fs");

	float vertices[] = {

		// position			 // normals				 // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glad_glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int diffuseMap, specularMap;

	
	
	


	
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("resources/textures/container2.png", &width, &height, &nrChannels, 0);
	glGenTextures(1, &diffuseMap);

	if (data) {

		std::cout << " Nr Channel value " << nrChannels << endl;

		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	}
	else 
	{
		fs::path demofilePath = "resources/textures/";
		const std::size_t PATHSIZE = demofilePath.string().size();

		std::cout << "Failed to load texture" << std::endl;
		std::cout << fs::current_path() << std::endl;
		std::cout << demofilePath << endl;
		std::cout << PATHSIZE << endl;

	}

    stbi_image_free(data);

	unsigned char* dataT = stbi_load("resources/textures/container2_specular.png", &width, &height, &nrChannels, 0);
	glGenTextures(1, &specularMap);

	if (dataT) {

			std::cout << " Nr Channel value " << nrChannels << endl;

		glBindTexture(GL_TEXTURE_2D, specularMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataT);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else 
	{
		fs::path demofilePath = "resources/textures/";
		const std::size_t PATHSIZE = demofilePath.string().size();
		std::cout << "Failed to load texture" << std::endl;
		std::cout << fs::current_path() << std::endl;
		std::cout << demofilePath << endl;
		std::cout << PATHSIZE << endl;
	}

	stbi_image_free(dataT);
	 
	
	lightingShader.use();
	// lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	


	while (!glfwWindowShouldClose(window))
	{
	
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		
		processInput(window);

	    
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		lightingShader.use();
	;
		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f);

		lightingShader.setVec3("objectColor", glm::vec3( 1.0f, 0.5f, 0.31f));
		lightingShader.setVec3("lightColor",glm::vec3( 1.0f, 1.0f, 1.0f));
		lightingShader.setVec3("lightPos", lightPos);
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setVec3("light.position", lightPos);
	
		// lightingShader.setVec3("light.ambient", ambientColor);
		//lightingShader.setVec3("light.diffuse", diffuseColor);

		lightingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		lightingShader.setVec3("material.ambient",glm::vec3( 0.2f, 0.2f, 0.2f));
		lightingShader.setVec3("material.diffuse",glm::vec3( 0.5f, 0.5f, 0.5f));
		lightingShader.setVec3("material.specular",glm::vec3( 1.0f, 1.0f, 1.0f));
		lightingShader.setFloat("material.shininess", 64.0f);
	

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
	
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		lightCubeShader.use();
		lightCubeShader.setVec3("lightColor", lightColor);

		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubeShader.setMat4("model", model);


		// lightPos = glm::translate(glm::vec4(lightPos, 0)) * deltaTime;
		lightPos.x = 0.0f + sin(glfwGetTime() * 5) * 2.0f;
		lightPos.z = 0.0f + cos(glfwGetTime() * 5) * 2.0f;

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return 0;
}
/*
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RED;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
*/
/*
int Projection()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	GLFWwindow* window = glfwCreateWindow(800, 600, "MankonEngine", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader ourShader("4.2.2.texture.vs", "4.2.texture.fs");
	

	//float vertices[] = {
	//	// positions		// colors			// texture coords
	//	0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	1.0f, 1.0f,// TOP RIGHT
	//	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // BOTTOM RIGHT
	//	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // BOTTOM LEFT
	//	-0.5F, 0.5F, 0.0f,	 1.0f, 1.0f, 0.0f,	0.0f, 1.0f// TOP LEFT
	//};

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};


	//unsigned int indices[] = {
	//	0, 1, 3,
	//	1, 2, 3
	//};


	unsigned int VBO;
	unsigned int VAO;

	glGenBuffers(1, &VBO);

	// unsigned int EBO;
	// glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	



	unsigned int texture1, texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);


	// texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set t`exture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("resources/textures/wall.jpeg", &width, &height, &nrChannels, 0);


	if (data) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


	}
	else {
		fs::path demofilePath = "resources/textures/";
		const std::size_t PATHSIZE = demofilePath.string().size();

		std::cout << "Failed to load texture" << std::endl;
		std::cout << fs::current_path() << std::endl;
		std::cout << demofilePath << endl;
		std::cout << PATHSIZE << endl;

	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("resources/textures/wallsecond.jpeg", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		cout << "failed to load" << endl;
	}

	stbi_image_free(data);

	// QUERY MAX VERTEX DECLARATION FOR OPENGL
	//*int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;


	ourShader.use();
	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
	ourShader.setInt("texture2", 1);

	glEnable(GL_DEPTH_TEST);

	glm::vec3 cubePositions[] = {
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


	cameraPos  =  glm::vec3(0.0f, 0.0f, 3.0f);
	cameraFront =  glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp =  glm::vec3(0.0f, 1.0f, 0.0f);



	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		glClearColor(0.1f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);


		ourShader.setFloat("blendAmount", blendamount);
		ourShader.use();

		//glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));

		//trans = glm::scale(trans, glm::vec3(0.2, 0.2, 0.2));

	
		glm::mat4  view = glm::mat4(1.0f);
		
		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		// view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.f);

		glBindVertexArray(VAO);

		// unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		// glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		int viewLoc = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		for (unsigned int i = 0; i < 10; i++) {

			if (i % 2 != 0) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * 20;
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.7f));
				ourShader.setMat4("model", model);

				
			}
			else {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * 0;
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));
				ourShader.setMat4("model", model);
			}

			glDrawArrays(GL_TRIANGLES, 0, 36);
			
		}
	
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
} */
//int TextureExecution()
//{
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
//	if (window == NULL) {
//		cout << "Failed to create GLFW window" << endl;
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwMakeContextCurrent(window);
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		cout << "Failed to initialize GLAD" << endl;
//		return -1;
//	}
//
//	glViewport(0, 0, 800, 600);
//
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//
//	Shader ourShader("4.2.texture.vs", "4.2.texture.fs");
//	Shader adShader("4.2.texture.vs", "4.2.texture.fs");
//
//	float vertices[] = {
//		// positions		// colors			// texture coords
//		0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	1.0f, 1.0f,// TOP RIGHT
//		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // BOTTOM RIGHT
//		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // BOTTOM LEFT
//		-0.5F, 0.5F, 0.0f,	 1.0f, 1.0f, 0.0f,	0.0f, 1.0f// TOP LEFT
//	};
//
//
//	unsigned int indices[] = {
//		0, 1, 3,
//		1, 2, 3
//	};
//
//
//	unsigned int VBO;
//	unsigned int VAO;
//
//	glGenBuffers(1, &VBO);
//
//	unsigned int EBO;
//	glGenBuffers(1, &EBO);
//
//	glGenVertexArrays(1, &VAO);
//
//	glBindVertexArray(VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//
//
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//	
//	// position attribute
//
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(2);
//
//
//
//	unsigned int texture1, texture2;
//
//	glGenTextures(1, &texture1);
//	glBindTexture(GL_TEXTURE_2D, texture1);
//
//
//	// texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	// set t`exture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//	// load image, create texture and generate mipmaps
//	int width, height, nrChannels;
//	stbi_set_flip_vertically_on_load(true);
//	unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
//
//
//	if (data) {
//
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	
//
//	}
//	else {
//		fs::path demofilePath = "resources/textures/";
//		const std::size_t PATHSIZE = demofilePath.string().size();
//
//		std::cout << "Failed to load texture" << std::endl;
//		std::cout << fs::current_path() << std::endl;
//		std::cout << demofilePath << endl;
//		std::cout << PATHSIZE << endl;
//		
//	}
//	stbi_image_free(data);
//
//	glGenTextures(1, &texture2);
//	glBindTexture(GL_TEXTURE_2D, texture2);
//
//	// texture wrapping parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	// set texture filtering parameters
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	stbi_set_flip_vertically_on_load(true);
//	data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
//
//	if (data) {
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//	}
//	else {
//		cout << "failed to load" << endl;
//	}
//
//	stbi_image_free(data);
//
//	// QUERY MAX VERTEX DECLARATION FOR OPENGL
//	int nrAttributes;
//	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;
//
//	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//	glm::mat4 trans = glm::mat4(1.0f);
//	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
//	trans = glm::scale(trans, glm::vec3(0.2, 0.2, 0.2));
//	// trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
//	vec = trans * vec;
//	std::cout << vec.x << vec.y << vec.z << endl;
//
//
//
//
//
//	ourShader.use();
//	glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
//	ourShader.setInt("texture2", 1);
//
//	
//
//
//	while (!glfwWindowShouldClose(window)) {
//
//		processInput(window);
//
//		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture1);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, texture2);
//
//		
//		ourShader.setFloat("blendAmount", blendamount);
//		ourShader.use();
//		
//		glm::mat4 trans = glm::mat4(1.0f);
//		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
//		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
//		
//		trans = glm::scale(trans, glm::vec3(0.2, 0.2, 0.2));
//
//		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
//		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
//
//		glBindVertexArray(VAO);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//
//
//
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, texture1);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, texture2);
//
//		
//		trans = glm::mat4(1.0f);
//		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
//		trans = glm::rotate(trans, sin((float)glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
//	
//		trans = glm::scale(trans, glm::vec3(0.3, 0.3, 0.3));
//
//		transformLoc = glGetUniformLocation(ourShader.ID, "transform");
//		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
//
//		glBindVertexArray(VAO);
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glDeleteVertexArrays(1, &VAO);
//	glDeleteBuffers(1, &VBO);
//	glDeleteBuffers(1, &EBO);
//
//	glfwTerminate();
//
//}
//
//int MainExecution(bool& retFlag)
//{
//	retFlag = true;
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	GLFWwindow* window = glfwCreateWindow(1200, 600, "LearnOpenGL", NULL, NULL);
//	if (window == NULL) {
//		cout << "Failed to create GLFW window" << endl;
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwMakeContextCurrent(window);
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		cout << "Failed to initialize GLAD" << endl;
//		return -1;
//	}
//
//	glViewport(0, 0, 1200, 600);
//
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//	// test area
//
//#pragma region SHADER WRITING
//	//float vertices[] = {
//	//	0.5f, 0.5f, 0.0f, // TOP RIGHT
//	//	0.5f, -0.5f, 0.0f,  // BOTTOM RIGHT
//	//	-0.5f, -0.5f, 0.0f,   // BOTTOM LEFT
//	//	-0.5F, 0.5F, 0.0f  // TOP LEFT
//	//};
//
//	//float vertices[] = { // traingles side by side
//	//	-0.5f, 0.5f, 0,
//	//	-0.5f, -0.5f, 0,
//	//	0, -0.5f, 0,
//	//	0, 0.5f, 0,
//	//	0.5f, 0.5f, 0,
//	//	0.5f, -0.5f, 0,
//	//};
//
//	float vertices[] = {
//		-0.5f, 0.5f, 0,
//		-0.5f, -0.5f, 0,
//		0, -0.5f, 0,
//		0, -0.5f, 0,
//		0.5f, -0.5f, 0,
//		0.5f, 0.5f, 0,
//
//
//	};
//
//
//
//	float verticesOne[] = {
//		/*	-0.5f, 0.5f, 0,
//			-0.5f, -0.5f, 0,
//			0, -0.5f, 0,*/
//
//			-0.7f, 0, 0,      1.0f, 0.0f, 0.0f,
//			-0.7f, -0.7f, 0,  0.0f, 1.0f, 0.0f,
//			0.7f, 0.0f, 0,    0.0f, 0.0f, 1.0f,
//
//			0.7f, 0.0f, 0,    1.0f, 0.0f, 0.0f,
//			-0.7f, -0.7f, 0,  0.0f, 1.0f, 0.0f,
//		    0.7f, -0.7f, 0,	  0.0f, 0.0f, 1.0f,
//
//			-0.7f, 0, 0,	  1.0f, 0.0f, 0.0f,
//			-0.5f, 0, 0,	  0.0f, 1.0f, 0.0f,
//			-0.5f, 0.3f, 0,	  0.0f, 0.0f, 1.0f,
//
//			0.5f, 0.3f, 0,	  1.0f, 0.0f, 0.0f,
//			-0.5f, 0.3f, 0,	  0.0f, 1.0f, 0.0f,
//			-0.5f, 0, 0,	  0.0f, 0.0f, 1.0f,
//		
//		    -0.5f, 0, 0,	  1.0f, 0.0f, 0.0f,
//			0.5f, 0.3f, 0,	  0.0f, 1.0f, 0.0f,
//			0.5f, 0, 0,		  0.0f, 0.0f, 1.0f,
//
//			0.7f, 0, 0,		 1.0f, 0.0f, 0.0f,
//			0.5f, 0.3f, 0,	 0.0f, 1.0f, 0.0f,
//			0.5f, 0, 0,		 0.0f, 0.0f, 1.0f
//
//	};
//
//	float verticesTwo[] = { 
//		/*0, 0.5f, 0,
//		0.5f, 0.5f, 0,
//		0.5f, -0.5f, 0,*/
//
//		-0.3f, -0.2f, 0,
//		-0.3f, -0.7f, 0,
//		0, -0.7f,0, 
//
//		0, -0.2f, 0,
//		0, -0.7f, 0,
//		-0.3f, -0.2f, 0
//
//
//	};
//
//
//	float verticesThree[] = {
//		// positions		// colors
//		0.4f, 0.0f, 0,	  
//		0.4f, -0.2f, 0,	
//		0.6f, -0.2f, 0,	
//
//		0.6f, 0.0f, 0,	
//		0.6f, -0.2f, 0,	
//		0.4f, 0.0f, 0,	
//	};
//
//	//unsigned int indices[] = {
//	//	0, 1, 3,
//	//	1, 2, 3
//	//};
//
//	
//	unsigned int VBOs[3];
//	unsigned int VAOs[3];
//
//	glGenBuffers(3, VBOs);
//
//	// unsigned int EBO;
//	// glGenBuffers(1, &EBO);
//
//	glGenVertexArrays(3, VAOs);
//
//	glBindVertexArray(VAOs[0]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOne), verticesOne, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//
//
//
//
//	glBindVertexArray(VAOs[1]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTwo), verticesTwo, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//	glEnableVertexAttribArray(0);
//
//
//
//	glBindVertexArray(VAOs[2]);
//	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesThree), verticesThree, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//	glEnableVertexAttribArray(0);
//
//
//
//	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//
//	const char* vertexShaderSource = "#version 330 core\n"
//		"layout (location = 0) in vec3 aPos;\n"
//		"layout (location = 1) in vec3 aColor;\n"
//		"uniform float offset;\n"
//		"out vec3 ourColor;\n"
//		"void main()\n"
//		"{\n"
//		// inverst the current vertex object
//		// "gl_Position = vec4(aPos.x + offset, aPos.y * -1 , aPos.z , 1.0);\n"
//		"gl_Position = vec4(aPos.x + offset, aPos.y  , aPos.z , 1.0);\n"
//		"ourColor = gl_Position.xyz;\n"
//		"}\0";
//
//	unsigned int vertexShader;
//	vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//	glCompileShader(vertexShader);
//
//	int success;
//	char infoLog[512];
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//
//	if (!success) {
//		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//		cout << "ERROR::SHADER::VERTEX::COMPLICATION_FAILED\n" << infoLog << endl;
//	}
//	else {
//		cout << "SUCCESS::SHADER::VERTEX" << endl;
//	}
//
//	const char* fragmentShaderSource = "#version 330 core\n"
//		"out vec4 FragColor;\n"
//		"in vec3 ourColor;\n"
//		"void main()\n"
//		"{\n"
//		"FragColor = vec4(ourColor, 1.0);\n"
//		"}\n";
//
//	const char* fragmentShaderSourceYellow = "#version 330 core\n"
//		"out vec4 FragColor;\n"
//		"void main()\n"
//		"{\n"
//		"FragColor = vec4(1.0f, 1.0f, 0.1f, 1.0f);\n"
//		"}\n";
//
//	const char* fragmentShaderSourceGreen = "#version 330 core\n"
//		"out vec4 FragColor;\n"
//		"uniform vec4 ourColor;\n"
//		"void main()\n"
//		"{\n"
//		"FragColor = ourColor;\n"
//		"}\n";
//
//
//	unsigned int fragmentShader[3];
//	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader[0], 1, &fragmentShaderSource, NULL);
//	glCompileShader(fragmentShader[0]);
//
//	glGetShaderiv(fragmentShader[0], GL_COMPILE_STATUS, &success);
//
//	if (!success) {
//		glGetShaderInfoLog(fragmentShader[0], 512, NULL, infoLog);
//		cout << "ERROR::SHADER::FRAGMENT::COMPLICATION_FAILED\n" << infoLog << endl;
//	}
//	else {
//		cout << "SUCCESS::SHADER::FRAGMENT" << endl;
//	}
//
//
//	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader[1], 1, &fragmentShaderSourceYellow, NULL);
//	glCompileShader(fragmentShader[1]);
//
//	glGetShaderiv(fragmentShader[1], GL_COMPILE_STATUS, &success);
//
//	if (!success) {
//		glGetShaderInfoLog(fragmentShader[1], 512, NULL, infoLog);
//		cout << "ERROR::SHADER::FRAGMENT::COMPLICATION_FAILED\n" << infoLog << endl;
//	}
//	else {
//		cout << "SUCCESS::SHADER::FRAGMENT" << endl;
//	}
//
//	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader[2], 1, &fragmentShaderSourceGreen, NULL);
//	glCompileShader(fragmentShader[2]);
//
//	glGetShaderiv(fragmentShader[2], GL_COMPILE_STATUS, &success);
//
//	if (!success) {
//		glGetShaderInfoLog(fragmentShader[2], 512, NULL, infoLog);
//		cout << "ERROR::SHADER::FRAGMENT::COMPLICATION_FAILED\n" << infoLog << endl;
//	}
//	else {
//		cout << "SUCCESS::SHADER::FRAGMENT" << endl;
//	}
//
//#pragma endregion
//
//#pragma region SHADER PROGRAM
//
//	unsigned int shaderProgram[3];
//	shaderProgram[0] = glCreateProgram();
//	glAttachShader(shaderProgram[0], vertexShader);
//	glAttachShader(shaderProgram[0], fragmentShader[0]);
//	glLinkProgram(shaderProgram[0]);
//
//	glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLog);
//	}
//
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader[0]);
//
//
//
//	shaderProgram[1] = glCreateProgram();
//	glAttachShader(shaderProgram[1], vertexShader);
//	glAttachShader(shaderProgram[1], fragmentShader[1]);
//	glLinkProgram(shaderProgram[1]);
//
//	glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLog);
//	}
//
//
//	shaderProgram[2] = glCreateProgram();
//	glAttachShader(shaderProgram[2], vertexShader);
//	glAttachShader(shaderProgram[2], fragmentShader[2]);
//	glLinkProgram(shaderProgram[2]);
//
//	glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(shaderProgram[2], 512, NULL, infoLog);
//	}
//
//
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader[1]);
//	glDeleteShader(fragmentShader[0]);
//	glDeleteShader(fragmentShader[2]);
//
//
//
//
//
//#pragma endregion
//
//
//	// QUERY MAX VERTEX DECLARATION FOR OPENGL
//	int nrAttributes;
//	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
//	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;
//
//
//
//	int width, height, nrChannels;
//	unsigned char* data = stbi_load("window.jpg", &width, &height, &nrChannels, 0);
//
//
//
//
//	while (!glfwWindowShouldClose(window)) {
//		processInput(window);
//
//		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		glUseProgram(shaderProgram[0]);
//
//		float timeMove = glfwGetTime();
//		float Move = sin(timeMove) * 0.2;
//		
//		int vertexLocation = glGetUniformLocation(shaderProgram[0], "offset");
//		glUniform1f(vertexLocation, Move);
//
//		glBindVertexArray(VAOs[0]);
//		glDrawArrays(GL_TRIANGLES, 0, 18);
//
//		glUseProgram(shaderProgram[1]);
//
//		int vertexSecondLocation = glGetUniformLocation(shaderProgram[1], "offset");
//		glUniform1f(vertexSecondLocation, Move);
//		
//
//		
//
//		glBindVertexArray(VAOs[1]);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//		// glBindVertexArray(0);
//
//		glUseProgram(shaderProgram[2]);
//		int vertexThirdLocation = glGetUniformLocation(shaderProgram[2], "offset");
//		glUniform1f(vertexThirdLocation, Move);
//
//
//		float timeValue = glfwGetTime();
//		float greenValue = sin(timeValue) / 2.0f + 0.5f;
//		int vertexColorLocation = glGetUniformLocation(shaderProgram[2], "ourColor");
//		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
//
//
//		glBindVertexArray(VAOs[2]);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//
//
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glDeleteVertexArrays(3, VAOs);
//	glDeleteBuffers(3, VBOs);
//	glDeleteProgram(shaderProgram[1]);
//	glDeleteProgram(shaderProgram[0]);
//	glDeleteProgram(shaderProgram[2]);
//
//
//	glfwTerminate();
//	retFlag = false;
//	return {};
//}

/*
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
*/

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		cout << "Escape Key Pressed to quit" << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		
		if (blendamount < 1.0) {
			blendamount += 0.001f;
		}
		else {
			blendamount = 1.0f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (blendamount > 0) {
			blendamount -= 0.001f;
		}
		else {
			blendamount = 0.0f;
		}
	}

	const float cameraSpeed = 0.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);


}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);


	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
