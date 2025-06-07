//
// Created by jihoon jung on 2025. 6. 5..
//

#include "Geometry.h"
void Geometry::createCube_Geometry() {
    std::vector<Vertex> cubeVertices = {
            // 뒤쪽 face (Z-)
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}}, // 0
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}}, // 1
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}}, // 2
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}}, // 3

            // 앞쪽 face (Z+)
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}}, // 4
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}}, // 5
            {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}}, // 6
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}}, // 7

            // 위쪽 face (Y+)
            {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}}, // 8 (3)
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}}, // 9 (2)
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}}, // 10 (6)
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}}, // 11 (7)

            // 아래쪽 face (Y-)
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}}, // 12 (0)
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}}, // 13 (1)
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}}, // 14 (5)
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}}, // 15 (4)

            // 오른쪽 face (X+)
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}}, // 16 (1)
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}}, // 17 (2)
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}}, // 18 (6)
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}}, // 19 (5)

            // 왼쪽 face (X-)
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}}, // 20 (0)
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}}, // 21 (3)
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}}, // 22 (7)
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}}  // 23 (4)
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

    _indexCount = indices.size();

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

void Geometry::createSphere_Geometry() {
    const int sectorCount = 36; // 경도 분할
    const int stackCount = 18;  // 위도 분할
    const float radius = 0.5f;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // 정점 생성
    for (int i = 0; i <= stackCount; ++i) {
        float stackAngle = M_PI / 2 - i * M_PI / stackCount; // 위도
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * M_PI / sectorCount; // 경도

            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            float u = (float)j / sectorCount;
            float v = 1.0f - (float)i / stackCount; // y축 뒤집기(텍스처 정상)

            vertices.push_back({Eigen::Vector3f(x, y, z), Eigen::Vector2f(u, v)});
        }
    }

    // 인덱스 생성
    for (int i = 0; i < stackCount; ++i) {
        int k1 = i * (sectorCount + 1);     // 현재 스택의 시작 인덱스
        int k2 = k1 + sectorCount + 1;      // 다음 스택의 시작 인덱스

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                // 위 삼각형
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                // 아래 삼각형
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    _indexCount = indices.size();

    std::vector<float> vertexBuffer;
    for (const auto& v : vertices) {
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Geometry::createPyramid_Geometry() {
    std::vector<Vertex> vertices = {
            // 바닥 (정사각형)
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}}, // 0
            {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}}, // 1
            {{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}}, // 2
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}}, // 3
            // 꼭짓점
            {{ 0.0f,  0.5f,  0.0f}, {0.5f, 0.5f}}  // 4
    };

    std::vector<unsigned int> indices = {
            // 바닥
            0, 1, 2, 2, 3, 0,
            // 옆면 4개
            0, 1, 4,
            1, 2, 4,
            2, 3, 4,
            3, 0, 4
    };

    _indexCount = indices.size();

    std::vector<float> vertexBuffer;
    for (const auto& v : vertices) {
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

unsigned int Geometry::getVAO() {
    return VAO;
}
unsigned int Geometry::getVBO() {
    return VBO;
}
unsigned int Geometry::getEBO() {
    return EBO;
}

Geometry::~Geometry() {
    LOGI("Remove Geometry");
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Geometry::createDefaultPlane(int sizeX, int sizeZ) {
    vector<Vertex> gridVertex;

    for (int z = 0; z < sizeZ + 1; z++)
    {
        for (int x = 0; x < sizeX + 1; x++)
        {
            Vertex v;
            v.position = Eigen::Vector3f(static_cast<float>(x), 0, static_cast<float>(z));
            v.uv = Eigen::Vector2f(static_cast<float>(x), static_cast<float>(sizeZ - z));
            gridVertex.push_back(v);
        }
    }

    vector<unsigned int> idx;
    for (int z = 0; z < sizeZ; z++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            //  [0]
            //   |	\
			//  [2] - [1]
            idx.push_back((sizeX + 1) * (z + 1) + (x));
            idx.push_back((sizeX + 1) * (z)+(x + 1));
            idx.push_back((sizeX + 1) * (z)+(x));
            //  [1] - [2]
            //   	\  |
            //		  [0]
            idx.push_back((sizeX + 1) * (z)+(x + 1));
            idx.push_back((sizeX + 1) * (z + 1) + (x));
            idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
        }
    }

    _indexCount = idx.size();

    std::vector<float> vertexBuffer;
    for (const auto& v : gridVertex)
    {
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(unsigned int), idx.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
