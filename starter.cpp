#include <iostream>
#include "starter.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "twoHello.h"

using namespace std;




#pragma region FUNCTION SIGNATURE

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

#pragma endregion


int main() {
	bool retFlag;


	int retVal = MainExecution(retFlag);
	if (retFlag) return retVal;

	return 0;
}

int MainExecution(bool& retFlag)
{
	retFlag = true;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1200, 600, "LearnOpenGL", NULL, NULL);
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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// test area

#pragma region SHADER WRITING
	//float vertices[] = {
	//	0.5f, 0.5f, 0.0f, // TOP RIGHT
	//	0.5f, -0.5f, 0.0f,  // BOTTOM RIGHT
	//	-0.5f, -0.5f, 0.0f,   // BOTTOM LEFT
	//	-0.5F, 0.5F, 0.0f  // TOP LEFT
	//};

	//float vertices[] = { // traingles side by side
	//	-0.5f, 0.5f, 0,
	//	-0.5f, -0.5f, 0,
	//	0, -0.5f, 0,
	//	0, 0.5f, 0,
	//	0.5f, 0.5f, 0,
	//	0.5f, -0.5f, 0,
	//};

	float vertices[] = {
		-0.5f, 0.5f, 0,
		-0.5f, -0.5f, 0,
		0, -0.5f, 0,
		0, -0.5f, 0,
		0.5f, -0.5f, 0,
		0.5f, 0.5f, 0,


	};



	float verticesOne[] = {
		/*	-0.5f, 0.5f, 0,
			-0.5f, -0.5f, 0,
			0, -0.5f, 0,*/

			-0.7f, -0.7f, 0,
			-0.7f, 0.7f, 0,
			0.7f, 0.7f, 0,
			0.7f, 0.7f, 0,
			-0.7f, -0.7f, 0,
		    0.7f, -0.7f, 0
			
	};

	float verticesTwo[] = { 0, 0.5f, 0,0.5f, 0.5f, 0,0.5f, -0.5f, 0, };

	//unsigned int indices[] = {
	//	0, 1, 3,
	//	1, 2, 3
	//};


	unsigned int VBOs[2];
	unsigned int VAOs[2];

	glGenBuffers(2, VBOs);

	// unsigned int EBO;
	// glGenBuffers(1, &EBO);

	glGenVertexArrays(2, VAOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOne), verticesOne, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTwo), verticesTwo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPLICATION_FAILED\n" << infoLog << endl;
	}
	else {
		cout << "SUCCESS::SHADER::VERTEX" << endl;
	}

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n";

	const char* fragmentShaderSourceYellow = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 1.0f, 0.1f, 1.0f);\n"
		"}\n";


	unsigned int fragmentShader[2];
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[0], 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader[0]);

	glGetShaderiv(fragmentShader[0], GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader[0], 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPLICATION_FAILED\n" << infoLog << endl;
	}
	else {
		cout << "SUCCESS::SHADER::FRAGMENT" << endl;
	}


	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, &fragmentShaderSourceYellow, NULL);
	glCompileShader(fragmentShader[1]);

	glGetShaderiv(fragmentShader[1], GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader[1], 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPLICATION_FAILED\n" << infoLog << endl;
	}
	else {
		cout << "SUCCESS::SHADER::FRAGMENT" << endl;
	}


#pragma endregion

#pragma region SHADER PROGRAM

	unsigned int shaderProgram[2];
	shaderProgram[0] = glCreateProgram();
	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[0], fragmentShader[0]);
	glLinkProgram(shaderProgram[0]);

	glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[0]);



	shaderProgram[1] = glCreateProgram();
	glAttachShader(shaderProgram[1], vertexShader);
	glAttachShader(shaderProgram[1], fragmentShader[1]);
	glLinkProgram(shaderProgram[1]);

	glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[1]);
	glDeleteShader(fragmentShader[0]);





#pragma endregion




	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram[0]);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glUseProgram(shaderProgram[1]);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram[1]);
	glDeleteProgram(shaderProgram[0]);


	glfwTerminate();
	retFlag = false;
	return {};
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		cout << "Escape Key Pressed to quit" << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


