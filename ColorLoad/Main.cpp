#include<glad/glad.h>
#include<iostream>
#include<GLFW/glfw3.h>
#include<ctime>
#include "CModel.h"
#include "Camera.h"
#include "CTinyModel.h"
#include "CHandledModel.h"
#include "CBuildobj.h"
#include "CRebuiltModel.h"


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
	CTinyModel MyModel("./Model/nanosuit.obj");
	MyModel.changeTexToColor();	
	//MyModel.showMesh();
	//CBuildobj ReModel(MyModel);
	//CTinyModel RebuiltModel("./Model2/Remeshed-CornellBox-Empty-RG-404.obj");
	CTinyModel RebuiltModel("./Model/nanosuit-1.96k.obj");
	//CTinyModel MyModel("./Model/rewrittenobj2.obj");
	//CBuildobj ReObj(RebuiltModel);

	//RebuiltModel.calColor();
	//RebuiltModel.setMeshes();
	CHandleModel HandleModel(MyModel);
	SBox MyBox = MyModel.getBox();
	//MyModel.isVertexIn();
	std::vector<SVertex> Vertices = HandleModel.getVertices();
	std::vector<SVertex*> MyVertices;
	for (int i = 0; i < RebuiltModel.getMeshes().size(); i++)
	{				
		for(int k = 0;k< RebuiltModel.getMeshes()[i].getVertices().size();k++)
		MyVertices.push_back(&RebuiltModel.getMeshes()[i].getVertices()[k]);
	}
	clock_t Start = clock();
	COctree MyTree(Vertices, MyBox, 20, MyVertices);
	clock_t End = clock();
	
	std::cout << "Build Octree use " << End - Start << std::endl;
	//for (int i = 0; i < RebuiltModel.getMeshes()[0].getVertices().size(); i++)
	Start = clock();
	/*for (int i = 0; i < 30; i++)
	{		
		MyTree.doMatch(MyVertices[i]);
		std::cout << "in main "<< MyVertices[i].NN.size()<<std::endl;
		
	}*/
	for (int i = 0; i < MyVertices.size(); i++)
	{
		MyTree.doMatch(MyVertices[i]);
		MyVertices[i]->resetMeshID();
	}
	for (int i = 0; i < MyVertices.size() / 3; i++)
	{
		if (MyVertices[3 * i]->MeshID != MyVertices[3 * i + 1]->MeshID || MyVertices[3 * i]->MeshID != MyVertices[3 * i + 2]->MeshID)
		{
			MyVertices[3 * i + 1]->MeshID = MyVertices[3 * i]->MeshID;
			MyVertices[3 * i + 2]->MeshID = MyVertices[3 * i]->MeshID;
		}
	}
	for (int i = 0; i < MyVertices.size(); i++)
	{
		MyVertices[i]->calTexCoords();
	}
	End = clock();
	std::cout << "match vertices use " << End - Start << std::endl;	

	RebuiltModel.calColor();
	RebuiltModel.setMeshes();
	int Count = 0;
	for (int i = 0; i < MyVertices.size(); i++)
	{
		if (!MyVertices[i]->isHandled)
		{
			Count++;
			
		}
	}
	std::cout << Count << " miss" << std::endl;
	
	CHandleModel MatchedModel(RebuiltModel);
	std::vector<SVertex> MatchedVertices = MatchedModel.getVertices();

	CRebuiltModel TarModel(MyModel, MatchedVertices);
	TarModel.getModel().showMesh();
	CBuildobj Build(TarModel.getModel());
	
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
		RebuiltModel.drawInColor(OurShaderC);
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