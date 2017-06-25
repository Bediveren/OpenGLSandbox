
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <math.h>

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600

#define SUCCESS			0x00000000
#define FAILURE			0x00000001

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.7f, 0.3f, 1.0f);\n"
"}\n\0";

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


float clampFloat(float value, float min, float max)
{
	if (value < min) return min;
	else if (value > max) return max;
	else return value;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int createVertexShader(unsigned int* vertexShader)
{
	int success;
	*vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(*vertexShader);

	glGetShaderiv(*vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		std::cout << "Failed to compile vertex shader:" << std::endl;

		char error[512];
		glGetShaderInfoLog(*vertexShader, 512, NULL, error);
		std::cout << error << std::endl;

		return FAILURE;
	}
	return SUCCESS;

}

int main()
{
	// Initializing GLFW
	//-----------------------------------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	unsigned int VBO;
	unsigned int VAO;

	
	// GLFW window creation
	//-----------------------------------------------------------------------------
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RE renderer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to initialize renderer" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load pointers to OpenGL functions
	//-----------------------------------------------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "FAILED TO LOAD GLAD" << std::endl;
		Sleep(1500);
		glfwTerminate();
		return -1;
	}

	// Success param
	//-----------------------------------------------------------------------------
	int success;

	// Vertex shader
	//-----------------------------------------------------------------------------
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);


	if (!success)
	{
		char error[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, error);
		std::cout << "Error compiling shader:" << std::endl << error << std::endl;
		Sleep(500);
		return -1;
	}

	
	// Fragment shader
	//-----------------------------------------------------------------------------
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char error[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, error);
		std::cout << "Error compiling shader:" << std::endl << error << std::endl;
		Sleep(500);
		return -1;
	}

	// Shader program
	//-----------------------------------------------------------------------------
	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char error[512];
		glGetShaderInfoLog(shaderProgram, 512, NULL, error);
		std::cout << "Error compiling shader:" << std::endl << error << std::endl;
		Sleep(500);
		return -1;
	}


	// Discarding used variables.
	//-----------------------------------------------------------------------------
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// VAO
	//-----------------------------------------------------------------------------
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Render code
		//-----------------------------------------------------------------------------
		glClearColor(0.1f, 0.1f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	glDeleteProgram(shaderProgram);

	return 0;
}