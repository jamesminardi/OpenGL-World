#include <GL/glew.h> // before glfw
#include <GLFW/glfw3.h>


#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stb_image.h>

#include "triangle.h"
#include "shader.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void APIENTRY message_callback(   GLenum source,
                                  GLenum type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei length,
                                  const GLchar* message,
                                  const void* userParam );

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));

glm::vec3 light_position(-3.0f, 1.5f, 3.0f);
glm::vec3 light_color(1.0f, 1.0f, 1.0f);
// Cube only uniforms
float cube_specular_strength = 1.0f;
float cube_ambient_strength = 0.6f;
float cube_diffuse_strength = 0.6f;
float cube_shininess = 32.0f;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glew: load all OpenGL function pointers
    // ---------------------------------------
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    std::cout << "Initialized GLEW" << std::endl;

    int32_t flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        std::cout << "Enabling Debug Output" << std::endl;
        // Initialize debug output
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Callback immediately
        glDebugMessageCallback(message_callback, nullptr);

        // Filter all messaged to be callback
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    }

    Shader shader("../../src/shaders/passthrough.vert", "../../src/shaders/passthrough.frag");
    std::cout << "Loaded Shaders" << std::endl;

    uint32_t pos_attrib = 0;
    uint32_t color_attrib = 1;


    // 1st, bind the vao.
    uint32_t vao;
    glCreateVertexArrays(1, &vao);

    uint32_t ibo;
    glCreateBuffers(1, &ibo);
    glNamedBufferData(ibo, sizeof(triangle_indices), triangle_indices, GL_STATIC_DRAW);



    uint32_t pos_vbo;
    glCreateBuffers(1, &pos_vbo);
    glNamedBufferData(pos_vbo, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    uint32_t color_vbo;
    glCreateBuffers(1, &color_vbo);
    glNamedBufferData(color_vbo, sizeof(triangle_color), triangle_color, GL_STATIC_DRAW);

    glVertexArrayVertexBuffer(vao, 0, pos_vbo, 0, 3 * sizeof(float));
    glVertexArrayVertexBuffer(vao, 1, color_vbo, 0, 3 * sizeof(float));


    // Enable attributes for cube_vao
    glEnableVertexArrayAttrib(vao, pos_attrib);
    glEnableVertexArrayAttrib(vao, color_attrib);

    // Set up formats for cube_vao attributes
    glVertexArrayAttribFormat(vao, pos_attrib, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(vao, color_attrib, 3, GL_FLOAT, GL_FALSE, 0);

    // Make attributes use binding 0
    glVertexArrayAttribBinding(vao, pos_attrib, 0);
    glVertexArrayAttribBinding(vao, color_attrib, 1);

    glVertexArrayElementBuffer(vao, ibo);

    glEnable(GL_DEPTH_TEST);


//    stbi_image_free(data);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glBindVertexArray(vao);
        std::cout << "Drawing Elements" << std::endl;
        glDrawElements(GL_TRIANGLES, sizeof(triangle_indices)/sizeof(triangle_indices[0]), GL_UNSIGNED_INT, 0);
//        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        light_position.y += 0.25f;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        light_position.y -= 0.25f;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        light_position.x -= 0.25f;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        light_position.x += 0.25f;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        light_position.z += 0.25f;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        light_position.z -= 0.25f;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        cube_ambient_strength = std::min(1.0f, cube_ambient_strength + 0.1f);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        cube_ambient_strength = std::max(0.0f, cube_ambient_strength - 0.1f);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        cube_diffuse_strength = std::min(1.0f, cube_diffuse_strength + 0.1f);
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        cube_diffuse_strength = std::max(0.0f, cube_diffuse_strength - 0.1f);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void APIENTRY message_callback(  GLenum source,
                                 GLenum type,
                                 GLuint id,
                                 GLenum severity,
                                 GLsizei length,
                                 const GLchar* message,
                                 const void* userParam )
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        default: break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        default: break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        default: break;
    } std::cout << std::endl;
    std::cout << std::endl;

    if (type == GL_DEBUG_TYPE_ERROR)
    {
        system("pause");
    }
}