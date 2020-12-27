#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

	if (!glfwInit()) { //Initialize glfw

		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	//Ensure version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Uncomment for Mac OS X

	//Create a new window 800x600 pixels
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) //If window failed to open, print error and exit
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Set the new window as current context
	glfwMakeContextCurrent(window);

	//Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //If GLAD did not initialize, print error and exit
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600); //Set viewport to match initial window dimensions

	//Tell GLFW to call framebuffer_size_callback when the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Set clear color to  red
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	//Main render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window); //Process user input

		glfwSwapBuffers(window); //Display new frame
		glfwPollEvents(); //Get user input

		glClear(GL_COLOR_BUFFER_BIT); //Clear the window
	}

	glfwTerminate(); //Terminate GLFW

	return 0;
}

/*
 * Function framebuffer_size_callback
 *
 * Input: GLFWwindow* window, int width, int height
 * Output: void
 *
 * Function to be called whenever the window is resized, to resize the
 * OpenGL viewport
 */

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {

	glViewport(0, 0, width, height); //Resize viewport to match dimensions of window
}

/*
 * Function processInput
 *
 * Input: GLFWwindow* window
 * Output: void
 *
 * Function to be called on each render to check for user input
 */

void processInput(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {

		glfwSetWindowShouldClose(window, true);
	}
}