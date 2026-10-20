// Lab5-2.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// ======================================================
// อ่านไฟล์ Shader
// ======================================================
std::string readShaderFile(const char* filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cout << "ERROR: Cannot open shader file: "
            << filePath << std::endl;

        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();

    return buffer.str();
}


// ======================================================
// สร้าง Shader Program
// ======================================================
unsigned int createShaderProgram(
    const char* vertexPath,
    const char* fragmentPath)
{
    // อ่าน Vertex Shader
    std::string vertexCode = readShaderFile(vertexPath);

    // อ่าน Fragment Shader
    std::string fragmentCode = readShaderFile(fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty())
    {
        return 0;
    }

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();


    // ==================================================
    // Vertex Shader
    // ==================================================
    unsigned int vertexShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexSource,
        NULL
    );

    glCompileShader(vertexShader);


    // ตรวจสอบ Vertex Shader
    int success;
    char infoLog[512];

    glGetShaderiv(
        vertexShader,
        GL_COMPILE_STATUS,
        &success
    );

    if (!success)
    {
        glGetShaderInfoLog(
            vertexShader,
            512,
            NULL,
            infoLog
        );

        std::cout << "ERROR::VERTEX SHADER\n"
            << infoLog << std::endl;
    }


    // ==================================================
    // Fragment Shader
    // ==================================================
    unsigned int fragmentShader;

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentSource,
        NULL
    );

    glCompileShader(fragmentShader);


    // ตรวจสอบ Fragment Shader
    glGetShaderiv(
        fragmentShader,
        GL_COMPILE_STATUS,
        &success
    );

    if (!success)
    {
        glGetShaderInfoLog(
            fragmentShader,
            512,
            NULL,
            infoLog
        );

        std::cout << "ERROR::FRAGMENT SHADER\n"
            << infoLog << std::endl;
    }


    // ==================================================
    // Shader Program
    // ==================================================
    unsigned int shaderProgram;

    shaderProgram = glCreateProgram();

    glAttachShader(
        shaderProgram,
        vertexShader
    );

    glAttachShader(
        shaderProgram,
        fragmentShader
    );

    glLinkProgram(shaderProgram);


    // ตรวจสอบ Program
    glGetProgramiv(
        shaderProgram,
        GL_LINK_STATUS,
        &success
    );

    if (!success)
    {
        glGetProgramInfoLog(
            shaderProgram,
            512,
            NULL,
            infoLog
        );

        std::cout << "ERROR::SHADER PROGRAM\n"
            << infoLog << std::endl;
    }


    // ลบ Shader ที่ไม่ใช้แล้ว
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    return shaderProgram;
}


// ======================================================
// MAIN
// ======================================================
int main()
{
    // ==================================================
    // GLFW initialize
    // ==================================================
    glfwInit();

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        3
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3
    );

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );

#ifdef __APPLE__
    glfwWindowHint(
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE
    );
#endif


    // ==================================================
    // Create Window
    // ==================================================
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "Lab5-2 Texture",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout
            << "Failed to create GLFW window"
            << std::endl;

        glfwTerminate();

        return -1;
    }


    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(
        window,
        framebuffer_size_callback
    );


    // ==================================================
    // GLAD
    // ==================================================
    if (!gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress))
    {
        std::cout
            << "Failed to initialize GLAD"
            << std::endl;

        return -1;
    }


    // ==================================================
    // Create Shader Program
    // ==================================================
    unsigned int shaderProgram =
        createShaderProgram(
            "vShader.glsl",
            "fShader.glsl"
        );

    if (shaderProgram == 0)
    {
        std::cout
            << "Failed to create shader program"
            << std::endl;

        glfwTerminate();

        return -1;
    }


    // ==================================================
    // Vertex Data
    // ==================================================
    float vertices[] =
    {
        // positions          // colors           // texture coords

         0.5f,  0.5f, 0.0f,
         1.0f, 0.0f, 0.0f,
         1.0f, 1.0f,

         0.5f, -0.5f, 0.0f,
         0.0f, 1.0f, 0.0f,
         1.0f, 0.0f,

        -0.5f, -0.5f, 0.0f,
         0.0f, 0.0f, 1.0f,
         0.0f, 0.0f,

        -0.5f,  0.5f, 0.0f,
         1.0f, 1.0f, 0.0f,
         0.0f, 1.0f
    };


    // ==================================================
    // Indices
    // ==================================================
    unsigned int indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };


    // ==================================================
    // VAO / VBO / EBO
    // ==================================================
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // Bind VAO
    glBindVertexArray(VAO);


    // ==================================================
    // VBO
    // ==================================================
    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );


    // ==================================================
    // EBO
    // ==================================================
    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        EBO
    );

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );


    // ==================================================
    // Position Attribute
    // location = 0
    // ==================================================
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // ==================================================
    // Color Attribute
    // location = 1
    // ==================================================
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);


    // ==================================================
    // Texture Coordinate Attribute
    // location = 2
    // ==================================================
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    );

    glEnableVertexAttribArray(2);


    // ==================================================
    // Texture
    // ==================================================
    unsigned int texture;

    glGenTextures(
        1,
        &texture
    );

    glBindTexture(
        GL_TEXTURE_2D,
        texture
    );


    // Texture Wrapping
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );


    // Texture Filtering
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );


    // ==================================================
    // Load Texturelabs_Atmosphere_128S.jpg
    // ==================================================
    int width;
    int height;
    int nrChannels;

    unsigned char* data = stbi_load(
        "Texturelabs_Atmosphere_128S.jpg",
        &width,
        &height,
        &nrChannels,
        0
    );


    if (data)
    {
        GLenum format;

        if (nrChannels == 1)
        {
            format = GL_RED;
        }
        else if (nrChannels == 3)
        {
            format = GL_RGB;
        }
        else if (nrChannels == 4)
        {
            format = GL_RGBA;
        }
        else
        {
            format = GL_RGB;
        }


        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );

        glGenerateMipmap(
            GL_TEXTURE_2D
        );


        std::cout
            << "Texture loaded successfully!"
            << std::endl;

        std::cout
            << "Width: "
            << width
            << std::endl;

        std::cout
            << "Height: "
            << height
            << std::endl;
    }
    else
    {
        std::cout
            << "ERROR: Failed to load "
            << "Texturelabs_Atmosphere_128S.jpg"
            << std::endl;

        std::cout
            << "Make sure the image is in the correct folder."
            << std::endl;
    }


    // Free image memory
    stbi_image_free(data);


    // ==================================================
    // Tell Shader to use Texture Unit 0
    // ==================================================
    glUseProgram(shaderProgram);

    glUniform1i(
        glGetUniformLocation(
            shaderProgram,
            "texture1"
        ),
        0
    );


    // ==================================================
    // Render Loop
    // ==================================================
    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);


        // Clear screen
        glClearColor(
            0.9f,
            0.5f,
            0.8f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT
        );


        // ==================================================
        // Activate Texture Unit 0
        // ==================================================
        glActiveTexture(
            GL_TEXTURE0
        );

        glBindTexture(
            GL_TEXTURE_2D,
            texture
        );


        // Use Shader
        glUseProgram(shaderProgram);


        // Bind VAO
        glBindVertexArray(VAO);


        // Draw Rectangle
        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            0
        );


        // Swap buffers
        glfwSwapBuffers(window);

        // Poll events
        glfwPollEvents();
    }


    // ==================================================
    // Delete Resources
    // ==================================================
    glDeleteVertexArrays(
        1,
        &VAO
    );

    glDeleteBuffers(
        1,
        &VBO
    );

    glDeleteBuffers(
        1,
        &EBO
    );

    glDeleteTextures(
        1,
        &texture
    );

    glDeleteProgram(
        shaderProgram
    );


    // ==================================================
    // Terminate
    // ==================================================
    glfwTerminate();

    return 0;
}


// ======================================================
// Input
// ======================================================
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(
        window,
        GLFW_KEY_ESCAPE
    ) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(
            window,
            true
        );
    }
}


// ======================================================
// Resize Window
// ======================================================
void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height)
{
    glViewport(
        0,
        0,
        width,
        height
    );
}