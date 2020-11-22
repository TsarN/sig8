/*
 *  sig8 is a library for creating retro games
 *
 *  Copyright (c) 2020 Nikita Tsarev
 *  See https://github.com/TsarN/sig8/blob/master/LICENSE for license info
 */

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef SIG8_USE_GLAD
#include <glad.h>
#else
#include <GLES3/gl3.h>
#endif

static const char *vertexShaderSource =
        "#version 300 es\n"
        "layout (location = 0) in vec2 pos;\n"
        "out mediump vec2 uv;\n"
        "uniform vec2 off;\n"
        "void main()\n"
        "{\n"
        "  uv = vec2(pos.x, 1.0 - pos.y);\n"
        "  gl_Position = vec4(2.0 * (pos * (1.0 - 2.0 * off) + off - vec2(0.5)), 0.0, 1.0);\n"
        "}";

static const char *fragmentShaderSource =
        "#version 300 es\n"
        "out mediump vec4 fragColor;\n"
        "in mediump vec2 uv;\n"
        "uniform sampler2D tex;\n"
        "void main()\n"
        "{\n"
        "  fragColor = texture(tex, uv); fragColor.a = 1.0;\n"
        "}\n";

static const float screenRect[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
};

static void OnResize(void)
{
    glfwGetWindowSize(state->window.window, &state->window.width,
                      &state->window.height);

    float pixelScaleX =
            (float) state->window.width / (float) state->display.width;
    float pixelScaleY =
            (float) state->window.height / (float) state->display.height;
    float pixelScale;

    if (pixelScaleX < pixelScaleY) {
        pixelScale = floorf(pixelScaleX);
    } else {
        pixelScale = floorf(pixelScaleY);
    }

    state->window.offsetX = (1.0f - (float) state->display.width * pixelScale /
                                    (float) state->window.width) / 2.0f;
    state->window.offsetY = (1.0f - (float) state->display.height * pixelScale /
                                    (float) state->window.height) / 2.0f;
}

static void SetFramebufferSizeCallback(GLFWwindow *window, int w, int h)
{
    (void)window;
    (void)w;
    (void)h;
    OnResize();
}

static void UpdateBufferData(void)
{
    size_t size = state->display.width * state->display.height * sizeof(Color);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, state->window.screenPBO);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    uint8_t *ptr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, size,
                                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    memcpy(ptr, state->display.pixels, size);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    glBindTexture(GL_TEXTURE_2D, state->window.screenTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                    state->display.width, state->display.height,
                    GL_RGB, GL_UNSIGNED_BYTE, 0);

}

static void GLESInitPixelBuffer(void)
{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, state->window.screenPBO);
    glBufferData(GL_PIXEL_UNPACK_BUFFER,
                 state->display.width * state->display.height * sizeof(Color),
                 state->display.pixels, GL_DYNAMIC_DRAW);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 state->display.width, state->display.height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
}

static void GLESInit(void)
{
    if (state->window.screenVBO) {
        glDeleteBuffers(1, &state->window.screenVBO);
    }
    glGenBuffers(1, &state->window.screenVBO);

    if (state->window.screenPBO) {
        glDeleteBuffers(1, &state->window.screenPBO);
    }
    glGenBuffers(1, &state->window.screenPBO);

    if (state->window.screenTexture) {
        glDeleteTextures(1, &state->window.screenTexture);
    }
    glGenTextures(1, &state->window.screenTexture);

    if (state->window.screenVAO) {
        glDeleteVertexArrays(1, &state->window.screenVAO);
    }
    glGenVertexArrays(1, &state->window.screenVAO);

    // Configure buffer objects

    glBindVertexArray(state->window.screenVAO);

    glBindBuffer(GL_ARRAY_BUFFER, state->window.screenVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof screenRect, screenRect,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *) 0);
    glEnableVertexAttribArray(0);

    // Configure texture

    glBindTexture(GL_TEXTURE_2D, state->window.screenTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLESInitPixelBuffer();

    // Compile shaders

    int success;
    char infoLog[4096];

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        fputs("Vertex shader compilation failed.", stderr);
        glGetShaderInfoLog(vertexShader, sizeof infoLog, NULL, infoLog);
        fputs(infoLog, stderr);
        exit(EXIT_FAILURE);
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        fputs("Fragment shader compilation failed.", stderr);
        glGetShaderInfoLog(fragmentShader, sizeof infoLog, NULL, infoLog);
        fputs(infoLog, stderr);
        exit(EXIT_FAILURE);
    }

    if (state->window.shader) {
        glDeleteProgram(state->window.shader);
    }

    state->window.shader = glCreateProgram();
    glAttachShader(state->window.shader, vertexShader);
    glAttachShader(state->window.shader, fragmentShader);
    glLinkProgram(state->window.shader);

    glGetProgramiv(state->window.shader, GL_LINK_STATUS, &success);
    if (!success) {
        fputs("Shader linkage failed.", stderr);
        glGetProgramInfoLog(state->window.shader, sizeof infoLog, NULL,
                            infoLog);
        fputs(infoLog, stderr);
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    state->window.offLoc = glGetUniformLocation(state->window.shader, "off");

    OnResize();
    UpdateBufferData();

    glfwSetFramebufferSizeCallback(state->window.window,
                                   SetFramebufferSizeCallback);
}

static void WindowHandleEvents(void)
{
    if (!state->window.initialized) return;

    glfwPollEvents();
    if (glfwWindowShouldClose(state->window.window)) {
        state->shouldQuit = true;
    }
}

static void WindowDraw(void)
{
    if (!state->window.initialized) return;

    UpdateBufferData();
    glViewport(0, 0, state->window.width, state->window.height);
    Color black = {0, 0, 0};
    glClearColor(
            (float)black.r / 255.0f,
            (float)black.g / 255.0f,
            (float)black.b / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(state->window.shader);
    glBindVertexArray(state->window.screenVAO);

    glUniform2f(state->window.offLoc, state->window.offsetX, state->window.offsetY);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glfwSwapBuffers(state->window.window);
}

static void WindowFail(const char *err)
{
    fprintf(stderr, "sig8: %s failed\n", err);
    Deinitialize();
    exit(EXIT_FAILURE);
}

static void ErrorCallback(int errorCode, const char *message)
{
    fprintf(stderr, "sig8: glfw error %d: %s\n", errorCode, message);
}

static void WindowInit(void)
{
    if (!glfwInit()) {
        WindowFail("glfwInit");
    }

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow *window = state->window.window =
            glfwCreateWindow(512, 512, "sig8", NULL, NULL);

    if (!window) {
        WindowFail("glfwCreateWindow");
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

#ifdef SIG8_USE_GLAD
    if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
        WindowFail("gladLoadGLES2Loader");
    }
#endif

    GLESInit();

    printf("sig8: using %s\n", glGetString(GL_VERSION));

    state->window.initialized = true;
}

void WindowDeinit(void)
{
    if (!state->window.initialized) return;

    state->window.initialized = false;
    glfwTerminate();
}
