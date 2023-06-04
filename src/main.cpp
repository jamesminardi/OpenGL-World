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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int modifiers);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
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
const unsigned int NUM_PATCH_PTS = 4;
int use_wireframe = 0;
int display_grayscale = 0;

Camera camera(glm::vec3(0.0f, 5.0f, 0.0f));

float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

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
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    Shader shader("../../src/shaders/gpu_terrain.vert", "../../src/shaders/gpu_terrain.frag");
//    Shader shader("../../src/shaders/gpu_terrain.vert", "../../src/shaders/gpu_terrain.frag",
//                  nullptr, "../../src/shaders/gpu_terrain.tesc", "../../src/shaders/gpu_terrain.tese");
    shader.use();
    // Load and create texture heightmap
//    uint32_t texture;
//    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
//    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, num_channels;
//    uint8_t *data = stbi_load("../../resources/textures/iceland_heightmap.png", &width, &height, &num_channels, 0);
//    if (data)
//    {
//        std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
////        glTextureStorage2D(texture, 1, GL_RGB, width, height);
////        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
////        glBindTexture(GL_TEXTURE_2D, texture);
////        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//        std::cout << "setting subimage " << std::endl;
//
////        glGenerateTextureMipmap(texture);
////        shader.setInt("heightMap", 0);
//    }
//    else
//    {
//        std::cout << "Failed to load texture" << std::endl;
//    }
//    stbi_image_free(data);

//    int max_tess_level;
//    glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &max_tess_level);
//    std::cout << "Max available tess level: " << max_tess_level << std::endl;


    // Generate mesh vertices based on height map
    std::vector<float> vertices;
    float y_scale = 64.0f / 256.0f;
    float y_shift = 0.0f; // Apply a scale and shift to the height data (16.0)
    uint32_t bytes_per_pixel = num_channels;

    width = 4;
    height = 4;
    //vertices.resize(width*height);


    uint32_t rez = 1;
    for(uint32_t i = 0; i <= height; i++)
    {
        for(uint32_t j = 0; j <= width; j++)
        {
            vertices.push_back(j);
			vertices.push_back(0.0f);
			vertices.push_back(i);
		}
	}
    std::cout << "Loaded " << vertices.size() / 3 << " vertices, expected " << (width+1) * (height+1) << std::endl;
//    std::cout << "Loaded " << rez*rez << " patches of 4 control points each" << std::endl;
//    std::cout << "Processing " << rez*rez*4 << " vertices in vertex shader" << std::endl;

//	index generation
    std::vector<unsigned int> indices;
    for(uint32_t i = 0; i < height; i++)		// for each row
    {
        for(uint32_t j = 0; j < width; j++)	// for each column
        {
			// Starting vertex for the current quad
			uint32_t tri_start_vertex = ((width+1)*i) + j;			//  1		.
			indices.push_back(tri_start_vertex);					// | \		.
			indices.push_back(tri_start_vertex + (width+1) );		// |  \		.
			indices.push_back(tri_start_vertex + (width+1) + 1);	// 2---3	.

																	// 1---2
			indices.push_back(tri_start_vertex);					//  \  |
			indices.push_back(tri_start_vertex + 1);				//   \ |
			indices.push_back(tri_start_vertex + (width+1) + 1);	//    3
        }
    }
    std::cout << "Loaded " << indices.size() << " indices, expected " << width * height * 2 * 3 << std::endl;

//    const uint32_t num_strips = (height - 1)/rez;
//    const uint32_t num_tris_per_strip = (width/rez)*2 - 2;
//    std::cout << "Created lattice of " << num_strips << " strips with " << num_tris_per_strip - 2 << " triangles each" << std::endl;
//    std::cout << "Created " << num_strips * num_tris_per_strip << " triangles total" << std::endl;

    // Set up VAO
    uint32_t terrain_vao;
    uint32_t terrain_vbo;
//    uint32_t terrain_ebo;
    uint32_t pos_attrib = 0;

    glCreateVertexArrays(1, &terrain_vao);
    glCreateBuffers(1, &terrain_vbo);
//    glCreateBuffers(1, &terrain_ebo);
//
//    glNamedBufferData(terrain_vbo, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
//    glNamedBufferData(terrain_ebo, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);
//
//
//    glEnableVertexArrayAttrib(terrain_vao, pos_attrib);
//    glVertexArrayAttribFormat(terrain_vao, pos_attrib, 3, GL_FLOAT, GL_FALSE, 0);
//    glVertexArrayAttribBinding(terrain_vao, pos_attrib, 0);
//    glVertexArrayVertexBuffer(terrain_vao, 0, terrain_vbo, 0, 3 * sizeof(float));
//
//    glVertexArrayElementBuffer(terrain_vao, terrain_ebo);
//    glPatchParameteri(GL_PATCH_VERTICES, 4);



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
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);

        glm::mat4 mvp = projection * view * model;

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBindVertexArray(terrain_vao);
        glDrawArrays(GL_PATCHES, 0, NUM_PATCH_PTS*rez*rez);
//        for(unsigned int strip = 0; strip < num_strips; strip++)
//        {
//            glDrawElements(GL_TRIANGLE_STRIP, num_tris_per_strip+2, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * (num_tris_per_strip+2) * strip));
//        }

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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);

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
//        system("pause");
    }
}

// glfw: whenever a key event occurs, this callback is called
// ---------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int modifiers)
{
    if(action == GLFW_PRESS)
    {
        switch(key)
        {
            case GLFW_KEY_SPACE:
                use_wireframe = 1 - use_wireframe;
                if (use_wireframe) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                } else {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
                }
                break;
            case GLFW_KEY_G:
                display_grayscale = 1 - display_grayscale;
                break;
            case GLFW_KEY_P:
                std::cout << "Position: " << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << std::endl;
            default:
                break;
        }
    }
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float x_offset = xpos - last_x;
    float y_offset = last_y - ypos; // reversed since y-coordinates go from bottom to top

    last_x = xpos;
    last_y = ypos;

    camera.ProcessMouseMovement(x_offset, y_offset);
}