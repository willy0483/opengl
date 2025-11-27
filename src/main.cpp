#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
								 "layout (location = 0) in vec3 aPos;\n"
								 "void main()\n"
								 "{\n"
								 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								 "}\0";

// Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   "void main()\n"
								   "{\n"
								   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
								   "}\n\0";

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell OpenGL we are using CORE profile for modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Vertices coordinates
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // bottom left
		0.5f,  -0.5f, 0.0f, // bottom right
		0.0f,  0.5f,  0.0f, // top middle
	};

	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(800, 800, "Opengl", NULL, NULL);
	// Error check if the window fails to creade
	if(window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	glViewport(0, 0, 800, 800);

	// Crate Vertex Shader Object and get its reference
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Complie the Vertex Shader init machine code
	glCompileShader(vertexShader);

	// Crate Fragment Shader Object and get its reference
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Complie the Fragment Shader init machine code
	glCompileShader(fragmentShader);

	// Create  Shader Program Object and get its reference
	unsigned int shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the Shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader Objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create reference containers for the Vertex Array Object and the Vertex Buffer Object
	unsigned int VAO;
	unsigned int VBO;

	// Generate the VAO and VBO with only 1 Object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

    // Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

    // Bind the VBO specifying its a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Configure the Vertex Attribute so the OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable the Vertex Attribute so the OpenGL knows to use it
	glEnableVertexAttribArray(0);

    // Bind both the VAO and VBO to 0 so that we dont accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Set color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// Main while loop
	while(!glfwWindowShouldClose(window))
	{

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
        // Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
        // Draw the triangle using the GL_TRIANGLES primitive
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		// Take care of all GLFW events
		glfwPollEvents();
	}

    // Delete all the Objects we created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Delete window before closing the program
	glfwDestroyWindow(window);
	// Terminate GLFW before closing the program
	glfwTerminate();
	return 0;
}
