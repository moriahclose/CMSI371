/***
 Assignment-3: Geometric Modeling of a Scene
 
 Name: Tolliver, Moriah 
 
 Project Summary: First, I created the build_cube() function to create a unit cube cetnered at (0,0,0). Then, I transformed the cube to make different shapes within the scene. The resulting scene is an office space located in Seaver rotating about the y-axis.

 To run:
 g++ -o assignment3 assignment3.cpp -std=c++14 -lGL -lGLU -lglut && ./   assignment3
 ***/


#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#pragma GCC diagnostic pop

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
using namespace std;

// If a float is < EPSILON or > -EPILSON then it should be 0
float EPSILON = 0.000001;
// theta is the angle to rotate the scene
float THETA = 0.0;
// Vector placeholders for the scene and color array
vector<GLfloat> SCENE;
vector<GLfloat> COLOR;

/**************************************************
 *  Rectangular Prisms via Hierarchical Modeling  *
 *                                                *
 *  using planes as building blocks, build a unit *
 *  cube with transformations that will serve as  *
 *  a primitive for modeling objects in the scene *
 *                                                *
 *************************************************/

// Initializes a square plane of unit lengths
vector<GLfloat> init_plane() {
    vector<GLfloat> vertices = {
        +0.5,   +0.5,   +0.0,
        -0.5,   +0.5,   +0.0,
        -0.5,   -0.5,   +0.0,
        +0.5,   -0.5,   +0.0
    };
    return vertices;
}

// Converts degrees to radians for rotation
float deg2rad(float d) {
    return (d*M_PI) / 180.0;
}

// Converts a vector to an array
GLfloat* vector2array(vector<GLfloat> vec) {
    GLfloat* arr = new GLfloat[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_homogeneous_coord(vector<GLfloat> cartesian_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < cartesian_coords.size(); i++) {
        result.push_back(cartesian_coords[i]);
        if ((i+1) % 3 == 0) {
            result.push_back(1.0);
        }
    }
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogeneous_coords) {
    vector<GLfloat> result;
    for (int i = 0; i < homogeneous_coords.size(); i++) {
        if ((i+1) % 4 == 0) {
            continue;
        } else {
            result.push_back(homogeneous_coords[i]);
        }
    }
    return result;
}

// Definition of a translation matrix
vector<GLfloat> translation_matrix (float dx, float dy, float dz) {
    vector<GLfloat> translate_mat = {
	1, 0, 0, dx,
	0, 1, 0, dy,
	0, 0, 1, dz,
	0, 0, 0, 1
    };
    
    return translate_mat;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix (float sx, float sy, float sz) {
    vector<GLfloat> scale_mat = {
	sx, 0, 0, 0,
	0, sy, 0, 0,
	0, 0, sz, 0,
	0, 0, 0, 1
    };
    
    return scale_mat;
}

// Definition of a rotation matrix about the x-axis theta degrees
vector<GLfloat> rotation_matrix_x (float theta) {
    vector<GLfloat> rotate_mat_x = {
	1, 0, 0, 0,
	0, cos(theta), -1*sin(theta), 0,
	0, sin(theta), cos(theta), 0,
	0, 0, 0, 1
    };
    
    return rotate_mat_x;
}


// Definition of a rotation matrix about the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    vector<GLfloat> rotate_mat_y = {
	cos(theta), 0, sin(theta), 0,
	0, 1, 0, 0,
	-1*sin(theta), 0, cos(theta), 0,
	0, 0, 0, 1
    };
    
    return rotate_mat_y;
}


// Definition of a rotation matrix about the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    vector<GLfloat> rotate_mat_z = {
	cos(theta), -1*sin(theta), 0, 0,
	sin(theta), cos(theta), 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
    };
    
    return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;

    for (int b = 0; b < B.size()/4; b++) {
        for (int a = 0; a < 4; a++) {
            float element_wise_sum = 0.0;
            for (int k = 0; k < 4; k++) {
                float element_wise = A[a*4+k]*B[b*4+k];
                if (element_wise < EPSILON && element_wise > -1.0*EPSILON) {
                    element_wise = 0.0;
                }
                element_wise_sum += element_wise;
            }
            result.push_back(element_wise_sum);
        }
    }
    return result;
}

// Builds a unit cube centered at the origin
vector<GLfloat> build_cube() {
    vector<GLfloat> result;
    vector<GLfloat> plane = to_homogeneous_coord(init_plane());
    
    vector<GLfloat> front = mat_mult(translation_matrix(0.0,0.0,0.5), plane);
    vector<GLfloat> left = mat_mult(rotation_matrix_y(deg2rad(-90)), plane);
    left = mat_mult(translation_matrix(-0.5,0.0,0.0), left);
    vector<GLfloat> right = mat_mult(rotation_matrix_y(deg2rad(90)), plane);
    right = mat_mult(translation_matrix(0.5,0.0,0.0), right);
    vector<GLfloat> back = mat_mult(rotation_matrix_y(deg2rad(180)), plane);
    back = mat_mult(translation_matrix(0.0,0.0,-0.5), back);
    vector<GLfloat> top = mat_mult(rotation_matrix_x(deg2rad(-90)), plane);
    top = mat_mult(translation_matrix(0.0,0.5,0.0), top);
    vector<GLfloat> bottom = mat_mult(rotation_matrix_x(deg2rad(90)), plane);
    bottom = mat_mult(translation_matrix(0.0,-0.5,0.0), bottom);

    result = front;
    result.insert(result.end(), left.begin(), left.end());
    result.insert(result.end(), right.begin(), right.end());
    result.insert(result.end(), back.begin(), back.end());
    result.insert(result.end(), top.begin(), top.end());
    result.insert(result.end(), bottom.begin(), bottom.end());
    
    return result;
}

/**************************************************
 *            Camera and World Modeling           *
 *                                                *
 *  create a scene by applying transformations to *
 *  the objects built from planes and position    *
 *  the camera to view the scene by setting       *
 *  the projection/viewing matrices               *
 *                                                *
 *************************************************/

void setup() {
    // Enable the vertex array functionality
    glEnableClientState(GL_VERTEX_ARRAY);
    // Enable the color array functionality (so we can specify a color for each vertex)
    glEnableClientState(GL_COLOR_ARRAY);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Set up some default base color
    glColor3f(0.5, 0.5, 0.5);
    // Set up white background
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

void init_camera() {
    // Camera parameters
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 7
    gluPerspective(50.0, 1.0, 2.0, 10.0);
    // Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
    gluLookAt(2.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
}

// Construct the scene using objects built from cubes/prisms
vector<GLfloat> init_scene() {
    vector<GLfloat> scene;

    //walls
    vector<GLfloat> back_wall = mat_mult(scaling_matrix(4.0, 4.0, 0.1), build_cube());
    back_wall = mat_mult(translation_matrix(0.0, 0.0, -2.0), back_wall);

    vector<GLfloat> left_wall = mat_mult(scaling_matrix(0.1, 4.0, 3.95), build_cube());
    left_wall = mat_mult(translation_matrix(-2.0, 0.0, 0.0), left_wall);

    vector<GLfloat> floor = mat_mult(scaling_matrix(4.0, 0.1, 4.0), build_cube());
    floor = mat_mult(translation_matrix(0.0, -2.0, 0.0), floor);

    //white board
    vector<GLfloat> board_border = mat_mult(scaling_matrix(3.2, 2.0, 0.04), build_cube());
    board_border = mat_mult(translation_matrix(0.0, 0.98, -1.94), board_border);
    
    vector<GLfloat> board = mat_mult(scaling_matrix(3.0, 1.8, 0.04), build_cube());
    board = mat_mult(translation_matrix(0.0, 0.98, -1.90), board);

    //desk
    vector<GLfloat> desk_leg_right = mat_mult(scaling_matrix(0.08, 1.6, 1.2), build_cube());
    desk_leg_right = mat_mult(translation_matrix(-0.2, -1.2, -1.4), desk_leg_right);

    vector<GLfloat> desk_leg_left = mat_mult(scaling_matrix(0.08, 1.6, 1.2), build_cube());
    desk_leg_left = mat_mult(translation_matrix(-1.80, -1.2, -1.4), desk_leg_left);

    vector<GLfloat> desk_top = mat_mult(scaling_matrix(1.6, 0.08, 1.2), build_cube());
    desk_top = mat_mult(translation_matrix(-1.0, -0.4, -1.4), desk_top);

    //monitor
    vector<GLfloat> monitor = mat_mult(scaling_matrix(0.8, 0.6, 0.02), build_cube());
    monitor = mat_mult(rotation_matrix_x(deg2rad(-10)), monitor);
    monitor = mat_mult(translation_matrix(-1.2, 0.3, -1.4), monitor);

    vector<GLfloat> stand = mat_mult(scaling_matrix(0.15, 0.4, 0.05), build_cube());
    stand = mat_mult(translation_matrix(-1.2, 0.0, -1.4), stand);

    vector<GLfloat> monitor_bottom = mat_mult(scaling_matrix(0.3, 0.04, 0.3), build_cube());
    monitor_bottom = mat_mult(translation_matrix(-1.2, -0.2, -1.4), monitor_bottom);

    //chair
    vector<GLfloat> chair_back = mat_mult(scaling_matrix(0.6, 1.2, 0.09), build_cube());
    chair_back = mat_mult(translation_matrix(-1.0, -0.6, 0.0), chair_back);

    vector<GLfloat> chair_bottom = mat_mult(scaling_matrix(0.6, 0.09, 0.6), build_cube());
    chair_bottom = mat_mult(translation_matrix(-1.0, -1.2, -0.3), chair_bottom);

    vector<GLfloat> chair_stand = mat_mult(scaling_matrix(0.075, 1.0, 0.075), build_cube());
    chair_stand = mat_mult(translation_matrix(-1.0, -1.68, -0.2), chair_stand); 

    vector<GLfloat> chair_leg_back = mat_mult(scaling_matrix(0.075, 0.075, 0.6), build_cube());
    chair_leg_back = mat_mult(translation_matrix(-1.0, -1.9, -0.4), chair_leg_back);

    vector<GLfloat> chair_leg_right = mat_mult(scaling_matrix(0.6, 0.075, 0.075), build_cube());
    chair_leg_right = mat_mult(translation_matrix(-0.8, -1.9, -0.2), chair_leg_right);

    vector<GLfloat> chair_leg_front = mat_mult(scaling_matrix(0.075, 0.075, 0.6), build_cube());
    chair_leg_front = mat_mult(translation_matrix(-1.0, -1.9, 0.0), chair_leg_front);

    vector<GLfloat> chair_leg_left = mat_mult(scaling_matrix(0.6, 0.075, 0.075), build_cube());
    chair_leg_left = mat_mult(translation_matrix(-1.2, -1.9, -0.2), chair_leg_left);

    scene = back_wall;
    scene.insert(scene.end(), left_wall.begin(), left_wall.end());
    
    scene.insert(scene.end(), floor.begin(), floor.end());

    scene.insert(scene.end(), board_border.begin(), board_border.end());
    scene.insert(scene.end(), board.begin(), board.end());

    scene.insert(scene.end(), desk_leg_right.begin(), desk_leg_right.end());
    scene.insert(scene.end(), desk_leg_left.begin(), desk_leg_left.end());
    scene.insert(scene.end(), desk_top.begin(), desk_top.end());

    scene.insert(scene.end(), monitor.begin(), monitor.end());
    scene.insert(scene.end(), stand.begin(), stand.end());
    scene.insert(scene.end(), monitor_bottom.begin(), monitor_bottom.end());

    scene.insert(scene.end(), chair_back.begin(), chair_back.end());
    scene.insert(scene.end(), chair_bottom.begin(), chair_bottom.end());
    scene.insert(scene.end(), chair_stand.begin(), chair_stand.end());
    scene.insert(scene.end(), chair_leg_back.begin(), chair_leg_back.end());
    scene.insert(scene.end(), chair_leg_right.begin(), chair_leg_right.end());
    scene.insert(scene.end(), chair_leg_left.begin(), chair_leg_left.end());
    scene.insert(scene.end(), chair_leg_front.begin(), chair_leg_front.end());
    //scene.insert(scene.end(), back_wheel.begin(), back_wheel.end());
    scene = to_cartesian_coord(scene);
    return scene;
}

// Construct the color mapping of the scene
vector<GLfloat> init_color(vector<GLfloat> scene) {
    vector<GLfloat> colors;
    for (int i = 0; i < scene.size(); i++) {
        colors.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
    }
    return colors;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    vector<GLfloat> homogeneous_points = to_homogeneous_coord(SCENE);
    vector<GLfloat> rotate_y = rotation_matrix_y(deg2rad(THETA));

    vector<GLfloat> rotated_points_y = mat_mult(rotate_y, homogeneous_points);

    SCENE = to_cartesian_coord(rotated_points_y);
    
    GLfloat* scene_vertices = vector2array(SCENE);
    GLfloat* color_vertices = vector2array(COLOR);
    // Pass the scene vertex pointer
    glVertexPointer(3,                // 3 components (x, y, z)
                    GL_FLOAT,         // Vertex type is GL_FLOAT
                    0,                // Start position in referenced memory
                    scene_vertices);  // Pointer to memory location to read from
    
    // Pass the color vertex pointer
    glColorPointer(3,                   // 3 components (r, g, b)
                   GL_FLOAT,            // Vertex type is GL_FLOAT
                   0,                   // Start position in referenced memory
                   color_vertices);     // Pointer to memory location to read from
    
    // Draw quad point planes: each 4 vertices
    glDrawArrays(GL_QUADS, 0, 4 * SCENE.size()/12);
    
    glFlush();			//Finish rendering
    glutSwapBuffers();
}

void idle_func() {
    THETA = -0.03;
    display_func();
}

int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Moriah Assignment 3");
    
    setup();
    init_camera();
    // Setting global variables SCENE and COLOR with actual values
    SCENE = init_scene();
    COLOR = init_color(SCENE);
    
    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();
    
    // Remember to call "delete" on your dynmically allocated arrays
    // such that you don't suffer from memory leaks. e.g.
    // delete arr;
    
    return 0;
}
