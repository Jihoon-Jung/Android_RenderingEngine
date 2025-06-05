//
// Created by jihoon jung on 2025. 6. 5..
//

#include "Shaders.h"

struct ShaderSet {
    std::string name;
    const char* vertexSource;
    const char* fragmentSource;
};

const char *SimpleVertexShaderSource = R"(#version 300 es
layout (location = 0) in vec3 aPos;
uniform mat4 uMVP;
void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
}
)";

const char *SimpleRedFragmentShaderSource = R"(#version 300 es
precision mediump float;
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

std::vector<ShaderSet> shaderList = {
        {"SimpleRed", SimpleVertexShaderSource, SimpleRedFragmentShaderSource}
};

Shaders::Shaders() {
}

Shaders::~Shaders() {

}

bool Shaders::CreateShaderProgram() {

    for (const auto& shader : shaderList)
    {
        const char* vertexShaderSource = shader.vertexSource;
        const char* fragmentShaderSource = shader.fragmentSource;

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            LOGE("ERROR::SHADER::VERTEX::COMPILATION_FAILED \n");
            return false;
        }
        // fragment shader
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            LOGE("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED \n");
            return false;
        }
        // link shaders
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            LOGE("ERROR::SHADER::ROGRAM::LINKING_FAILED \n");
            return false;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        shaderMap[shader.name] = shaderProgram;

    }

    return true;
}

unsigned int Shaders::GetShaderProgram(string s) {
    return shaderMap[s];
}