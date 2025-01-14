#include <librar/shader.h> //At top of file
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[21] = {
	//x   //y  //z   //r  //g  //b  //a
	-0.5, -0.5, 0.0, 0.98, 0.74, 0.011, 1.0, //Bottom left
	 0.5, -0.5, 0.0, 0.0, 0.71, 0.15, 1.0, //Bottom right
	 0.0,  0.5, 0.0, 0.011, 0.28, 0.98, 1.0  //Top center
};

//Creates a new vertex array object with vertex data
unsigned int createVAO(float* vertexData, int numVertices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Define position attribute (3 floats)
	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	return vao;
};

unsigned int createShader(GLenum shaderType, const char* sourceCode) {
	//Create a new vertex shader object
	unsigned int vertexShader = glCreateShader(shaderType);
	//Supply the shader object with source code
	glShaderSource(vertexShader, 1, &sourceCode, NULL);
	//Compile the shader object
	glCompileShader(vertexShader);
	
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		//512 is an arbitrary length, but should be plenty of characters for our error message.
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
	}
	return vertexShader;
};

unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	unsigned int shaderProgram = glCreateProgram();
	//Attach each stage
	glAttachShader(shaderProgram, vertexShader);
	//glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Link all the stages together
	glLinkProgram(shaderProgram);

	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
	}
	//The linked program now contains our compiled code, so we can delete these intermediate objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
};

int main() {

	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	std::string vertexShaderSource =librar::loadShaderSourceFromFile("assets/vertexShader.vert");
	std::string fragmentShaderSource = librar::loadShaderSourceFromFile("assets/fragmentShader.frag");
	unsigned int shader = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

	unsigned int shader = createShaderProgram(vertexShaderSource, fragmentShaderSource);
	unsigned int vao = createVAO(vertices, 3);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vao);

		//In render loop...
		//The current time in seconds this frame
		float time = (float)glfwGetTime();
		//Get the location of the uniform by name
		int timeLocation = glGetUniformLocation(shader, "_Time");
		//Set the value of the variable at the location
		glUniform1f(timeLocation, time);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}
