//
// Created by jihoon jung on 2025. 6. 5..
//

#include "Shaders.h"

struct ShaderSet {
    std::string name;
    const char* vertexSource;
    const char* fragmentSource;
};


const char *DefaultVertexShaderSource = R"(#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uLightSpaceMat;

out vec3 vFragPos;
out vec2 vUv;
out vec3 vNormal;
out vec4 vFragPosLightSpace;

void main() {
    vFragPos = vec3(uModel * vec4(aPos, 1.0));
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vUv = aUv;
    vFragPosLightSpace = uLightSpaceMat * vec4(vFragPos, 1.0);
    gl_Position = uProj * uView * vec4(vFragPos, 1.0);
}
)";

const char *DefaultFragmentShaderSource = R"(#version 300 es
precision mediump float;

in vec2 vUv;
in vec3 vNormal;
in vec3 vFragPos;
in vec4 vFragPosLightSpace;

out vec4 FragColor;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D uTexture;
uniform sampler2D uShadowMap;

float ShadowCalculation(vec3 projCoords, float currentDepth, vec3 normal, vec3 lightDir)
{
    float closestDepth = texture(uShadowMap, projCoords.xy).r;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(uShadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(uShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

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

    // Shadow 계산
    vec3 projCoords = vFragPosLightSpace.xyz / vFragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    float shadow = ShadowCalculation(projCoords, currentDepth, normal, lightDir);

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor.rgb;
//    vec3 result = (ambient + diffuse + specular) * texColor.rgb;
    FragColor = vec4(result, 1.0);

}
)";


const char *ShadowMapFragmentShaderSource = R"(#version 300 es
precision mediump float;

void main() {

}
)";

const char *SimpleWhiteVertexShaderSource = R"(#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;
layout (location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uLightSpaceMat;

out vec3 vFragPos;
out vec2 vUv;
out vec3 vNormal;
out vec4 vFragPosLightSpace;

void main() {
    vFragPos = vec3(uModel * vec4(aPos, 1.0));
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vUv = aUv;
    vFragPosLightSpace = uLightSpaceMat * vec4(vFragPos, 1.0);
    gl_Position = uProj * uView * vec4(vFragPos, 1.0);
}
)";
const char *SimpleWhiteFragmentShaderSource = R"(#version 300 es
precision mediump float;

in vec2 vUv;
in vec3 vNormal;
in vec3 vFragPos;
in vec4 vFragPosLightSpace;

out vec4 FragColor;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D uShadowMap;

float ShadowCalculation(vec3 projCoords, float currentDepth, vec3 normal, vec3 lightDir)
{
    float closestDepth = texture(uShadowMap, projCoords.xy).r;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(uShadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(uShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main() {
    // 텍스처 샘플링
    vec4 texColor = vec4(1.0, 1.0, 1.0, 1.0);

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

    // Shadow 계산
    vec3 projCoords = vFragPosLightSpace.xyz / vFragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    float shadow = ShadowCalculation(projCoords, currentDepth, normal, lightDir);

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * texColor.rgb;
//    vec3 result = (ambient + diffuse + specular) * texColor.rgb;
    FragColor = vec4(texColor.rgb, 1.0);

//    vec3 projCoords = vFragPosLightSpace.xyz / vFragPosLightSpace.w;
//    projCoords = projCoords * 0.5 + 0.5;
//    float depth = texture(uShadowMap, projCoords.xy).r;
//
//
//    FragColor = vec4(vec3(depth), 1.0);
}
)";

const char *DebugShadowMapVertexShaderSource = R"(#version 300 es
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec2 vUv;

void main() {
    vUv = aUv;
    gl_Position = vec4(aPos, 1.0);
}
)";

const char *DebugShadowMapFragmentShaderSource = R"(#version 300 es
precision mediump float;
in vec2 vUv;
out vec4 FragColor;
uniform sampler2D uShadowMap;

void main() {
    float depthValue = texture(uShadowMap, vUv).r;
    FragColor = vec4(vec3(depthValue), 1.0);
}
)";

std::vector<ShaderSet> shaderList = {
        {"DefaultShader", DefaultVertexShaderSource, DefaultFragmentShaderSource},
        {"ShadowMapShader", DefaultVertexShaderSource, ShadowMapFragmentShaderSource},
        {"SimpleWhiteShader", SimpleWhiteVertexShaderSource, SimpleWhiteFragmentShaderSource},
        {"DebugShadowMapShader", DebugShadowMapVertexShaderSource, DebugShadowMapFragmentShaderSource}
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

        // Vertex Shader 컴파일
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            LOGE("ERROR::SHADER::VERTEX::COMPILATION_FAILED for %s\n%s", shader.name.c_str(), infoLog);
            return false;
        }

        // Fragment Shader 컴파일
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            LOGE("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED for %s\n%s", shader.name.c_str(), infoLog);
            glDeleteShader(vertexShader);  // 정리
            return false;
        }

        // Shader Program 링크
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            LOGE("ERROR::SHADER::PROGRAM::LINKING_FAILED for %s\n%s", shader.name.c_str(), infoLog);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return false;
        }

        // 셰이더 객체 정리
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        shaderMap[shader.name] = shaderProgram;
        LOGI("Shader Compile and Link Success for %s (Program ID: %d)", shader.name.c_str(), shaderProgram);
    }

    return true;
}

unsigned int Shaders::GetShaderProgram(string s) {
    return shaderMap[s];
}