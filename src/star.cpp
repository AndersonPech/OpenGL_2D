#include "main.hpp"


Stars make_stars() {
    Stars star;
    std::vector<glm::vec3> star_Position;
    for (int i = 0; i < 10000; ++i) {
        float x = (rand() % 2000 + (-1000));
        x = x / 1000;
        float y = (rand() % 2000 + (-1000));
        y = y / 1000;
        star_Position.emplace_back(x, y, 0);
    }

    glGenVertexArrays(1, &star.vao);
    glGenBuffers(1, &star.vbo);
 
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(star.vao);

    glBindBuffer(GL_ARRAY_BUFFER, star.vbo);
    glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(glm::vec3), &star_Position[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    return star;
};
