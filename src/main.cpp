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

    float texture_coords[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,

            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f
    };

    Shader shader("../../src/shaders/phong.vert", "../../src/shaders/phong.frag");
    Shader light_shader("../../src/shaders/light_caster.vert", "../../src/shaders/light_caster.frag");



    uint32_t pos_attrib = 0;
    uint32_t color_attrib = 1;
    uint32_t texture_attrib = 2;
    uint32_t normal_attrib = 3;


    // 1st, bind the vao.
    uint32_t cube_vao;
    glCreateVertexArrays(1, &cube_vao);

    uint32_t light_vao;
    glCreateVertexArrays(1, &light_vao);

    uint32_t pos_vbo;
    glCreateBuffers(1, &pos_vbo);
    glNamedBufferData(pos_vbo, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    uint32_t color_vbo;
    glCreateBuffers(1, &color_vbo);
    glNamedBufferData(color_vbo, sizeof(cube_color), cube_color, GL_STATIC_DRAW);

    uint32_t texture_vbo;
    glCreateBuffers(1, &texture_vbo);
    glNamedBufferData(texture_vbo, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);

    uint32_t normal_vbo;
    glCreateBuffers(1, &normal_vbo);
    glNamedBufferData(normal_vbo, sizeof(cube_normal), cube_normal, GL_STATIC_DRAW);


    // Enable attributes for cube_vao
    glEnableVertexArrayAttrib(cube_vao, pos_attrib);
    glEnableVertexArrayAttrib(cube_vao, color_attrib);
    glEnableVertexArrayAttrib(cube_vao, texture_attrib);
    glEnableVertexArrayAttrib(cube_vao, normal_attrib);

    // Set up formats for cube_vao attributes
    glVertexArrayAttribFormat(cube_vao, pos_attrib, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(cube_vao, color_attrib, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(cube_vao, texture_attrib, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(cube_vao, normal_attrib, 3, GL_FLOAT, GL_FALSE, 0);

    // Make attributes use binding 0
    glVertexArrayAttribBinding(cube_vao, pos_attrib, 0);
    glVertexArrayAttribBinding(cube_vao, color_attrib, 1);
    glVertexArrayAttribBinding(cube_vao, texture_attrib, 2);
    glVertexArrayAttribBinding(cube_vao, normal_attrib, 3);

    glVertexArrayVertexBuffer(cube_vao, 0, pos_vbo, 0, 3 * sizeof(float));
    glVertexArrayVertexBuffer(cube_vao, 1, color_vbo, 0, 3 * sizeof(float));
    glVertexArrayVertexBuffer(cube_vao, 2, texture_vbo, 0, 2 * sizeof(float));
    glVertexArrayVertexBuffer(cube_vao, 3, normal_vbo, 0, 3 * sizeof(float));


    // Light VAO and attributes setup
    glEnableVertexArrayAttrib(light_vao, pos_attrib);
    glVertexArrayAttribFormat(light_vao, pos_attrib, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(light_vao, pos_attrib, 0);
    glVertexArrayVertexBuffer(light_vao, 0, pos_vbo, 0, 3 * sizeof(float));


    glEnable(GL_DEPTH_TEST);

    shader.use();


    uint32_t texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    float border_color[] = { 1.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    int width, height, nrChannels;
    uint8_t *data = stbi_load("../../resources/textures/brickwall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTextureStorage2D(texture, 1, GL_RGB8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(texture);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

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

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

//        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 mvp = projection * view * model;

        shader.setVec3("LightColor", light_color);
        shader.setVec3("LightPos", light_position);
        shader.setVec3("ViewPos", camera.Position);
        shader.setFloat("SpecularStrength", cube_specular_strength);
        shader.setFloat("AmbientStrength", cube_ambient_strength);
        shader.setFloat("Shininess", cube_shininess);
        shader.setFloat("DiffuseStrength", cube_diffuse_strength);
        shader.setMat4("MVPMatrix", mvp);
        shader.setMat4("ModelMatrix", model);

        glBindTextureUnit(0, texture);
        glBindVertexArray(cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

        // LIGHT

        light_shader.use();

        glm::mat4 light_model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 light_view          = glm::mat4(1.0f);
        glm::mat4 light_projection    = glm::mat4(1.0f);
        light_model = glm::scale(light_model, glm::vec3(0.2f));
        light_model = glm::translate(light_model, light_position);
        light_view = camera.GetViewMatrix();
        light_projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 light_mvp = light_projection * light_view * light_model;

        light_shader.setMat4("mvp", light_mvp);
        light_shader.setVec3("lightColor", light_color);

        glBindVertexArray(light_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

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