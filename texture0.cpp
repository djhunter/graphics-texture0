/*===================================================
// Skeleton Project for CS-150: 3D Computer Graphics
//===================================================*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;
using namespace glm;

struct Vertex {
    vec3 position;
    vec2 tex_coords;
};

const array<Vertex, 24> cube =
{{
    // top (if +z is up, +y is right, +x is forward)
    {vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)},
    {vec3(-1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)},
    {vec3(-1.0f, -1.0f, 1.0f), vec2(0.0f, 1.0f)},
    {vec3(1.0f, -1.0f, 1.0f), vec2(1.0f, 1.0f)},
    // bottom
    {vec3(1.0f, 1.0f, -1.0f), vec2(1.0f, 0.0f)},
    {vec3(1.0f, -1.0f, -1.0f), vec2(0.0f, 0.0f)},
    {vec3(-1.0f, -1.0f, -1.0f), vec2(0.0f, 1.0f)},
    {vec3(-1.0f, 1.0f, -1.0f), vec2(1.0f, 1.0f)},
    // right
    {vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)},
    {vec3(1.0f, 1.0f, -1.0f), vec2(0.0f, 0.0f)},
    {vec3(-1.0f, 1.0f, -1.0f), vec2(0.0f, 1.0f)},
    {vec3(-1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)},
    // left
    {vec3(1.0f, -1.0f, 1.0f), vec2(1.0f, 0.0f)},
    {vec3(-1.0f, -1.0f, 1.0f), vec2(0.0f, 0.0f)},
    {vec3(-1.0f, -1.0f, -1.0f), vec2(0.0f, 1.0f)},
    {vec3(1.0f, -1.0f, -1.0f), vec2(1.0f, 1.0f)},
    // front
    {vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)},
    {vec3(1.0f, -1.0f, 1.0f), vec2(0.0f, 0.0f)},
    {vec3(1.0f, -1.0f, -1.0f), vec2(0.0f, 1.0f)},
    {vec3(1.0f, 1.0f, -1.0f), vec2(1.0f, 1.0f)},
    // back
    {vec3(-1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)},
    {vec3(-1.0f, 1.0f, -1.0f), vec2(0.0f, 0.0f)},
    {vec3(-1.0f, -1.0f, -1.0f), vec2(0.0f, 1.0f)},
    {vec3(-1.0f, -1.0f, 1.0f), vec2(1.0f, 1.0f)}
}};

const array<GLuint, 36> cube_idx = { 0, 1, 2, 0, 2, 3,
                                     4, 5, 6, 4, 6, 7,
                                     8, 9,10, 8,10,11,
                                     12,13,14,12,14,15,
                                     16,17,18,16,18,19,
                                     20,21,22,20,22,23 };

const GLchar* vertexShaderSource = R"glsl(
#version 330
uniform mat4 MVP;
in vec3 vPos;
in vec2 tc;
out vec2 texCoord;
void main()
{
    gl_Position = MVP * vec4(vPos, 1.0);
    texCoord = tc;
}
)glsl";

const GLchar* fragmentShaderSource = R"glsl(
#version 330
uniform vec3 uColor;
in vec2 texCoord;
out vec4 fragColor;
uniform sampler2D tex0;
void main()
{
    fragColor = texture(tex0, texCoord) * vec4(uColor, 1.0);
}
)glsl";

static void errorCallback(int error, const char* description)
{
    cerr << "GLFW Error: " << description << endl;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void compileShader(GLuint shader, const char *shaderText)
{
    glShaderSource(shader, 1, &shaderText, NULL);
    glCompileShader(shader);
    GLchar infoLog[8192];
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 8192, NULL, infoLog);
        cerr << "Shader failed to complile." << endl
             << "Error log: " << infoLog << endl;
    }
}

int main(void)
{
    GLFWwindow* window;
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    // Request OpenGL version 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Don't use old OpenGL
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // OSX needs this
    window = glfwCreateWindow(640, 480, "CS 150 Template Project: Textures", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    cout << "GL version: " << glGetString(GL_VERSION) << endl
         << "GL vendor: " << glGetString(GL_VENDOR) << endl
         << "GL renderer: " << glGetString(GL_RENDERER) << endl
         << "GL shading language version: "
         << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    glfwSwapInterval(1); // Framerate matches monitor refresh rate

    // Compile shaders and check for errors
    GLuint vertexShader, fragmentShader, program;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compileShader(vertexShader, vertexShaderSource);
    compileShader(fragmentShader, fragmentShaderSource);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glBindFragDataLocation(program, 0, "fragColor");
    glLinkProgram(program);
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        cerr << "ERROR: Shader linking failed." << endl;
        glDeleteProgram(program);
        program = 0u;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glUseProgram(program);

    GLint l_MVP = glGetUniformLocation(program, "MVP");
    GLint l_vPos = glGetAttribLocation(program, "vPos");
    GLint l_tc = glGetAttribLocation(program, "tc");
    GLint l_uColor = glGetUniformLocation(program, "uColor");

    GLuint VAO, VBO, EBO; // vertex array and vertex and index buffer objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO); // only one VAO; leave it bound for whole program
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cube.size()*sizeof(Vertex), cube.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(l_vPos);
    glVertexAttribPointer(l_vPos, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (GLvoid*) 0);
    glEnableVertexAttribArray(l_tc);
    glVertexAttribPointer(l_tc, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (GLvoid*) (3*sizeof(float)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube_idx.size()*sizeof(GLuint),
                 cube_idx.data(), GL_STATIC_DRAW);

    // Load textures
    int tWidth, tHeight, tComps;
    GLuint dipTexture;
    glGenTextures(1, &dipTexture);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(program, "tex0"), 0);
    glBindTexture(GL_TEXTURE_2D, dipTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *tData = stbi_load("dip.jpg", &tWidth, &tHeight, &tComps, 0);
    if (tData == NULL) {
        cerr << "Problem reading file " << "dip.jpg" << "." << endl;
    }
    else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tWidth, tHeight, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, tData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(tData);

    glEnable(GL_DEPTH_TEST);

    mat4 M, V, P, MVP;
    vec3 eye = vec3(0.0f, 5.0f, 15.0f);
    vec3 center = vec3(0.0f, 0.0f, 0.0f);
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    V = lookAt(eye, center, up);
    vec3 cubeColor = vec3(1.0f, 1.0f, 1.0f);
    glUniform3fv(l_uColor, 1, value_ptr(cubeColor));

    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ratio = width / (float) height;
        P = perspective(0.60f, ratio, 1.0f, 100.0f);
        M = rotate(mat4(1.0f), (float) glfwGetTime(), vec3(0.0f, 0.0f, 1.0f));
        MVP = P * V * M;

        glUniformMatrix4fv(l_MVP, 1, GL_FALSE, value_ptr(MVP));
        glDrawElements(GL_TRIANGLES, cube_idx.size(), GL_UNSIGNED_INT, 0);
        // check for OpenGL errors
        GLenum error_code;
        while ((error_code = glGetError()) != GL_NO_ERROR)
            cerr << "OpenGL error HEX: " << hex << error_code << endl;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
