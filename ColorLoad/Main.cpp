#include<glad/glad.h>
#include<iostream>
#include<GLFW/glfw3.h>
#include<thread>
#include<ctime>
#include "CModel.h"
#include "Camera.h"
#include "CTinyModel.h"
#include "CHandledModel.h"
#include "CBuildobj.h"
#include "COctree.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void callMulitThread(CTinyModel* vModel, CTinyModel vMatchModel, int vStart, int vEnd);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glEnable(GL_DEPTH_TEST);
	//CModel OurModel("./Model/nanosuit.obj");
	//OurModel.changeTexToColor();
	Shader OurShaderC("./Shader/ModelCVertexShader.vert", "./Shader/ModelCFragmentShader.frag");
	Shader OurShaderT("./Shader/ModelTVertexShader.vert", "./Shader/ModelTFragmentShader.frag");
	//CTinyModel MyModel("./Model2/CornellBox-Empty-RG.obj");
	//CTinyModel MyModel("./Model/nanosuit.obj");
	//MyModel.changeTexToColor();	
	//MyModel.showMesh();
	//CBuildobj ReModel(MyModel);
	//CTinyModel RebuiltModel("./Model2/Remeshed-CornellBox-Empty-RG-404.obj");
	//CTinyModel RebuiltModel("./Model/nanosuit-1.96k.obj");
	//CTinyModel MyModel("./Model/rewrittenobj2.obj");
	//CBuildobj ReObj(RebuiltModel);
	//callMulitThread(&RebuiltModel, MyModel, 0, 500);
	/*clock_t Start = clock();
	std::thread First(callMulitThread, &RebuiltModel, MyModel, 0, 7958);
	std::thread Second(callMulitThread, &RebuiltModel, MyModel, 7958, 15917);
	std::thread Third(callMulitThread, &RebuiltModel, MyModel, 15917, 23876);
	std::thread Forth(callMulitThread, &RebuiltModel, MyModel, 23876, 31835);
	std::thread Fifth(callMulitThread, &RebuiltModel, MyModel, 31835, 39794);
	std::thread Sixth(callMulitThread, &RebuiltModel, MyModel, 39794, 47753);
	
	First.join();
	Second.join();
	Third.join();
	Forth.join();
	Fifth.join();
	Sixth.join();
	clock_t End = clock();
	std::cout << "time used " << End - Start << std::endl;*/

	
	//RebuiltModel.calColor();
	//RebuiltModel.setMeshes();
	SBox MyBox(10, 0, 12, 2, 20, 10);
	std::vector<SVertex> Vertices;
	COctree MyTree(Vertices, MyBox, 4);
	MyTree.show();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		OurShaderC.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		OurShaderC.setMat4("projection", projection);
		OurShaderC.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		OurShaderC.setMat4("model", model);
		//OurModel.drawInColor(OurShaderC);
		//MyModel.drawInColor(OurShaderC);
		//RebuiltModel.drawInColor(OurShaderC);
		//OurShaderT.use();
		OurShaderT.setMat4("projection", projection);
		OurShaderT.setMat4("view", view);
		OurShaderT.setMat4("model", model);
		//OurModel.drawInTex(OurShaderT);
		//MyModel.drawInTex(OurShaderT);

		

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void callMulitThread(CTinyModel* vModel, CTinyModel vMatchModel, int vStart, int vEnd)
{
	vModel->matchVertexForMultiThread(vMatchModel, vStart, vEnd);
}