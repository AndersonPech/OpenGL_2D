#include "main.hpp"

SpaceShip make_Ship() {
    SpaceShip ship;
    float triangles[] = {
        0.1f,  0.1f, 0.0f, 1.0f, 1.0f,  // top right
        0.1f, -0.1f, 0.0f,  1.0f, 0.0f, // bottom right
        -0.1f, -0.1f, 0.0f,  0.0f, 0.0f, // bottom left
        -0.1f,  0.1f, 0.0f,  0.0f, 1.0f// top left 
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(2, &ship.vao);
    glGenBuffers(2, &ship.vbo);
    glGenBuffers(1, &ship.ebo);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(ship.vao);

    glBindBuffer(GL_ARRAY_BUFFER, ship.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ship.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);  

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    return ship;
}
