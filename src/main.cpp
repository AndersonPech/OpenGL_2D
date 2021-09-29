// C++17 doesn't have std::pi
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include "main.hpp"
#include <stb/stb_image.h>
#include <bits/stdc++.h>
#include <stdlib.h> 
#include <sstream>  

#include <cstdlib>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

#include <chicken3421/chicken3421.hpp>
#include <unistd.h>

typedef std::chrono::high_resolution_clock Clock;

const char *VERT_PATH = "res/shaders/vert1.glsl";
const char *VERT_PATH2 = "res/shaders/vert2.glsl";
const char *VERT_PATH3 = "res/shaders/vert3.glsl";
const char *FRAG_PATH = "res/shaders/shader1.glsl";
const char *FRAG_PATH2 = "res/shaders/shader2.glsl";
const char *FRAG_PATH3 = "res/shaders/shader3.glsl";


struct image_t {
    int width;
    int height;
    int n_channels;
    void *data;
};

std::unordered_map<std::string, GLint> explosions_key_frames;

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

/**
 * Make texture
 */
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

/**
 * Load shader
 */
GLuint load_shader(const std::string &path, GLenum type) {
    std::string source = chicken3421::read_file(path);
    const char* src = source.data();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    return shader;
}


/* process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
*
*/
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
            mesh->rotate = glm::rotate(mesh->rotate, (float) M_PI/12, glm::vec3(0, 0, 1));
            break;
        case GLFW_KEY_E:

            mesh->rotate = glm::rotate(mesh->rotate, (float) -M_PI/12, glm::vec3(0, 0, 1));
        default:
            break;
    }
}

/**
 * Load GLuin code from hashmap
 */ 
GLuint loadExplosionKey(int key) {
    std::string number = std::to_string(key);
    if (key < 10) {
        std::string zero = "0";
        number = zero + number;
    }

    std::string frame = "frame_";
    frame = frame + number;
    std::string end = "_delay-0.07s.gif";
    frame = frame + end;
    std::cout << frame;
    std::string position = "res/shaders/Image/Explosion/";
    position = position + frame;
    std::unordered_map<std::string, GLint>::const_iterator got = explosions_key_frames.find(position);
    if (got != explosions_key_frames.end()) {
        return got->second;
    } else {
        return INT_MAX;
    }
}

/**
 * Save memory by loading textures into hashmap
 */ 
void createExplosionTextures() {
    for (int i = 0; i < 89; ++i) {
        std::string number = std::to_string(i);
        if (i < 10) {
            std::string zero = "0";
            number = zero + number;
        }

        std::string frame = "frame_";
        frame = frame + number;
        std::string end = "_delay-0.07s.gif";
        frame = frame + end;
        std::string position = "res/shaders/Image/Explosion/";
        position = position + frame;
        GLint stub = make_texture(position);
        explosions_key_frames.insert({position, stub});
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
    GLuint stars = chicken3421::make_program(vert_shader2, frag_shader_white);
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
    GLint transform_loc3 = glGetUniformLocation(stars, "transform");
    GLuint texture1 = make_texture("res/shaders/Image/star.png");
    GLuint texture3 = make_texture("res/shaders/Image/x.png");
    createExplosionTextures();

    glfwSetWindowUserPointer(window, &ship);
    glfwSetKeyCallback(window, on_key_press);

    //If death star doesnt spawn change this number
    int current_buffer = INT_MAX;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float scaler_zoom = 0.0f;
    int explosion_key_frame = 0;
    bool explosion = false;

     // Render loop
    while(!glfwWindowShouldClose(window)) {
        float timeValue = glfwGetTime();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        usleep(1000);
        //Draw the stars (anything more than 100000 gives weird behaviour
        //if program doesn't work reduce the number of stars)
        glUseProgram(stars);
        glBindVertexArray(star.vao);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, 10000);
        float scale_variable = 1.0f + scaler_zoom;
        scaler_zoom = scaler_zoom + 0.0001f;
        std::cout << scaler_zoom << std::endl;
        glm::mat4 scaler = {
            scale_variable ,0 ,0 ,0, 
            0 , scale_variable ,0 ,0, 
            0 ,0 , scale_variable ,0, 
            0 ,0 ,0, 1, 
        }; 
        
        glUniformMatrix4fv(transform_loc3, 1, GL_FALSE, glm::value_ptr(scaler));

        ///Draw the sun
        glUseProgram(program);
        glBindVertexArray(circle.vao);


        glBindBuffer(GL_ARRAY_BUFFER, circle.vbo);

        glm::mat4 rotation = glm::mat4(1);
        rotation = glm::rotate(rotation, (float) glfwGetTime() / 20.0f, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(rotation));

        //Change the colour of the star overtime if in explosion mode    
        int vertexColorLocation = glGetUniformLocation(program, "ourColor");
        if (current_buffer != texture1) {
            float blueValue = sin(timeValue) / 2.0f + 0.8f;
            glUniform4f(vertexColorLocation, 1.0f, blueValue, 0.0f, 1.0f);
        } else {
            glUniform4f(vertexColorLocation, 0.8f, 0.8f, 0.8f, 1.0f);
        }

        if (((int)timeValue % 20) == 0) {
            if (current_buffer != texture1) {
                current_buffer = texture1;
                explosion = false;
            } else {
                explosion = true;
            }
            scaler_zoom = 0.0f;
            //To prevent flickering
            sleep(1);
        } 

        if (explosion) {
            current_buffer = loadExplosionKey(explosion_key_frame);
            explosion_key_frame += 1;
            if (current_buffer == INT_MAX) {
                current_buffer = 0;
                explosion_key_frame = 0;
            }
        }


        glBindTexture(GL_TEXTURE_2D, current_buffer);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circle.n_verts);


        //Rotate ship and translate
        glm::mat4 transform = ship.translate * ship.rotate; 
        
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
    glDeleteProgram(stars);
    glDeleteProgram(space_ship);

    glfwTerminate();
    return EXIT_SUCCESS;
}

