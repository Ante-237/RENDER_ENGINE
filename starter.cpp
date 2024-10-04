#include <iostream>
#include "starter.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "std_image.h"
#include "Shader.h";

using namespace std;




#pragma region FUNCTION SIGNATURE

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

#pragma endregion


int main() {
	bool retFlag;


	int retVal = TextureExecution(retFlag);
	if (retFlag) return retVal;

	return 0;
}

int TextureExecution(bool& retFlag)
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

	glViewport(0, 0, 1200, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	Shader ourShader("4.2.texture.vs", "4.2.texture.fs");


	float vertices[] = {
		// positions		// colors			// texture coords
		0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	1.0f, 1.0f,// TOP RIGHT
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // BOTTOM RIGHT
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // BOTTOM LEFT
		-0.5F, 0.5F, 0.0f,	 1.0f, 1.0f, 0.0f,	0.0f, 1.0f// TOP LEFT
	};


	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};


	unsigned int VBO;
	unsigned int VAO;

	glGenBuffers(1, &VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// position attribute

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	unsigned int texture1, texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	if (data) {

	}




	// QUERY MAX VERTEX DECLARATION FOR OPENGL
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;



	int width, height, nrChannels;
	unsigned char* data = stbi_load("window.jpg", &width, &height, &nrChannels, 0);




	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float timeMove = glfwGetTime();
		float Move = sin(timeMove) * 0.2;


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

	
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0);

	


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	retFlag = false;
	return {};
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

	glViewport(0, 0, 1200, 600);

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

			-0.7f, 0, 0,      1.0f, 0.0f, 0.0f,
			-0.7f, -0.7f, 0,  0.0f, 1.0f, 0.0f,
			0.7f, 0.0f, 0,    0.0f, 0.0f, 1.0f,

			0.7f, 0.0f, 0,    1.0f, 0.0f, 0.0f,
			-0.7f, -0.7f, 0,  0.0f, 1.0f, 0.0f,
		    0.7f, -0.7f, 0,	  0.0f, 0.0f, 1.0f,

			-0.7f, 0, 0,	  1.0f, 0.0f, 0.0f,
			-0.5f, 0, 0,	  0.0f, 1.0f, 0.0f,
			-0.5f, 0.3f, 0,	  0.0f, 0.0f, 1.0f,

			0.5f, 0.3f, 0,	  1.0f, 0.0f, 0.0f,
			-0.5f, 0.3f, 0,	  0.0f, 1.0f, 0.0f,
			-0.5f, 0, 0,	  0.0f, 0.0f, 1.0f,
		
		    -0.5f, 0, 0,	  1.0f, 0.0f, 0.0f,
			0.5f, 0.3f, 0,	  0.0f, 1.0f, 0.0f,
			0.5f, 0, 0,		  0.0f, 0.0f, 1.0f,

			0.7f, 0, 0,		 1.0f, 0.0f, 0.0f,
			0.5f, 0.3f, 0,	 0.0f, 1.0f, 0.0f,
			0.5f, 0, 0,		 0.0f, 0.0f, 1.0f

	};

	float verticesTwo[] = { 
		/*0, 0.5f, 0,
		0.5f, 0.5f, 0,
		0.5f, -0.5f, 0,*/

		-0.3f, -0.2f, 0,
		-0.3f, -0.7f, 0,
		0, -0.7f,0, 

		0, -0.2f, 0,
		0, -0.7f, 0,
		-0.3f, -0.2f, 0


	};


	float verticesThree[] = {
		// positions		// colors
		0.4f, 0.0f, 0,	  
		0.4f, -0.2f, 0,	
		0.6f, -0.2f, 0,	

		0.6f, 0.0f, 0,	
		0.6f, -0.2f, 0,	
		0.4f, 0.0f, 0,	
	};

	//unsigned int indices[] = {
	//	0, 1, 3,
	//	1, 2, 3
	//};

	
	unsigned int VBOs[3];
	unsigned int VAOs[3];

	glGenBuffers(3, VBOs);

	// unsigned int EBO;
	// glGenBuffers(1, &EBO);

	glGenVertexArrays(3, VAOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOne), verticesOne, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);





	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTwo), verticesTwo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);



	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesThree), verticesThree, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);



	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"uniform float offset;\n"
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		// inverst the current vertex object
		// "gl_Position = vec4(aPos.x + offset, aPos.y * -1 , aPos.z , 1.0);\n"
		"gl_Position = vec4(aPos.x + offset, aPos.y  , aPos.z , 1.0);\n"
		"ourColor = gl_Position.xyz;\n"
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
		"in vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(ourColor, 1.0);\n"
		"}\n";

	const char* fragmentShaderSourceYellow = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 1.0f, 0.1f, 1.0f);\n"
		"}\n";

	const char* fragmentShaderSourceGreen = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = ourColor;\n"
		"}\n";


	unsigned int fragmentShader[3];
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

	fragmentShader[2] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[2], 1, &fragmentShaderSourceGreen, NULL);
	glCompileShader(fragmentShader[2]);

	glGetShaderiv(fragmentShader[2], GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader[2], 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPLICATION_FAILED\n" << infoLog << endl;
	}
	else {
		cout << "SUCCESS::SHADER::FRAGMENT" << endl;
	}

#pragma endregion

#pragma region SHADER PROGRAM

	unsigned int shaderProgram[3];
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


	shaderProgram[2] = glCreateProgram();
	glAttachShader(shaderProgram[2], vertexShader);
	glAttachShader(shaderProgram[2], fragmentShader[2]);
	glLinkProgram(shaderProgram[2]);

	glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram[2], 512, NULL, infoLog);
	}


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[1]);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[2]);





#pragma endregion


	// QUERY MAX VERTEX DECLARATION FOR OPENGL
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;



	int width, height, nrChannels;
	unsigned char* data = stbi_load("window.jpg", &width, &height, &nrChannels, 0);




	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram[0]);

		float timeMove = glfwGetTime();
		float Move = sin(timeMove) * 0.2;
		
		int vertexLocation = glGetUniformLocation(shaderProgram[0], "offset");
		glUniform1f(vertexLocation, Move);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 18);

		glUseProgram(shaderProgram[1]);

		int vertexSecondLocation = glGetUniformLocation(shaderProgram[1], "offset");
		glUniform1f(vertexSecondLocation, Move);
		

		

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0);

		glUseProgram(shaderProgram[2]);
		int vertexThirdLocation = glGetUniformLocation(shaderProgram[2], "offset");
		glUniform1f(vertexThirdLocation, Move);


		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram[2], "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


		glBindVertexArray(VAOs[2]);
		glDrawArrays(GL_TRIANGLES, 0, 6);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(3, VAOs);
	glDeleteBuffers(3, VBOs);
	glDeleteProgram(shaderProgram[1]);
	glDeleteProgram(shaderProgram[0]);
	glDeleteProgram(shaderProgram[2]);


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
