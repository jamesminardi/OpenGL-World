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

Camera camera(glm::vec3(67.0f, 628.0f, 170.0f));



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

    glEnable(GL_DEPTH_TEST);

    Shader shader("../../src/shaders/cpu_terrain.vert", "../../src/shaders/cpu_terrain.frag");

    int width, height, num_channels;
    uint8_t *data = stbi_load("../../resources/textures/iceland_heightmap.png", &width, &height, &num_channels, 0);
    if (data)
    {
        std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Generate mesh vertices based on height map
    std::vector<float> vertices;
    float y_scale = 64.0f / 256.0f;
    float y_shift = 16.0f; // Apply a scale and shift to the height data
    uint32_t bytes_per_pixel = num_channels;
    int rez = 1;

    for( uint32_t i =  0; i < height; i++)
    {
        for (uint32_t j = 0; j < width; j++)
        {
            // Retrieve texel for (i,j) texture coord
            uint8_t* pixel_offset = data + (width * i + j) * num_channels;

            // Raw height at coordinate
            uint8_t y = pixel_offset[0];

            // Vertex
            vertices.push_back( -height/2.0f + height*i/(float)height );       // v.x
            vertices.push_back( (int)y * y_scale - y_shift);// v.y
            vertices.push_back( -width / 2.0f + width*j/(float)width );       // v.z
        }
    }
    std::cout << "Loaded " << vertices.size() / 3 << " vertices" << std::endl;
    stbi_image_free(data);

    // index generation
    std::vector<unsigned int> indices;
    for(uint32_t i = 0; i < height-1; i+=rez)       // for each row a.k.a. each strip
    {
        for(uint32_t j = 0; j < width; j+=rez)      // for each column
        {
            for(uint32_t k = 0; k < 2; k++)      // for each side of the strip
            {
                indices.push_back(j + width * (i + k*rez));
            }
        }
    }
    std::cout << "Loaded " << indices.size() << " indices" << std::endl;

    const uint32_t num_strips = (height - 1)/rez;
    const uint32_t num_tris_per_strip = (width/rez)*2 - 2;
    std::cout << "Created lattice of " << num_strips << " strips with " << num_tris_per_strip - 2 << " triangles each" << std::endl;
    std::cout << "Created " << num_strips * num_tris_per_strip << " triangles total" << std::endl;

    // Set up VAO
    uint32_t terrain_vao;
    uint32_t terrain_vbo;
    uint32_t terrain_ebo;
    uint32_t pos_attrib = 0;

    glCreateVertexArrays(1, &terrain_vao);
    glCreateBuffers(1, &terrain_vbo);
    glCreateBuffers(1, &terrain_ebo);

    glNamedBufferData(terrain_vbo, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    glNamedBufferData(terrain_ebo, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);


    glEnableVertexArrayAttrib(terrain_vao, pos_attrib);
    glVertexArrayAttribFormat(terrain_vao, pos_attrib, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(terrain_vao, pos_attrib, 0);
    glVertexArrayVertexBuffer(terrain_vao, 0, terrain_vbo, 0, 3 * sizeof(float));

    glVertexArrayElementBuffer(terrain_vao, terrain_ebo);

    shader.use();

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

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

//        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 mvp = projection * view * model;

        shader.setMat4("mvp", mvp);

        glBindVertexArray(terrain_vao);
        for(unsigned int strip = 0; strip < num_strips; ++strip)
        {
            glDrawElements(GL_TRIANGLE_STRIP, num_vertices_per_strip, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * num_vertices_per_strip * strip));
        }

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