
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

using namespace glm;
float tambah;
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

GLuint loadCubemap(vector<const GLchar*> faces);
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
// Properties
GLuint screenWidth = 800, screenHeight = 600;
// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//global variabel
int i;
// The MAIN function, from here we start the application and run the game loop

int main()
{
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	// Game loop
	//skybox
	Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.frag");
	// Setup and compile our shaders
	Shader shader("shaders/planet/model_loading.vs", "shaders/planet/model_loading.fs");

	// Load objek
	Model venus("src/venus/venus.obj");
	Model matahari("src/matahari/matahari.obj");
	Model bumi("src/bumi/bumi2.obj");
	Model merkurius("src/merkurius/merkurius2.obj");
	Model mars("src/mars/mars1.obj");
	Model jupiter("src/jupiter/jupiter1.obj");
	Model saturnus("src/saturnus/saturnus1.obj");
	Model saturnus_ring("src/saturnus/saturnus_ring1.obj");
	Model uranus("src/uranus/uranus1.obj");
	Model neptunus("src/neptunus/neptunus1.obj");
	Model orbit_p("src/orbit/orbit.obj");

	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};
	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	// Cubemap (Skybox)
	vector<const GLchar*> faces;
	faces.push_back("skybox/right.png");
	faces.push_back("skybox/left.png");
	faces.push_back("skybox/top.png");
	faces.push_back("skybox/bot.png");
	faces.push_back("skybox/front.png");
	faces.push_back("skybox/back.png");

	GLuint cubemapTexture = loadCubemap(faces);
	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Transformation matrices
		glm::mat4 projection_venus = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view_venus = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection_venus));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view_venus));

		// Draw skybox first
		glDepthMask(GL_FALSE);// Remember to turn depth writing off
		skyboxShader.Use();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glm::mat4 projection = glm::perspective(120.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
		// Render
		// MATAHARI
		shader.Use();
		// Draw the loaded model
		glm::mat4 model_matahari;
		model_matahari = glm::translate(model_matahari, glm::vec3(0.0f, 0.0f, 0.0f));
		model_matahari = glm::rotate(model_matahari, (GLfloat)glfwGetTime()*10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model_matahari = glm::scale(model_matahari, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_matahari));
		matahari.Draw(shader);

		// MERKURIUS
		shader.Use();
		// Draw the loaded model
		glm::mat4 model_merkurius;
		model_merkurius = glm::translate(model_merkurius, glm::vec3(0.0f, 0.0f, 0.0f));
		model_merkurius = glm::rotate(model_merkurius, (GLfloat)glfwGetTime()*15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model_merkurius = glm::scale(model_merkurius, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_merkurius));
		merkurius.Draw(shader);

		// VENUS
		shader.Use();
		// Draw the loaded model
		glm::mat4 model_venus;
		model_venus = glm::translate(model_venus, glm::vec3(0.0f, 0.0f, 0.0f));
		model_venus = glm::rotate(model_venus, (GLfloat)glfwGetTime()*18.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model_venus = glm::scale(model_venus, glm::vec3(0.045f, 0.045f, 0.045f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_venus));
		venus.Draw(shader);

		// BUMI
		shader.Use();
		// Draw the loaded model
		glm::mat4 model_bumi;
		model_bumi = glm::translate(model_bumi, glm::vec3(0.0f, 0.0f, 0.0f));
		model_bumi = glm::rotate(model_bumi, (GLfloat)glfwGetTime()*21.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model_bumi = glm::scale(model_bumi, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_bumi));
		bumi.Draw(shader);

		// MARS
		shader.Use();
		// Draw the loaded model
		glm::mat4 model_mars;
		model_mars = glm::translate(model_mars, glm::vec3(0.0f, 0.0f, 0.0f));
		model_mars = glm::rotate(model_mars, (GLfloat)glfwGetTime()*24.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model_mars = glm::scale(model_mars, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_mars));
		mars.Draw(shader);

		// JUPITER
		shader.Use();
		// Draw the loaded model
		glm::mat4 model_jupiter;
		model_jupiter = glm::translate(model_jupiter, glm::vec3(0.0f, 0.0f, 0.0f));
		model_jupiter = glm::rotate(model_jupiter, (GLfloat)glfwGetTime()*27.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model_jupiter = glm::scale(model_jupiter, glm::vec3(0.24f, 0.24f, 0.24f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_jupiter));
		jupiter.Draw(shader);

		// SATURNUS
		shader.Use();
		// Draw the loaded model
		glm::mat4 model_saturnus;
		model_saturnus = glm::translate(model_saturnus, glm::vec3(0.0f, 0.0f, 0.0f));
		model_saturnus = glm::rotate(model_saturnus, (GLfloat)glfwGetTime()*30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model_saturnus = glm::scale(model_saturnus, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_saturnus));
		saturnus.Draw(shader);
		// SATURNUS_RING
		shader.Use();
		// Draw the loaded model
		glm::mat4 model_saturnus_ring;
		model_saturnus_ring = glm::translate(model_saturnus_ring, glm::vec3(0.0f, 0.0f, 0.0f));
		model_saturnus_ring = glm::rotate(model_saturnus_ring, (GLfloat)glfwGetTime()*30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model_saturnus_ring = glm::scale(model_saturnus_ring, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_saturnus_ring));
		saturnus_ring.Draw(shader);

		// URANUS
		shader.Use();
		// Draw the loaded model
		glm::mat4 model_uranus;
		model_uranus = glm::translate(model_uranus, glm::vec3(0.0f, 0.0f, 0.0f));
		model_uranus = glm::rotate(model_uranus, (GLfloat)glfwGetTime()*33.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model_uranus = glm::scale(model_uranus, glm::vec3(0.17f, 0.17f, 0.17f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_uranus));
		uranus.Draw(shader);

		// NEPTUNUS
		shader.Use();
		// Draw the loaded model
		glm::mat4 model_neptunus;
		model_neptunus = glm::translate(model_neptunus, glm::vec3(0.0f, 0.0f, 0.0f));
		model_neptunus = glm::rotate(model_neptunus, (GLfloat)glfwGetTime()*36.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		model_neptunus = glm::scale(model_neptunus, glm::vec3(0.16f, 0.16f, 0.16f));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model_neptunus));
		neptunus.Draw(shader);

		// ORBIT
		for (i = 0; i < 9; i++){
			shader.Use();
			glm::mat4 orbit;
			orbit = glm::translate(orbit, glm::vec3(0.0f, 0.0f, 0.0f));
			orbit = glm::scale(orbit, glm::vec3(i*0.1f, i*0.1f, i*0.1f));
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(orbit));
			orbit_p.Draw(shader);
		}

		// Clear the colorbuffer

		//pohonkanan1(view4, window);
		//jalan2(tambah);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
		std::cout << "MAJU" << std::endl;
	}

	if (keys[GLFW_KEY_S])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		std::cout << "MUNDUR" << std::endl;
	}

	if (keys[GLFW_KEY_A])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
		std::cout << "KIRI" << std::endl;
	}

	if (keys[GLFW_KEY_D])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
		std::cout << "KANAN" << std::endl;
	}
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

GLuint loadTexture(GLchar* path, GLboolean alpha)
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes value from next repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}