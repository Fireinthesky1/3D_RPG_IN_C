// Programmers: James Hicks
// Purpose: 3D OTGW RPG Game

//GLOBAL TODO
//  1)Create glcall() to wrap every function call in to log error messages
//  2)Leverage out shader functions into their own header and c files
//  3)General Cleanup

// Preprocessor
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

// Declarations
void
error_callback
(int error, const char* description);

void
framebuffer_size_callback(GLFWwindow* window, int width, int height);

void
key_callback
(GLFWwindow* window, int key, int scancode, int action, int mods);

const GLchar* load_Shader(const GLchar* filename);

void
mouse_button_callback
(GLFWwindow* window, int button, int action, int mods);

static void
cursor_position_callback
(GLFWwindow* window, double xpos, double ypos);

// Entry point
int main(int argc, char *argv[])
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

    // Print OpenGL version and Renderer
    const GLubyte* renderer = glGetString(GL_RENDERER);
    printf
    ("OpenGL %d.%d, RENDERER: %s\n",
        GLVersion.major, GLVersion.minor, renderer);

    // Vertex Shader (Remember to free shaders)
    // TODO(JAMES): Figure out how to pass relative path for shaders
    const GLchar* vertexShaderSource = load_Shader
    ("C:/Users/james/OneDrive/Desktop/OverTheGardenWall/vs/otgw/otgw/vertexShader.vert");
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
    const GLchar* fragmentShaderSource = load_Shader
    ("C:/Users/james/OneDrive/Desktop/OverTheGardenWall/vs/otgw/otgw/fragmentShader.frag");
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
         -0.50f,  0.00f,  0.00f, // left
          0.00f,  0.50f,  0.00f, // top
          0.50f,  0.00f,  0.00f  // right
    };

    // Vertex Buffer Object & Vertex Array Object
    GLuint vertexBufferObject, vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);
    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);

    glVertexAttribPointer
    (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);   //TODO(JAMES): LEARN THIS



    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Setup
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vertexArrayObject);

        // Draw Calls
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Poll for and process events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // Cleanup
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
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
}

// TODO(JAMES): BULLETPROOF: Stops loading characters after first '}'
const GLchar* load_Shader(const GLchar* filename)
{
    FILE* f;
    fopen_s(&f, filename, "r");

    if (f == NULL)
    {
        perror("ERROR::FAILED TO OPEN SHADER:: ");
        return NULL;
    }
 
    fseek(f, 0, SEEK_END);
    unsigned long length = (unsigned long)ftell(f);
    rewind(f);
    GLchar* shader = malloc(length);
 
    if (shader == NULL) 
    {
        printf("FAILED TO ALLOCATE MEMORY FOR SHADER");
        return NULL;
    }

    for (GLuint i = 0; i < length; ++i)
    {
        shader[i] = '\0';
    }

    for (GLuint i = 0; i < length; ++i)
    {
        GLchar c = getc(f);
        if (c == '}') { shader[i] = c; break; }
        shader[i] = c;
        //putc(shader[i], stdout); //TESTING CODE
    }
 
    fclose(f);
    return shader;
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