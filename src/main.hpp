#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include "main.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <stdlib.h> 

#include <cstdlib>
#include <chrono>



struct circle_t {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizeiptr n_verts = 0;

    std::vector<glm::vec4> positions;
    std::vector<glm::vec2> tcs;
};

struct Stars  {
    GLuint vao = 1;
    GLuint vbo = 1;
};

struct SpaceShip {
    GLuint vao = 2;
    GLuint vbo = 2;
    GLuint ebo = 1;
    std::vector<float> triangles;
    glm::mat4 translate = glm::mat4(1);
    glm::mat4 rotate = glm::mat4(1);

};


/**
 * 1000 dots to represent stars
 * */
Stars make_stars();


/**
 * Two triangles to form a rectangle
 * */
SpaceShip make_Ship();


/**
 * Renders a circle
 **/ 
circle_t make_circle(float cx, float cy, float r);


#endif