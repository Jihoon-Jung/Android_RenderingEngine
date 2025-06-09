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
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vFragPos;
out vec2 vUv;
out vec3 vNormal;

void main() {
    vFragPos = vec3(uModel * vec4(aPos, 1.0));
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vUv = aUv;
    gl_Position = uProj * uView * vec4(vFragPos, 1.0);
}
)";

const char *SimpleRedFragmentShaderSource = R"(#version 300 es
precision mediump float;
in vec2 vUv;
in vec3 vNormal;
in vec3 vFragPos;
out vec4 FragColor;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D uTexture;

void main() {
    // 텍스처 샘플링
    vec4 texColor = texture(uTexture, vUv);

    // Lighting 계산
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(uLightPos - vFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectLightDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectLightDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * texColor.rgb;
    FragColor = vec4(result, texColor.a);
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
        LOGI("Shader Compile finish and add shaderProgram : %d", shaderProgram);
    }

    return true;
}

unsigned int Shaders::GetShaderProgram(string s) {
    return shaderMap[s];
}