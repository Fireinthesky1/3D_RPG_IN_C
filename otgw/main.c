// Programmers: James Hicks
// Purpose: 3D OTGW RPG Game

// Preprocessor
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// Declarations
void
error_callback
(int error, const char* description);

void
key_callback
(GLFWwindow* window, int key, int scancode, int action, int mods);

void
mouse_button_callback
(GLFWwindow* window, int button, int action, int mods);

static void
cursor_position_callback
(GLFWwindow* window, double xpos, double ypos);

// Entry point
int main()
{
    // Window Variables
    int WIDTH = 640;
    int HEIGHT = 480;

    // Get a pointer to a GLFW window
    GLFWwindow* window;

    // Initialize GLFW
    if (!glfwInit())
    {
        printf("ERROR::FAILED TO INITIALIZE GLFW\n");
        return -1;
    }

    // Create a window and its associated context
    window = glfwCreateWindow(WIDTH, HEIGHT, "OTGW", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Register Callback functions for GLFW
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);


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

    // Vertex Data Array
    const GLfloat vertices[3][3] =
    {
        { -0.50,  0.00,  0.00}, // left
        {  0.00,  0.50,  0.00}, // top
        {  0.50,  0.00,  0.00}  // right
    };

    // Vertex Buffer Object
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(
        vertexBufferObject,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW);

    // Vertex Array Object
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    glEnableVertexAttribArray(0); // TODO(JAMES: Will this always need to be 0?
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Render Here
        glClearColor(.4,.1,.7,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap Buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }


    // Cleanup
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