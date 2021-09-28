#include "main.hpp"

/**
 * Draws a circle. Code is similar to tutorial. Removed matrixes for transformations
 */
circle_t make_circle(float cx, float cy, float r) {
    circle_t c;
    int detail = 32;
    c.positions.emplace_back(cx, cy, 0, 1);
    c.tcs.emplace_back(0.5f, 0.5f);
    c.n_verts++;

    float slice = 2 * M_PI/detail;
    for (int i = 0; i <= detail; ++i) {
        float theta = i * slice;
        float x = cx + r * std::cos(theta);
        float y = cy + r * std::sin(theta);

        float tx = (x/r + 1) * 0.5f;
        float ty = (y/r + 1) * 0.5f;

        c.positions.emplace_back(x, y, 0, 1);
        c.tcs.emplace_back(tx, ty);
        c.n_verts++;
    }

    glGenVertexArrays(1, &c.vao);
    glGenBuffers(1, &c.vbo);

    glBindVertexArray(c.vao);
    glBindBuffer(GL_ARRAY_BUFFER, c.vbo);

    glBufferData(GL_ARRAY_BUFFER, c.n_verts * (sizeof(glm::vec4) + sizeof(glm::vec2)), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, c.n_verts * sizeof(glm::vec4), c.positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, c.n_verts * sizeof(glm::vec4), c.n_verts * sizeof(glm::vec2), c.tcs.data());

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)(c.n_verts * sizeof(glm::vec4)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return c;
}
