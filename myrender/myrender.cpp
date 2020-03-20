// myrender.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Shader,顶点着色器设计一个输出
    const GLchar* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "out vec4 vertexColor;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "vertexColor =vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "}\0";

    /*const GLchar* vertexShader3Source = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "layout (location = 1) in vec3 color;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
        "vertexColor = color;\n"
        "}\0";*/

    // 创建顶点着色器，用ID引用
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // 将着色器源码附加到着色器上，之后进行编译
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 片段着色器
    const GLchar* fragmentShader1Source = "#version 330 core\n"
        "out vec4 color;\n"
        "uniform vec4 ourColor;\n"
        "void main()\n"
        "{\n"
        "color = ourColor;\n"
        "}\n\0";
    const GLchar* fragmentShader2Source = "#version 330 core\n"
        "out vec4 color;\n"
        "in vec4 vertexColor;\n"
        "void main()\n"
        "{\n"
        "color = vertexColor; // The color yellow \n"
        "}\n\0";
    GLuint fragmentShaderYello = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYello, 1, &fragmentShader1Source, NULL);
    glCompileShader(fragmentShaderYello);
    // 校验shader是否编译通过
    glGetShaderiv(fragmentShaderYello, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderYello, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glShaderSource(fragmentShaderOrange, 1, &fragmentShader2Source, NULL);
    glCompileShader(fragmentShaderOrange);

    GLuint shaderProgramYellow = glCreateProgram();
    GLuint shaderProgramOrange = glCreateProgram();
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachObjectARB(shaderProgramYellow, fragmentShaderYello);
    glLinkProgram(shaderProgramYellow);

    glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);

    //glDeleteShader(vertexShader);
    //glDeleteShader(shaderProgramYellwo);


    // 索引缓冲
    //GLfloat vertices[] = {
    //     0.0f, 0.8f, 0.0f,  // Top Right
    //     0.5f, 0.5f, 0.0f,  // Top Right
    //    -0.5f, 0.0f, 0.0f,
    //     0.5f, 0.0f, 0.0f,
    //     0.5f, 0.0f, 0.0f,
    //     0.0f, -0.8f, 0.0f,
    //};
    //GLuint indices[] = {  // Note that we start from 0!
    //    0, 2, 4,  // First Triangle
    //    2, 4, 5   // Second Triangle
    //};

    GLfloat firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // Left 
        -0.0f, -0.5f, 0.0f,  // Right
        -0.45f, 0.5f, 0.0f,  // Top 
    };
    GLfloat secondTriangle[] = {
         0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Left
         0.9f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Right
         0.45f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // Top 
    };
    // 创建顶点缓冲对象
    GLuint VBO[2],VAO[2];
    glGenBuffers(2, VBO);
    glGenVertexArrays(2, VAO);


    // 第一个三角形
    glBindVertexArray(VAO[0]);
    // 绑定缓冲类型，顶点缓冲对象类型：GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    // 将顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    // 设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // 第二个三角形
    glBindVertexArray(VAO[1]);
    // 绑定缓冲类型，顶点缓冲对象类型：GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    // 将顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    // 设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);


    // EBO绑定为索引缓冲对象
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //// 设置顶点属性
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    //glEnableVertexAttribArray(0);
    
    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as
    // the currently bound vertex buffer object so afterwards we can safely unbind
    //glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent 
    // strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    /*glBindVertexArray(0); */

     // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(shaderProgramYellow, "ourColor");
        glUseProgram(shaderProgramYellow);
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
        // Draw our first triangle
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgramOrange);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    //glDeleteBuffers(1, &EBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
