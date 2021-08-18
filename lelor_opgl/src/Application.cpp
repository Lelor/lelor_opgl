#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>



std::string GetShaderCode(std::string path)
{
    std::ifstream shaderCode(path);
    std::stringstream strBuffer;
    strBuffer << shaderCode.rdbuf();
    return strBuffer.str();
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    /* Get GLSL shader raw codeand load it into OpenGL compiler.
       source can be any of the GLenum shader types */
    unsigned int id = glCreateShader(type);
    const char* src = &source[0];
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "FAILED TO COMPILE SHADER" << std::endl;
        std::cout << message << std::endl;
        return 0;
    }

    return id;
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}


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

    //unsigned int redShader = CreateShader(GetShaderCode("res/shaders/Basic.vert"), GetShaderCode("res/shaders/Red.frag"));
    //unsigned int blueShader = CreateShader(GetShaderCode("res/shaders/Basic.vert"), GetShaderCode("res/shaders/Blue.frag"));
    unsigned int genericShader = CreateShader(GetShaderCode("res/shaders/Basic.vert"), GetShaderCode("res/shaders/Basic.frag"));
    glUseProgram(genericShader);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
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
    glDeleteProgram(genericShader);
    glfwTerminate();
    return 0;
}