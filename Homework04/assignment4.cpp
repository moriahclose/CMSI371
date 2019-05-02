/***
Assignment-4: Shading via Illumination and Colors

Name: Tolliver, Moriah

Collaborators: Doe, John; Doe, Jane
** Note: although the assignment should be completed individually
you may speak with classmates on high level algorithmic concepts. Please
list their names in this section

Project Summary: A short paragraph (3-4 sentences) describing the work you
did for the project.
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

/**************************************************
 *              Object Model Class                *
 *                                                *
 *  Stores the points of the object as a vector   *
 *  along with the colors and normals for each    *
 *  point. Normals are computed from the points.  *
 *                                                *
 *************************************************/
class ObjectModel {
    vector<GLfloat> _points;
    vector<GLfloat> _normals;
    vector<GLfloat> _base_colors;
    vector<GLfloat> _colors;
public:
    ObjectModel() { };
    vector<GLfloat> get_points() { return _points; };
    vector<GLfloat> get_normals() { return _normals; };
    vector<GLfloat> get_base_colors() { return _base_colors; };
    vector<GLfloat> get_colors() { return _colors; };
    void set_points(vector<GLfloat> points) { _points = points; };
    void set_normals(vector<GLfloat> normals) { _normals = normals; };
    void set_base_colors(vector<GLfloat> base_colors) { _base_colors = base_colors; };
    void set_colors(vector<GLfloat> colors) { _colors = colors; };
};

// The model of the scene
ObjectModel SCENE;

/**************************************************
 *              Utilitie Functions                *
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
        1.0,    0.0,    0.0,    dx,
        0.0,    1.0,    0.0,    dy,
        0.0,    0.0,    1.0,    dz,
        0.0,    0.0,    0.0,    1.0
    };
    return translate_mat;
}

// Definition of a scaling matrix
vector<GLfloat> scaling_matrix (float sx, float sy, float sz) {
    vector<GLfloat> scale_mat = {
        sx,     0.0,    0.0,    0.0,
        0.0,    sy,     0.0,    0.0,
        0.0,    0.0,    sz,     0.0,
        0.0,    0.0,    0.0,    1.0
    };
    return scale_mat;
}

// Definition of a rotation matrix about the x-axis theta degrees
vector<GLfloat> rotation_matrix_x (float theta) {
    theta = deg2rad(theta);
    vector<GLfloat> rotate_mat_x = {
        1.0,    0.0,                    0.0,                       0.0,
        0.0,    (float)(cos(theta)),    (float)(-sin(theta)),      0.0,
        0.0,    (float)(sin(theta)),    (float)(cos(theta)),       0.0,
        0.0,    0.0,                    0.0,                       1.0
    };
    return rotate_mat_x;
}


// Definition of a rotation matrix about the y-axis by theta degrees
vector<GLfloat> rotation_matrix_y (float theta) {
    theta = deg2rad(theta);
    vector<GLfloat> rotate_mat_y = {
        (float)cos(theta),     0.0,     (float)sin(theta),    0.0,
        0.0,                   1.0,     0.0,                  0.0,
        (float)-sin(theta),    0.0,     (float)cos(theta),    0.0,
        0.0,                   0.0,     0.0,                  1.0
    };
    return rotate_mat_y;
}


// Definition of a rotation matrix about the z-axis by theta degrees
vector<GLfloat> rotation_matrix_z (float theta) {
    theta = deg2rad(theta);
    vector<GLfloat> rotate_mat_z = {
        (float)cos(theta),  (float)-sin(theta), 0.0,    0.0,
        (float)sin(theta),  (float)cos(theta),  0.0,    0.0,
        0.0,                0.0,                1.0,    0.0,
        0.0,                0.0,                0.0,    1.0
    };
    return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<GLfloat> mat_mult(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> result;

    for (int b = 0; b < B.size()/4; b++) {
        for (int a = 0; a < 4; a++) {
            float element_wise_sum = 0.0;
            for (int k = 0; k < 4;  k++) {
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
    // Primitive plane
    vector<GLfloat> a0 = to_homogeneous_coord(init_plane());
    // Construct 6 planes of the cube
    vector<GLfloat> a1 = mat_mult(translation_matrix(0.0,  0.0,  0.5), a0);
    vector<GLfloat> a2 = mat_mult(translation_matrix(0.0,  0.0, -0.5), mat_mult(rotation_matrix_y(deg2rad(180)), a0));
    vector<GLfloat> a3 = mat_mult(translation_matrix(-0.5, 0.0,  0.0), mat_mult(rotation_matrix_y(deg2rad(-90)), a0));
    vector<GLfloat> a4 = mat_mult(translation_matrix(0.5,  0.0,  0.0), mat_mult(rotation_matrix_y(deg2rad(90)), a0));
    vector<GLfloat> a5 = mat_mult(translation_matrix(0.0,  0.5,  0.0), mat_mult(rotation_matrix_x(deg2rad(-90)), a0));
    vector<GLfloat> a6 = mat_mult(translation_matrix(0.0, -0.5,  0.0), mat_mult(rotation_matrix_x(deg2rad(90)), a0));

    result.insert(std::end(result), std::begin(a1), std::end(a1));
    result.insert(std::end(result), std::begin(a2), std::end(a2));
    result.insert(std::end(result), std::begin(a3), std::end(a3));
    result.insert(std::end(result), std::begin(a4), std::end(a4));
    result.insert(std::end(result), std::begin(a5), std::end(a5));
    result.insert(std::end(result), std::begin(a6), std::end(a6));

    return result;
}


/**************************************************
 *           Generating Surface Normals           *
 *                                                *
 *  Generate the surface normals of the objects   *
 *  using the cross product between two vectors   *
 *  that lie on the surface (plane) of interest.  *
 *  Recall that the direction of the normal to a  *
 *  surface follows the Right Hand Rule.          *
 *                                                *
 *************************************************/

// Performs the cross product between two vectors in cartesian coordinates
vector<GLfloat> cross_product(vector<GLfloat> A, vector<GLfloat> B) {
    vector<GLfloat> C;

	C[0] = A[1]*B[2] - A[2]*B[1]; //assign x
	C[1] = A[2]*B[0] - A[0]*B[2]; //assign y
	C[2] = A[0]*B[1] - A[1]*B[0]; //assign z


    return C;
}

// Generates the normals to each surface (plane) given cartesian set of points
vector<GLfloat> generate_normals(vector<GLfloat> points) {
    vector<GLfloat> normals;

    vector<GLfloat> a = { points[0] - points[9],
			  points[1] - points[10],
			  points[2] - points[11] };

    vector<GLfloat> b = { points[6] - points[9],
			  points[7] - points[10],
			  points[8] - points[11] };

    return cross_product(a, b);
}

/**************************************************
 *       Shading via Illumination and Color       *
 *                                                *
 *  Generate the set of colors for each face of   *
 *  the planes that compose the objects in the    *
 *  scene. Based on the light source and surface  *
 *  normals, render the colors of the objects by  *
 *  applying the shading equation.                *
 *                                                *
 *************************************************/

// Performs the dot product between two vectors
GLfloat dot_product(vector<GLfloat> A, vector<GLfloat> B) {

    return (A[0]*B[0]) + (A[1]*B[1]) + (A[2]*B[2]);
}

// Initializes the base color of a plane to a single color
vector<GLfloat> init_base_color(GLfloat r, GLfloat g, GLfloat b) {
    vector<GLfloat> base_color = {
        r,   g,   b,
        r,   g,   b,
        r,   g,   b,
        r,   g,   b
    };
    return base_color;
}

// Initializes the base color of a plane by specifying the color of each point
vector<GLfloat> init_base_color(GLfloat r0, GLfloat g0, GLfloat b0, GLfloat r1, GLfloat g1, GLfloat b1,
                                GLfloat r2, GLfloat g2, GLfloat b2, GLfloat r3, GLfloat g3, GLfloat b3) {
    // This enables OpenGL to use interpolation for (Gouraud) shading the plane
    vector<GLfloat> base_color = {
        r0,   g0,   b0,
        r1,   g1,   b1,
        r2,   g2,   b2,
        r3,   g3,   b3
    };
    return base_color;
}

// Normalizes a given vector
vector<GLfloat> normalize(vector<GLfloat> a) {
    GLfloat magnitude = sqrt(pow(a[0],2) + pow(a[1],2) + pow(a[2],2));

    for (int i = 0; i < a.size(); i++) {
        a[i] = a[i] / magnitude;
    }

    return a;
}

// Calculates h for light_source (a vector of 3 values) and viewer (a vector of 3 values)
vector<GLfloat> get_h(vector<GLfloat> light_source, vector<GLfloat> viewer) {
    vector<GLfloat> h;

    h[0] = light_source[0] + viewer[0];
    h[1] = light_source[1] + viewer[1];
    h[2] = light_source[2] + viewer[2];

    return normalize(h);
}

// Allows for ambience (a vector of 3 values), diffusion (vector of 3 values) and specular (vector of 3 values)
// as input to the shading equation
ObjectModel apply_shading(ObjectModel object_model, vector<GLfloat> light_source, vector<GLfloat> camera,
                          vector<GLfloat> amb, vector<GLfloat> diff, vector<GLfloat> spec) {
    vector<GLfloat> colors;

    GLfloat gloss = 0.5;

    object_model.set_points( to_cartesian_coord(object_model.get_points()) );

    for (int i = 0; i < points.size(); i++) {
        vector<GLfloat> light = {object_model.get_points()[i*3] - light_source[0],
                                object_model.get_points()[i*3 + 1] - light_source[1],
                                object_model.get_points()[i*3 + 2] - light_source[2]};

        light = normalize(light);

        vector<GLfloat> curr_normal = {object_model.get_normals()[i*3],
                                       object_model.get_normals()[i*3 + 1],
                                       object_model.get_normals()[i*3 + 2]};

        vector<GLfloat> h = {light[0] + camera[0],
                             light[1] + camera[1],
                             light[2] + camera[2]};

        h = normalize(h);

        GLfloat I_r = base_color[i*3 + 0] * (amb[0] + diff[0] * dot_product(curr_normal, light) + spec[0] * pow(dot_product(curr_normal, h), gloss ) );
        GLfloat I_g = base_color[i*3 + 1] * (amb[1] + diff[1] * dot_product(curr_normal, light) + spec[1] * pow(dot_product(curr_normal, h), gloss ) );
        GLfloat I_b = base_color[i*3 + 2] * (amb[2] + diff[2] * dot_product(curr_normal, light) + spec[2] * pow(dot_product(curr_normal, h), gloss ) );

        colors.push_back(I_r);
        colors.push_back(I_g);
        colors.push_back(I_b);
    }

    object_model.set_points( to_homogeneous_coord(object_model.get_points()) );
    object_model.set_colors(colors);
    return object_model;
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

/**************************************************
 *            Build Objects in Scene              *
 *                                                *
 *  methods that build furniture, decorations     *
 *  and other elements that appear in the scene   *
 *                                                *
 *************************************************/

// Walls that construct room
vector<GLfloat> build_room() {
	vector<GLfloat> room;

	vector<GLfloat> back_wall = mat_mult(scaling_matrix(4.0, 4.0, 0.1), build_cube());
    back_wall = mat_mult(translation_matrix(0.0, 0.0, -2.0), back_wall);

    vector<GLfloat> left_wall = mat_mult(scaling_matrix(0.1, 4.0, 3.95), build_cube());
    left_wall = mat_mult(translation_matrix(-2.0, 0.0, 0.0), left_wall);

    vector<GLfloat> floor = mat_mult(scaling_matrix(4.0, 0.1, 4.0), build_cube());
    floor = mat_mult(translation_matrix(0.0, -2.0, 0.0), floor);

	room = back_wall;
    room.insert(room.end(), left_wall.begin(), left_wall.end());

    room.insert(room.end(), floor.begin(), floor.end());

	return room;
}

// Construct desk
vector<GLfloat> build_desk() {
	vector<GLfloat> desk;

	vector<GLfloat> desk_leg_right = mat_mult(scaling_matrix(0.08, 1.6, 1.2), build_cube());
    desk_leg_right = mat_mult(translation_matrix(-0.2, -1.2, -1.4), desk_leg_right);

    vector<GLfloat> desk_leg_left = mat_mult(scaling_matrix(0.08, 1.6, 1.2), build_cube());
    desk_leg_left = mat_mult(translation_matrix(-1.80, -1.2, -1.4), desk_leg_left);

    vector<GLfloat> desk_top = mat_mult(scaling_matrix(1.6, 0.08, 1.2), build_cube());
    desk_top = mat_mult(translation_matrix(-1.0, -0.4, -1.4), desk_top);

    desk = desk_leg_right;
    desk.insert(desk.end(), desk_leg_left.begin(), desk_leg_left.end());
    desk.insert(desk.end(), desk_top.begin(), desk_top.end());

    return desk;
}


// Construct whiteboard
vector<GLfloat> build_whiteboard() {
	vector<GLfloat> whiteboard;

	vector<GLfloat> board_border = mat_mult(scaling_matrix(3.2, 2.0, 0.04), build_cube());
    board_border = mat_mult(translation_matrix(0.0, 0.98, -1.94), board_border);

    vector<GLfloat> board = mat_mult(scaling_matrix(3.0, 1.8, 0.04), build_cube());
    board = mat_mult(translation_matrix(0.0, 0.98, -1.90), board);

	whiteboard = board_border;
	whiteboard.insert(whiteboard.end(), board.begin(), board.end());

	return whiteboard;
}


// Construct monitor
vector<GLfloat> build_monitor() {
	vector<GLfloat> return_monitor;

	vector<GLfloat> monitor = mat_mult(scaling_matrix(0.8, 0.6, 0.02), build_cube());
    monitor = mat_mult(rotation_matrix_x(deg2rad(-10)), monitor);
    monitor = mat_mult(translation_matrix(-1.2, 0.3, -1.4), monitor);

    vector<GLfloat> stand = mat_mult(scaling_matrix(0.15, 0.4, 0.05), build_cube());
    stand = mat_mult(translation_matrix(-1.2, 0.0, -1.4), stand);

    vector<GLfloat> monitor_bottom = mat_mult(scaling_matrix(0.3, 0.04, 0.3), build_cube());
    monitor_bottom = mat_mult(translation_matrix(-1.2, -0.2, -1.4), monitor_bottom);

	return_monitor = monitor;
	return_monitor.insert(return_monitor.end(), stand.begin(), stand.end());
    	return_monitor.insert(return_monitor.end(), monitor_bottom.begin(), monitor_bottom.end());

	return return_monitor;
}

// Construct chair
vector<GLfloat> build_chair() {
	vector<GLfloat> chair;

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

	chair =  chair_back;
    chair.insert(chair.end(), chair_bottom.begin(), chair_bottom.end());
    chair.insert(chair.end(), chair_stand.begin(), chair_stand.end());
    chair.insert(chair.end(), chair_leg_back.begin(), chair_leg_back.end());
    chair.insert(chair.end(), chair_leg_right.begin(), chair_leg_right.end());
    chair.insert(chair.end(), chair_leg_left.begin(), chair_leg_left.end());
    chair.insert(chair.end(), chair_leg_front.begin(), chair_leg_front.end());

	return chair;
}


// Construct the scene using objects built from cubes/prisms
vector<GLfloat> init_scene() {
    vector<GLfloat> scene;

	scene = build_room();

	vector<GLfloat> whiteboard = build_whiteboard();
	vector<GLfloat> desk = build_desk();
	vector<GLfloat> monitor = build_monitor();
	vector<GLfloat>	chair = build_chair();

	scene.insert(scene.end(), whiteboard.begin(), whiteboard.end());
	scene.insert(scene.end(), desk.begin(), desk.end());
	scene.insert(scene.end(), monitor.begin(), monitor.end());
	scene.insert(scene.end(), chair.begin(), chair.end());

	scene = to_cartesian_coord(scene);
    return scene;
}

// Construct the color mapping of the scene
vector<GLfloat> init_color() {
    vector<GLfloat> colors;

    ObjectModel room = new ObjectModel();
    room.set_points(build_room());
    room.set_base_colors({1,0,0});

    colors.push_back( room.get_base_colors() );

    return colors;
}

void display_func() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     //TODO: Apply shading to the scene


     //TODO: Rotate the scene using the rotation matrix


    GLfloat* scene_vertices = vector2array(SCENE.get_points());
    GLfloat* color_vertices = vector2array(SCENE.get_colors());
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

    // Draw quad point planes: each 4 vertices with 3 dimensions
    glDrawArrays(GL_QUADS, 0, (int)SCENE.get_points().size() / 3);

    glFlush();			//Finish rendering
    glutSwapBuffers();

    // Clean up
    delete scene_vertices;
    delete color_vertices;
}

void idle_func() {
    THETA = THETA + 0.3;
    display_func();
}

int main (int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    // Create a window with rendering context and everything else we need
    glutCreateWindow("Moriah Tolliver Assignment 4");

    setup();
    init_camera();

    SCENE.set_points(init_scene());
    SCENE.set_base_colors(init_color());

    // Set up our display function
    glutDisplayFunc(display_func);
    glutIdleFunc(idle_func);
    // Render our world
    glutMainLoop();

    return 0;
}
