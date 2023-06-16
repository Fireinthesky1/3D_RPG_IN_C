// Programmers: James Hicks
// Purpose: 3D OTGW RPG Game

//GLOBAL TODO
//  1)Leverage out shader functions into their own header and c files
//  2)General Cleanup

// Preprocessor
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>   // DELETE WHEN SWITCHING TO GLM

#include "u_util.h"

// Declarations

//=============================================================================
// TODO(JAMES): Fill out this struct and test functionality
struct shader 
{
    // Function Pointers
    const char* (*p_load_shader_source)(const char*);
    void(*p_bind_shader_source);
    int(*p_compile_shader);
    int(*p_link_shader);

    // Data members
    unsigned int shader_ID;

};
//=============================================================================

void
error_callback
(int error, const char* description);

void
framebuffer_size_callback(GLFWwindow* window, int width, int height);

void
key_callback
(GLFWwindow* window, int key, int scancode, int action, int mods);

void GLAPIENTRY
MessageCallback
(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
);

void
mouse_button_callback
(GLFWwindow* window, int button, int action, int mods);

static void
cursor_position_callback
(GLFWwindow* window, double xpos, double ypos);

// GLOBALS
int WIREFRAMEMODE = 0;

// Entry point
int main(int argc, char* argv[])
{
    // Initialize GLFW
    if (!glfwInit())
    {
        printf("ERROR::FAILED TO INITIALIZE GLFW\n");
        return -1;
    }
 
    // Window Variables
    int WIDTH = 640;
    int HEIGHT = 480;
 
    // Get a pointer to a GLFW window
    GLFWwindow* window;
 
    // Create a window and its associated context
    window = glfwCreateWindow(WIDTH, HEIGHT, "OTGW", NULL, NULL);
 
    if (window == NULL)
    {
        printf("ERROR::FAILED TO CREATE WINDOW");
        glfwTerminate();
        return -1;
    }
 
    // Make the window's context current
    glfwMakeContextCurrent(window);
 
    // Register Callback functions for GLFW
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    // Load OpenGL Functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("ERROR::FAILED TO INITIALIZE OPENGL CONTEXT");
        return -1;
    }
 
    // Enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
 
    // Print OpenGL version and Renderer
    const GLubyte* renderer = glGetString(GL_RENDERER);
    printf
    ("OpenGL %d.%d, RENDERER: %s\n",
        GLVersion.major, GLVersion.minor, renderer);
 
    // Vertex Shader (Remember to free shaders)
    const GLchar* vertexShaderSource = load_file("vertexShader.vert");
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint vertexCompiled;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
    if (vertexCompiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(vertexShader, 1024, &log_length, message);
        printf("ERROR::VERTEX SHADER COMPILATION FAILED:: %s", message);
        return -1;
    }
 
    // Fragment Shader
    const GLchar* fragmentShaderSource = load_file("fragmentShader.frag");
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    GLint fragmentCompiled;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompiled);
    if (fragmentCompiled != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(fragmentShader, 1024, &log_length, message);
        printf("ERROR::FRAGMENT SHADER COMPILATION FAILED:: %s", message);
        return -1;
    }
 
    // Create Shader Program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLuint programLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &programLinked);
    if (programLinked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024, &log_length, message);
        printf("ERROR::SHADER PROGRAM LINKING FAILED:: %s", message);
        return -1;
    }
 
    // Cleanup after Shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexShaderSource);
    free(fragmentShaderSource);
 
    // Vertex Data Array
    const GLfloat vertices[] =
    {
          // Vertices           //Colors
         -0.50f,  0.50f, 0.00f, 1.00f, 0.00f, 0.00f,    // top left::RED
         -0.50f, -0.50f, 0.00f, 0.00f, 1.00f, 0.00f,    // bottom left::GREEN
          0.50f, -0.50f, 0.00f, 0.00f, 0.00f, 1.00f,    // bottom right::BLUE
          0.50f,  0.50f, 0.00f, 0.85f, 0.45f, 0.30f     // top right::FUN COLOR
    };
 
    const GLuint indices[] =
    {
        0, 1, 2,    // First Triangle
        0, 2, 3     // Second Triangle
    };
 
    // Vertex Buffer Object & Vertex Array Object
    /*
    1) Create ID's for our VBO, VAO, EBO
    2) Bind our VAO
    3) Bind VBO and copy vertex data into Vertex Buffer
    4) Bind EBO and copy index data into Element Buffer
    5) Set Vertex Attribute Pointers
    6) Enable Vertex Attribute Array
    */
    GLuint vertexBufferObject, vertexArrayObject, elementBufferObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
 
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);
 
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW);
 
    // Position Attribute
    glVertexAttribPointer
    (0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    // Color Attribute
    glVertexAttribPointer
    (1, 3,  GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
 
    // Uniform Stuff DELETE====================================================
    // Uniform Stuff DELETE====================================================
 
    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Setup
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vertexArrayObject);
 
        // Uniform Stuff DELETE================================================
        // Uniform Stuff DELETE================================================
 
        // Draw Calls
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        // Poll for and process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
 
    // Cleanup
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);
    glfwTerminate();
    printf("CLEANUP SUCCESS");
    return 0;
}

// Function Definitions
// TODO(JAMES): FILL OUT CURSOR POSITION CALLBACK
static void
cursor_position_callback
(GLFWwindow* window, double xpos, double ypos)
{
}

void
error_callback
(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void
framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void
key_callback
(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        printf("W KEY PRESSED\n");
    }
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        printf("A KEY PRESSED\n");
    }
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        printf("S KEY PRESSED\n");
    }
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
        printf("D KEY PRESSED\n");
    }
    else if (key == GLFW_KEY_E && action == GLFW_RELEASE)
    {
        printf("E KEY PRESSED\n");
    }
    else if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    {
        printf("ESCAPE KEY PRESSED\n");
    }
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
    {
        printf("SHIFT KEY PRESSED\n");
    }
    else if (key == GLFW_KEY_0 && action == GLFW_RELEASE)
    {
        printf("TOGGLE::WIRE FRAME MODE\n");
        if (WIREFRAMEMODE)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            WIREFRAMEMODE = 0;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            WIREFRAMEMODE = 1;
        }
    }
}

void
mouse_button_callback
(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        printf("LEFT MOUSE BUTTON CLICKED\n");
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        printf("RIGHT MOUSE BUTTON CLICKED\n");
    }
}

void GLAPIENTRY
MessageCallback
(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
)
{
    printf("---------------------opengl-callback-start------------\n");
    printf("message: %s\n", message);
    printf("type: ");
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        printf("ERROR");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        printf("DEPRECATED_BEHAVIOR");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        printf("UNDEFINED_BEHAVIOR");
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        printf("PORTABILITY");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        printf("PERFORMANCE");
        break;
    case GL_DEBUG_TYPE_OTHER:
        printf("OTHER");
        break;
    }
    printf("\n");

    printf("id: 0x%x\n", id);
    printf("severity: ");
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        printf("LOW");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        printf("MEDIUM");
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        printf("HIGH");
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        printf("NOTIFICATION");
        break;
    }
    printf("\n");
    printf("---------------------opengl-callback-end--------------\n\n");
}