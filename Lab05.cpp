// Lab05.cpp : This file contains the 'main' function.
// Program execution begins and ends there.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// ==============================
// Vertex Shader
// ==============================

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"}\0";


// ==============================
// Fragment Shader
// ==============================

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";


int main()
{
    // ==============================
    // Random
    // ==============================

    srand(static_cast<unsigned int>(time(0)));


    // ==============================
    // GLFW initialize
    // ==============================

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    // ==============================
    // Create Window
    // ==============================

    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "LearnOpenGL Lab05",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window"
            << std::endl;

        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(
        window,
        framebuffer_size_callback
    );


    // ==============================
    // GLAD
    // ==============================

    if (!gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD"
            << std::endl;

        return -1;
    }


    // ==============================
    // Vertex Shader
    // ==============================

    unsigned int vertexShader =
        glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        NULL
    );

    glCompileShader(vertexShader);

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

        std::cout
            << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
            << infoLog
            << std::endl;
    }


    // ==============================
    // Fragment Shader
    // ==============================

    unsigned int fragmentShader =
        glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        NULL
    );

    glCompileShader(fragmentShader);

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

        std::cout
            << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
            << infoLog
            << std::endl;
    }


    // ==============================
    // Shader Program
    // ==============================

    unsigned int shaderProgram =
        glCreateProgram();

    glAttachShader(
        shaderProgram,
        vertexShader
    );

    glAttachShader(
        shaderProgram,
        fragmentShader
    );

    glLinkProgram(shaderProgram);

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

        std::cout
            << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
            << infoLog
            << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // ==============================
    // Vertex Data
    // รูป 8 เหลี่ยม
    // ==============================

    float vertices[] =
    {
        // จุดกลาง
         0.0f,  0.0f, 0.0f,       // 0

         // จุดรอบนอก 8 จุด
          0.0f,  0.5f, 0.0f,       // 1 บน
          0.3536f,  0.3536f, 0.0f, // 2 บนขวา
          0.5f,  0.0f, 0.0f,       // 3 ขวา
          0.3536f, -0.3536f, 0.0f, // 4 ล่างขวา
          0.0f, -0.5f, 0.0f,       // 5 ล่าง
         -0.3536f, -0.3536f, 0.0f, // 6 ล่างซ้าย
         -0.5f,  0.0f, 0.0f,       // 7 ซ้าย
         -0.3536f,  0.3536f, 0.0f, // 8 บนซ้าย

         // ซ้ำจุดบนเพื่อปิดรูป
          0.0f,  0.5f, 0.0f       // 9
    };


    // ==============================
    // VAO / VBO
    // ==============================

    unsigned int VBO, VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

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

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // ==============================
    // Render Loop
    // ==============================

    double lastRandomTime = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        // ==============================
        // Input
        // ==============================

        processInput(window);


        // ==============================
        // Background
        // ==============================

        glClearColor(
            1.0f,
            0.8f,
            0.9f,
            1.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);


        // ==============================
        // Shader
        // ==============================

        glUseProgram(shaderProgram);


        // ==============================
        // เวลา
        // ==============================

        float timeValue =
            static_cast<float>(glfwGetTime());


        // ==============================
        // ตำแหน่งสี
        // ==============================

        int vertexColorLocation =
            glGetUniformLocation(
                shaderProgram,
                "ourColor"
            );


     

        float t = timeValue * 0.5f;

        float phase = fmod(t, 3.0f);

        float red;
        float green;
        float blue;


        if (phase < 1.0f)
        {
          

            float mixValue = phase;

            red =
                0.0f * (1.0f - mixValue)
                + 1.0f * mixValue;

            green =
                1.0f * (1.0f - mixValue)
                + 0.2f * mixValue;

            blue =
                0.0f * (1.0f - mixValue)
                + 0.6f * mixValue;
        }
        else if (phase < 2.0f)
        {
         

            float mixValue = phase - 1.0f;

            red =
                1.0f * (1.0f - mixValue)
                + 0.0f * mixValue;

            green =
                0.2f * (1.0f - mixValue)
                + 0.8f * mixValue;

            blue =
                0.6f * (1.0f - mixValue)
                + 1.0f * mixValue;
        }
        else
        {
     

            float mixValue = phase - 2.0f;

            red = 0.0f;

            green =
                0.8f * (1.0f - mixValue)
                + 1.0f * mixValue;

            blue =
                1.0f * (1.0f - mixValue);
        }


        // ส่งสีไป Shader

        glUniform4f(
            vertexColorLocation,
            red,
            green,
            blue,
            1.0f
        );


       

        if (timeValue - lastRandomTime >= 0.005)
        {
            int randomNumber = rand() % 100;

            std::cout
                << randomNumber
                << std::endl;

            lastRandomTime = timeValue;
        }


       

        glDrawArrays(
            GL_TRIANGLE_FAN,
            0,
            10
        );


        // ==============================
        // Swap Buffers
        // ==============================

        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    // ==============================
    // Delete Resources
    // ==============================

    glDeleteVertexArrays(
        1,
        &VAO
    );

    glDeleteBuffers(
        1,
        &VBO
    );

    glDeleteProgram(
        shaderProgram
    );


    // ==============================
    // Terminate
    // ==============================

    glfwTerminate();

    return 0;
}


// ==============================
// Process Input
// ==============================

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


// ==============================
// Framebuffer Size Callback
// ==============================

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