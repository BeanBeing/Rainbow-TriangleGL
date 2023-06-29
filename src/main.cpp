#include "../glad/include/glad.h"
#include <GLFW/glfw3.h>

#include "../headers/shader.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
   // Initializes GLFW and makes sure user's OpenGL version is 3.3+
   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   // If user uses APPLE then this will run
#ifdef __APPLE__
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

   GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
   if(window == nullptr)
   {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
   }
   glfwMakeContextCurrent(window);
   glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

   if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
   }

   /**----------------- SHADERS -----------------**/

   // Reads vertex attributes we're allowed to declare limited by hardware
   int nrAttributes{};
   glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
   std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

   Shader ourShader("shaders/rainbowShader.vs", "shaders/rainbowShader.fs");

   /**----------------- Renderer -----------------**/
   float first_triangle_vertices[] = {
      //	Positions		 // Color
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,0.0f,		// bottom left
      0.5f,  -0.5f, 0.0f, 0.0f,1.0f,0.0f,		// bottom right
      0.0f,  0.5f, 0.0f,0.0f,0.0f, 1.0f		// top middle
   };
   


   
   // VBO(Vertex Buffer Object) and VAO(Vertex Array Object)
   unsigned int VBO{}, VAO{};
   // Binds VAO and VBO
   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);

   // First Triangle bind Vertex Array Object
   glBindVertexArray(VAO);

   // First Triangle bind Vertex Buffer Object
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(first_triangle_vertices), first_triangle_vertices, GL_STATIC_DRAW);

   // Position Attributes
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(0));
   glEnableVertexAttribArray(0);

   // Color Attributes
   // Attribute location is 1, color value has size of 3 floats,
   // move 6 floats(24 bytes) to right(3 for pos, 3 for color).
   // Color attribute starts after position data so offset is (3 * sizeof(float)) therefore (12 bytes)
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);

   // Wireframe Mode
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   while(!glfwWindowShouldClose(window))
   {
      processInput(window);

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      /**----------------- RENDERER TRIANGLES -----------------**/
      ourShader.use();
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      /**----------------- SWAP BUFFERS AND POLL IO EVENTS -----------------**/
      glfwSwapBuffers(window);
      glfwPollEvents();
   }

   // Optional -- Frees resources
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   
   // Terminates GLFW and de-allocates the resources
   glfwTerminate();
   return 0;
}

   void processInput(GLFWwindow* window)
   {
      if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      {
         glfwSetWindowShouldClose(window, true);
      }
   }

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   glViewport(0, 0, width, height);
}
