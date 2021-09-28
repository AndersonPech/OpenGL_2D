// C++17 doesn't have std::pi
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "main.hpp"
#include <stb/stb_image.h>
#include <stdlib.h> 

#include <cstdlib>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chicken3421/chicken3421.hpp>

typedef std::chrono::high_resolution_clock Clock;

const char *VERT_PATH = "res/shaders/shader.vert";
const char *VERT_PATH2 = "res/shaders/vert2.glsl";
const char *VERT_PATH3 = "res/shaders/vert3.glsl";
const char *FRAG_PATH = "res/shaders/shader.frag";
const char *FRAG_PATH2 = "res/shaders/shader2.frag";
const char *FRAG_PATH3 = "res/shaders/shade3.glsl";


struct image_t {
    int width;
    int height;
    int n_channels;
    void *data;
};

/**
 * Load image. Taken from tutorial
 */
image_t load_image(const std::string &filename) {
    image_t img;

    stbi_set_flip_vertically_on_load(true);
    img.data = stbi_load(filename.data(), &img.width, &img.height, &img.n_channels, 0);

    chicken3421::expect(img.data, "Could not read " + filename);

    return img;
}

/**
 * Delete image, taken from tutorial
 */
void delete_image(image_t &img) {
    stbi_image_free(img.data);

    img.data = nullptr;
    img.width = 0;
    img.height = 0;
    img.n_channels = 0;
}

//Load textures, taken from tutorial code
GLuint make_texture(const std::string &path) {
    image_t img = load_image(path);

    GLint format;
    if (img.n_channels == 3)
        format = GL_RGB;
    else
        format = GL_RGBA;

    GLuint tex;
    glGenTextures(1, &tex);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, format, img.width, img.height, 0, format, GL_UNSIGNED_BYTE, img.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete_image(img);

    return tex;
}

//Load the shaders. Taken from Tutorial code.
GLuint load_shader(const std::string &path, GLenum type) {
    std::string source = chicken3421::read_file(path);
    const char* src = source.data();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


/**
 * W to go up,
 * S to go down,
 * A to go left
 * D to go right
 * Q to rotate left
 * E to rotate right 
 */
void on_key_press(GLFWwindow *win, int key, int scancode, int action, int mods) {
    SpaceShip *mesh = (SpaceShip *) glfwGetWindowUserPointer(win);
    switch(key) {
        case GLFW_KEY_W:
            mesh->translate = glm::translate(mesh->translate, glm::vec3(0, 0.01, 0));
            break;
        case GLFW_KEY_S:
            mesh->translate = glm::translate(mesh->translate, glm::vec3(0, -0.01, 0));
            break;
        case GLFW_KEY_A:
            mesh->translate = glm::translate(mesh->translate, glm::vec3(-0.01, 0, 0));
            break;
        case GLFW_KEY_D:
            mesh->translate = glm::translate(mesh->translate, glm::vec3(0.01, 0, 0));
            break;
        case GLFW_KEY_Q:
            mesh->rotate = glm::rotate(mesh->rotate, (float) M_PI/6, glm::vec3(0, 0, 1));
            break;
        case GLFW_KEY_E:

            mesh->rotate = glm::rotate(mesh->rotate, (float) -M_PI/6, glm::vec3(0, 0, 1));
        default:
            break;
    }
}

int main() {
    // Create our window
    GLFWwindow *window = chicken3421::make_opengl_window(1000, 1000, "ass1");

    // make context current (not too important for our purposes)
    glfwMakeContextCurrent(window);
    // load OpenGL
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);


    // load shader
    GLuint vert_shader, frag_shader, frag_shader_white, vert_shader2, vert_shader3, frag_shader_falcon;
    vert_shader = load_shader(VERT_PATH, GL_VERTEX_SHADER);
    vert_shader2 = load_shader(VERT_PATH2, GL_VERTEX_SHADER);

    frag_shader = load_shader(FRAG_PATH, GL_FRAGMENT_SHADER);
    frag_shader_white = load_shader(FRAG_PATH2, GL_FRAGMENT_SHADER);

    vert_shader3 = load_shader(VERT_PATH3, GL_VERTEX_SHADER);
    frag_shader_falcon = load_shader(FRAG_PATH3, GL_FRAGMENT_SHADER);
    

    // create our shader program that will run on the GPU
    GLuint program = chicken3421::make_program(vert_shader, frag_shader);
    GLuint white_program = chicken3421::make_program(vert_shader2, frag_shader_white);
    GLuint space_ship = chicken3421::make_program(vert_shader3, frag_shader_falcon);
    
    //Delete shaders
    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);
    glDeleteShader(frag_shader_white);
    glDeleteShader(vert_shader2);
    glDeleteShader(vert_shader3);
    glDeleteShader(frag_shader_falcon);

    //Make the meshes
    circle_t circle = make_circle(0, 0, 0.5);
    SpaceShip ship = make_Ship();
    Stars star = make_stars();


    GLint transform_loc = glGetUniformLocation(program, "transform");
    GLint transform_loc2 = glGetUniformLocation(space_ship, "transform");
    GLuint texture1 = make_texture("res/shaders/Image/oglass.jpg");
    GLuint texture2 = make_texture("res/shaders/Image/blue.jpg");
    GLuint texture3 = make_texture("res/shaders/Image/ship.png");

    glfwSetWindowUserPointer(window, &ship);
    glfwSetKeyCallback(window, on_key_press);

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    int current_buffer = texture2;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    
     // Render loop
    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Draw the stars (anything more than 10000 gives weird behaviour
        //if program doesn't work reduce the number of stars)
        glUseProgram(white_program);
        glBindVertexArray(star.vao);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, 10000);

        ///Draw the sun
        glUseProgram(program);
        glBindVertexArray(circle.vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, circle.vbo);

        //Change the colour of the star overtime
        float timeValue = glfwGetTime();
        float blueValue = sin(timeValue) / 2.0f + 0.6f;
        if (blueValue < 0.5f) {
            blueValue = 0.5f;
        } 
        
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        int duration = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();

        if ((duration % 10) == 0.0f) {
            if (current_buffer == texture2) {
                current_buffer = texture1;
            } else {
                current_buffer = texture2;
            }
        } 
        glBindTexture(GL_TEXTURE_2D, current_buffer);
        
        int vertexColorLocation = glGetUniformLocation(program, "ourColor");
        glUniform4f(vertexColorLocation, 0.5f, blueValue, 1.0f, 0.5f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circle.n_verts);


        //Rotate ship and translate
        glm::mat4 transform = ship.translate * ship.rotate; 
        
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));
        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //Draw the ship
        glBindVertexArray(ship.vao);
        glUseProgram(space_ship);
        
        glBindTexture(GL_TEXTURE_2D, texture3);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glUniformMatrix4fv(transform_loc2, 1, GL_FALSE, glm::value_ptr(transform));
        glDisable(GL_BLEND);

        // get events 
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    //Delete all the programs
    glDeleteProgram(program);
    glDeleteProgram(white_program);
    glDeleteProgram(space_ship);

    glfwTerminate();
    return EXIT_SUCCESS;
}

