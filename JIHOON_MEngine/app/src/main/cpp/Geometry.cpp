//
// Created by jihoon jung on 2025. 6. 5..
//

#include "Geometry.h"
void Geometry::CreateCube_Geometry() {
    std::vector<Vertex> cubeVertices = {
            // 뒤쪽 face (Z-)
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}}, // 0
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}}, // 1
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}}, // 2
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}}, // 3

            // 앞쪽 face (Z+)
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}}, // 4
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}}, // 5
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}}, // 6
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}}, // 7

            // 위쪽 face (Y+)
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}}, // 8 (3)
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}}, // 9 (2)
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}}, // 10 (6)
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}}, // 11 (7)

            // 아래쪽 face (Y-)
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}}, // 12 (0)
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}}, // 13 (1)
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}}, // 14 (5)
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}}, // 15 (4)

            // 오른쪽 face (X+)
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}}, // 16 (1)
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}}, // 17 (2)
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}}, // 18 (6)
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}}, // 19 (5)

            // 왼쪽 face (X-)
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}}, // 20 (0)
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}}, // 21 (3)
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}}, // 22 (7)
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}}  // 23 (4)
    };

    std::vector<unsigned int> indices = {
            // 뒤쪽 face
            0, 1, 2, 2, 3, 0,
            // 앞쪽 face
            4, 5, 6, 6, 7, 4,
            // 위쪽 face
            8, 9, 10, 10, 11, 8,
            // 아래쪽 face
            12, 13, 14, 14, 15, 12,
            // 오른쪽 face
            16, 17, 18, 18, 19, 16,
            // 왼쪽 face
            20, 21, 22, 22, 23, 20
    };


    std::vector<float> vertexBuffer;
    for (const auto& v : cubeVertices) {
        vertexBuffer.push_back(v.position.x());
        vertexBuffer.push_back(v.position.y());
        vertexBuffer.push_back(v.position.z());
        vertexBuffer.push_back(v.uv.x());
        vertexBuffer.push_back(v.uv.y());
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);              // position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float)*3));  // uv
    glEnableVertexAttribArray(1);


    // 바인딩 해제 (안전을 위해)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

unsigned int Geometry::GetVAO() {
    return VAO;
}
unsigned int Geometry::GetVBO() {
    return VBO;
}
unsigned int Geometry::GetEBO() {
    return EBO;
}