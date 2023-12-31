// Programmers: James Hicks
// Purpose: 3D OTGW RPG Game

//GLOBAL TODO
//  1)General Cleanup
//  2)Leverage texture loading out
//  3)Leverage vertex buffer object out
//  4)Create function to load png images (Get rid of stb_image.h)
//  5)Switch everything to SDL
//  6)Add hot reload

// Preprocessor
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "u_util.h"
#include "r_shader.h"
#include "r_texture.h"

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
    ("OpenGL %d.%d, RENDERER: %s\n\n",
        GLVersion.major, GLVersion.minor, renderer);

    struct shader vertex_shader =
    {
        u_load_file("vertexShader.vert"),
        GL_VERTEX_SHADER,
        glCreateShader(GL_VERTEX_SHADER)
    };
    s_bind_shader_source(&vertex_shader);
    s_compile_shader(&vertex_shader);

    struct shader fragment_shader =
    {
        u_load_file("fragmentShader.frag"),
        GL_FRAGMENT_SHADER,
        glCreateShader(GL_FRAGMENT_SHADER)
    };
    s_bind_shader_source(&fragment_shader);
    s_compile_shader(&vertex_shader);

    // Create Shader Program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader.id);
    glAttachShader(program, fragment_shader.id);
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

    /* TEXTURES
    1) Create a texture object and load texel data into it
    2) Include texture coordinates in vertex data
    3) Associate a texture sampler with each texture map used in shader
    4) Retrieve the texel values through the texture sampler from shader
    */

    // GET RID OF MESSY INITIALIZATION=============================================
    //    GLint t_width, t_height, t_channels;
    //    GLuint id;
    //    struct texture my_texture =
    //    {
    //        stbi_load("wall.jpg", &t_width, &t_height, &t_channels,0),
    //        t_width,
    //        t_height,
    //        GL_TEXTURE_2D,
    //        &id
    //    };
    //    t_initialize_id(&my_texture);
    //    t_bind_texture(&my_texture);
    //    t_set_tex_param(&my_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //    t_set_tex_param(&my_texture,
    //                    GL_TEXTURE_MAG_FILTER,
    //                    GL_LINEAR_MIPMAP_LINEAR);
    //    t_set_tex_param(&my_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //    t_set_tex_param(&my_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //    if(t_specify_image(&my_texture) == -1)
    //    {
    //        printf("ERROR::FAILED TO LOAD TEXTURE");
    //        return - 1;
    //    }
    //    t_gen_mip_map(&my_texture);
    // GET RID OF MESSY INITIALIZATION=============================================

        // Vertex Data Array
        //const GLfloat vertices[] =
        //{
        //      //Vertices             //Color                //Texture Coordinates
        //     -0.50f,  0.50f, 0.00f,   1.00f, 0.00f, 0.00f,   0.00f, 1.00f, //TL
        //     -0.50f, -0.50f, 0.00f,   0.00f, 1.00f, 0.00f,   0.00f, 0.00f, //BL
        //      0.50f, -0.50f, 0.00f,   0.00f, 0.00f, 1.00f,   1.00f, 0.00f, //BR
        //      0.50f,  0.50f, 0.00f,   0.85f, 0.45f, 0.30f,   1.00f, 1.00f  //TR
        //};

    const GLfloat vertices[] =
    {
        //vertices
        -0.50f,  0.50f, 0.00f, //FTL
         0.50f,  0.50f, 0.00f, //FBL
        -0.50f, -0.50f, 0.00f, //FBR
         0.50f, -0.50f, 0.00f, //FTR
        -0.50f,  0.50f, 1.00f, //BTL
         0.50f,  0.50f, 1.00f, //BBL
        -0.50f, -0.50f, 1.00f, //BBR
         0.50f, -0.50f, 1.00f  //BTR
    };

    //const GLuint indices[] =
    //{
    //    0, 1, 2,    // First Triangle
    //    0, 2, 3     // Second Triangle
    //};

    const GLuint indices[] =
    {
        0,2,3,   0,1,3, //FRONT
        4,6,7,   4,5,7, //BACK
        0,2,6,   0,4,6, //LEFT
        1,3,7,   1,5,7, //RIGHT
        0,4,1,   1,4,5, //TOP
        2,6,3,   3,6,7  //BOTTOM
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
    (0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    //    // Color Attribute
    //    glVertexAttribPointer
    //    (1, 3,  GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(3*sizeof(float)));
    //    glEnableVertexAttribArray(1);
    //
    //    // Texture Attribute
    //    glVertexAttribPointer
    //    (2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6*sizeof(float)));
    //    glEnableVertexAttribArray(2);

        // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Setup
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);

        //Textures and VAO
        //t_bind_texture(&my_texture);
//UNIFORM DELETE===============================================================
        float time = glfwGetTime();
        mat3 matrix_rotate_z =
        {
            cos(time),  sin(time), 0,
           -sin(time),  cos(time), 0,
            0,          0,         1
        };
        mat3 matrix_rotate_x =
        {
            1,  0,         0,
            0,  cos(time), sin(time),
            0, -sin(time), cos(time)
        };

        mat3 uniform_matrix;
        glm_mat3_mul(matrix_rotate_x, matrix_rotate_z, uniform_matrix);
        GLint location = glGetUniformLocation(program, "u_rotation_matrix");
        //UNIFORM DELETE===============================================================
        glUniformMatrix3fv(location, 1, GL_FALSE, &uniform_matrix);
        glBindVertexArray(vertexArrayObject);

        glUseProgram(program);

        // Draw Calls
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Poll for and process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);
    glDeleteShader(vertex_shader.id);
    glDeleteShader(fragment_shader.id);
    //    glDeleteTextures(1, &(my_texture.id));
    //    stbi_image_free(my_texture.image_data);
    free(vertex_shader.source_code);
    free(fragment_shader.source_code);
    glfwTerminate();
    printf("CLEANUP SUCCESS\n");
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