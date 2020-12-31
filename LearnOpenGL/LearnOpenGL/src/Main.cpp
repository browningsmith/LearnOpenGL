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

	//Set clear color to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Create and compile vertex shader

	//Vertex shader source code
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	//Create vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //Attach shader source code
	glCompileShader(vertexShader); //compile shader

	int success; //Variable to contain vertex shader compilation status
	char infoLog[512]; //String to contain an error log message
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //Get the compile status of vertex shader

	if (!success) //If compilation of vertex shader was not successful, print error message
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Create and compile fragment shader

	//fragment shader source code
	const char* fragmentShaderSource = "#version 330 core\n"
		"    out vec4 FragColor;\n"
		"    \n"
		"    \n"
		"    uniform vec4 myColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = myColor;\n"
		"}\n";

	//Create fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //Attach shader source code
	glCompileShader(fragmentShader); //compile shader

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success); //Get the compile status of fragment shader

	if (!success) //If compilation of fragment shader was not successful, print error message
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Link the vertex shader and fragment shader into a complete shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader); //Attach vertex shader
	glAttachShader(shaderProgram, fragmentShader); //Attach fragment shader
	glLinkProgram(shaderProgram); //Link

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); //Get the compile status of fragment shader

	if (!success) //If the program linking was not successful, print error message
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Delete vertex and fragment shaders now that the shader program has been compiled
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Now we work on drawing a triangle

	//Triangle coordinates
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	//Create vertex array object to store the following vertex buffer object configuration
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Create vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Copy over vertex data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Assign VBO, which should still be bound to array buffer, to location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //Enable the vertex attribute

	//Get the location of the myColor uniform
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "myColor");

	//Main render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window); //Process user input

		glClear(GL_COLOR_BUFFER_BIT); //Clear the window

		//Tell OpenGL to use this shader program for rendering
		glUseProgram(shaderProgram);

		//Bind the proper VAO
		glBindVertexArray(VAO);

		float timeValue = glfwGetTime(); //Get current time
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f; //Sett green value to be a sin wave based on current time
		
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		//Draw the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window); //Display new frame
		glfwPollEvents(); //Get user input
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