
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Vertices.h"
#include<iostream>

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main()
{
	// glfw: initialize and configure
   // ------------------------------
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	std::vector<glm::vec2> Texcroods =
	{
		glm::vec2(0.0f,0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f,1.0f),
		glm::vec2(1.0f,1.0f)
	};
	std::vector<glm::vec3> Position =
	{
		glm::vec3(-0.5f,-0.5f,0.0f),
		glm::vec3(0.5f,-0.5f,0.0f),
		glm::vec3(-0.5f,0.5f,0.0f),
		glm::vec3(0.5f,0.5f,0.0f)
	};
	std::vector<unsigned int> Indices =
	{
		1,2,3,
		0,2,1
	};

	CMesh MyMesh(Position, Texcroods, Indices, "test2.png");
	//Shader TexShader("./Shader/TexVertexShader.vert","./Shader/TexFragmentShader.frag");
	//Shader ColorShader("./Shader/ColorVertexShader.vert", "./Shader/ColorFragmentShader.frag");

	std::cin.get();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

