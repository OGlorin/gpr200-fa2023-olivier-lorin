#include <stdio.h>
#include <math.h>
#include <librar/shader.h>
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

librar::Vertex vertices[4] = {
	//x    y    z    u    v
   { 1.0 , 1.0 , 0.0 , 1.0 , 1.0 }, //Top right
   { -1.0 , 1.0 , 0.0 , 0.0 , 1.0 }, //Top Left
   { -1.0 , -1.0 , 0.0 , 0.0 , 0.0 }, //Bottom left
   { 1.0 , -1.0 , 0.0 , 1.0 , 0.0 }  //Bottom right
};

unsigned int indices[6] = {
	0 , 1 , 2 , //Triangle 1
	0 , 2 , 3  //Triangle 2
};

float SkyColor[3][3] = {
	//Day Colors
	{1.0, 0.17, 0.0}, {0.0, 0.2, 0.3},
	//Night Color
	{0.0, 0.0, 0.0}
};

float SunColor[2][3] = {
	{0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}
};

float SunRadius = 0.3;
float SunSpeed = 0.7;

float HillColor[1][3] = {
	{0.0, 0.0, 0.0}
};

bool showImGUIDemoWindow = false;

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
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	unsigned int vao = librar::createVAO(vertices, 4, indices, 6);
	librar::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniform
		shader.setVec2("_Resolution", (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);
		shader.setFloat("_Time", (float)glfwGetTime());
		shader.setFloat("_SunSpeed", SunSpeed);
		shader.setFloat("_SunRadius", SunRadius);
		shader.setVec3Array("_SkyColor", 4, SkyColor);
		shader.setVec3Array("_SunColor", 2, SunColor);
		shader.setVec3Array("_HillColor", 2, HillColor);
		//In render loop...
		//Draw using indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);

			ImGui::Separator();
			ImGui::Text("Daytime Colors");
			ImGui::ColorEdit3("Top", SkyColor[1]);
			ImGui::ColorEdit3("Bottom", SkyColor[0]);

			ImGui::Separator();
			ImGui::Text("Nighttime Color");
			ImGui::ColorEdit3("Color", SkyColor[2]);

			ImGui::Separator();
			ImGui::Text("Sun Settings");
			ImGui::ColorEdit3("Sunrise Color", SunColor[1]);
			ImGui::ColorEdit3("Sunset Color", SunColor[0]);
			ImGui::SliderFloat("Radius", &SunRadius, 0.1, 1.0);
			ImGui::SliderFloat("Speed", &SunSpeed, 0.1, 10.0);

			ImGui::Separator();
			ImGui::Text("Hill Color");
			ImGui::ColorEdit3("Color", HillColor[0]);
			ImGui::Separator();

			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

