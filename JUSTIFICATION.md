# Assignment 1 Justification

If you think any of the questions within the sections are not applicable, please write "N/A".

## Section 1: It runs

### How have you fulfilled this requirement?

Runs on: Windows 10/11 64x using the setup provided in the specifications

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement? 

A x-wing in the center that can be mvoed by q,w,e,a,s,d. 
The death star in the background should explode after every 15 seconds. 
The explosions should be animated and glows from dark red to light orange. 
The stars also move by increasing its scale. 


### Where in the code should tutors look to see that you've fulfilled this requirement?

Circle, main, ship, star structs and files. Render loop of main function. Changing of textures in render loop.


## Section 2: Visibility & Shaders

### How have you fulfilled this requirement?

Images appear on screen. Explosion colour changes. White shader for stars. Death star has grey shader.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

The ship, space, stars and death star. Shaders loaded in to explosion where it changes colour. Stars have white shader from shade2.glsl.

### Where in the code should tutors look to see that you've fulfilled this requirement?

Main function. create explosion textures and load explosion key functions. Shader1 shader2 shader3 glsl files.
Uniform ourcolour is used to give the explosion a glowing effect in shader2.glsl.
GL_BLEND used to make rectangle transparent from the texture.

## Section 3: Creation and Use of 2D Shapes


### How have you fulfilled this requirement?

2 rectangles to hold the spaceship.
Circle for the death star and explosion.
Dots for the stars.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

Images on screen. Stars, death star and xwing. X wing is transparent so rectangle should not be seen. 

### Where in the code should tutors look to see that you've fulfilled this requirement?

stars, ship and circle files.
shader1, shader2, shader3 glsl files.
main.hpp header file for declaration of structs.
In render loop GL_BLEND is used to make the rectangle transparent for the x wing.

## Section 4: Use of Textures

### How have you fulfilled this requirement?

in main.cpp. Before the render loop. Load explosion key and create explosion textures functions.
Explosion textures are loaded in a hashmap to prevent remaking the same texture.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

After 15 seconds the death star should change to an explosion. The explosion should be animated. After 15 seconds the explosion will go back to the death star texture.
Circle is loaded with death star. Texture loaded in for x wing.

### Where in the code should tutors look to see that you've fulfilled this requirement?

Render loop in main and initialisation. Textures loaded before the render loop. Load explosion key function to create the animation
by swapping the textures.
vert1 vert2 vert3 and shader1 shader2 shader3 to declare the the textures. 
3 different textures were used due to the 3 objects.
Circle and ship files have texture coordinates and loaded accordingly into vbo. 

### Section 5: Subjective Mark

### How have you fulfilled this requirement?

Uses points and blending and changing of colours. Death star and explosion to emulate the star wars a new hope when luke destroys death star.
Explosion is animated. Blends in to background. Stars move giving a "hyperspeed" visual.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

Stars, death star, explosion changing colours. Star moving towards. Death star rotating.

### Where in the code should tutors look to see that you've fulfilled this requirement?

Main function, stars, circle, ship files. Loadexplosion function to create the animation.

## Use of External Libraries

### What does the library do?

Write your answer here...

### Why did you decide to use it?

Write your answer here...