#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Shader.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        std::cout << "Failed to initalize GLEW" << std::endl;

    // vertex data that will be allocated to GL_ARRAY_BUFFER.
    // The layout as defined later is only 2 * sizeof(float) per vertice
    // containing only the coordinates for now
    float firstTriangle[] = {
        // first triangle
        -0.9f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // right
        -0.45f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // top
        // second triangle
         0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, // left
         0.9f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, // right
         0.45f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f // top
    };

    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5
    };

    /* Creating buffer for vertex array objects */
    unsigned int vbo, vao;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * 6 * sizeof(float), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    Shader myShader("res/shaders/Basic.vert", "res/shaders/Basic.frag");
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float greenShade = (float)((sin(glfwGetTime()) / 2.0f) + 0.5f);
        myShader.use();
        myShader.setFloat4("uniColor", 0.0f, greenShade, 0.0f, 1.0f);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();
    return 0;
}