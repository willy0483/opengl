#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// Vertices coordinates
float vertices[] = {

	//              Coordinates                       Colors
	-0.5f,	   -0.5f * float(sqrt(3)) / 3,	  0.0f, 0.8f, 0.3f,	 0.02f, // lower left
	0.5f,	   -0.5f * float(sqrt(3)) / 3,	  0.0f, 0.8f, 0.3f,	 0.02f, // lower right
	0.0f,	   0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 1.0f, 0.6f,	 0.32f, // upper
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6,	  0.0f, 0.9f, 0.45f, 0.17f, // inner left
	0.5f / 2,  0.5f * float(sqrt(3)) / 6,	  0.0f, 0.9f, 0.45f, 0.17f, // inner right
	0.0f,	   -0.5f * float(sqrt(3)) / 3,	  0.0f, 0.8f, 0.3f,	 0.02f, // inner down
};

unsigned int indices[] = {
	5, 3, 0, // lower left triangle
	4, 2, 3, // lower right triangle
	1, 4, 5, // upper triangle
};

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell OpenGL we are using CORE profile for modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
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

	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("src/shaders/default.vert", "src/shaders/default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generate Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generate Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Link VBO attributes sch as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Get ID if the uniform celled "scale"
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Main while loop
	while(!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Assign a valye to the uniform; NOTE: must be done after program activate
		glUniform1f(uniID, 0.5f);

		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the Objects we created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Delete window before closing the program
	glfwDestroyWindow(window);
	// Terminate GLFW before closing the program
	glfwTerminate();
	return 0;
}
