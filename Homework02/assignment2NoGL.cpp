/***
 Assignment-2: Rotating a Cube in 3-Dimensional Space

 Name: Tolliver, Moriah

 Collaborators: Doe, John; Doe, Jane
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section

 Project Summary: A short paragraph (3-4 sentences) describing the work you
 did for the project.
 ***/

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

// #ifdef __APPLE__
// #include <OpenGL/gl.h>
// #include <OpenGL/glu.h>
// #include <GLUT/glut.h>
// #else
// #include <GL/gl.h>
// #include <GL/glu.h>
// #include <GL/glut.h>
// #endif

#pragma GCC diagnostic pop

#include <math.h>
#include <vector>
#include <iostream>
using namespace std;


float theta = 0.0;

// Converts degrees to radians for rotation
float deg2rad(float d) {
    return (d*M_PI)/180.0;
}

// Converts a vector to an array
float* vector2array(vector<float> vec) {
    float* arr = new float[vec.size()];
    for (int i = 0; i < vec.size(); i++) {
        arr[i] = vec[i];
    }
    return arr;
}

void printVector( vector<float> v , int divider) {
    for ( int i = 0; i < v.size(); i++ ) {
        if ( i % divider == 0) {
            cout << endl;
        }
        cout << v[i] << " ";
    }
    cout << endl;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<float> to_homogenous_coord(vector<float> cartesian_coords) {
    vector<float> result;
    int resultIndex = 0;

    for ( int i = 0; i < cartesian_coords.size(); i++ ) {
        if ( (resultIndex+1) % 4 == 0 ) {
            result.push_back(1);
            resultIndex++;
            // cout << "push_back: " << result.back();
        }
        result.push_back( cartesian_coords[i] );
        resultIndex++;
    }

    //add 1 that would be out of bounds of original coordinates
    result.push_back(1);
    return result;
}

// Converts Cartesian coordinates to homogeneous coordinates
vector<float> to_cartesian_coord(vector<float> homogenous_coords) {
    vector<float> result;
    for ( int i = 0; i < homogenous_coords.size(); i++ ) {
        if ( (i+1) % 4 != 0 ) {
            result.push_back(homogenous_coords[i] );
        }
    }
    return result;
}

// Definition of a rotation matrix about the x-axis theta degrees
vector<float> rotation_matrix_x (float theta) {
    vector<float> rotate_mat_x;
    float thetaInRads = deg2rad( theta );

    rotate_mat_x.push_back(1);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(cos(thetaInRads) );
    rotate_mat_x.push_back( -1 * sin(thetaInRads) );
    rotate_mat_x.push_back(0);
    rotate_mat_x.push_back(sin(thetaInRads) );
    rotate_mat_x.push_back(cos(thetaInRads) );

    return rotate_mat_x;
}


// Definition of a rotation matrix along the y-axis by theta degrees
vector<float> rotation_matrix_y (float theta) {
    vector<float> rotate_mat_y;
    float thetaInRads = deg2rad( theta );

    rotate_mat_y.push_back(cos(thetaInRads));
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(sin(thetaInRads));
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(1);
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back( -1 * sin(thetaInRads) );
    rotate_mat_y.push_back(0);
    rotate_mat_y.push_back(cos(thetaInRads) );

    return rotate_mat_y;
}


// Definition of a rotation matrix along the z-axis by theta degrees
vector<float> rotation_matrix_z (float theta) {
    vector<float> rotate_mat_z;
    float thetaInRads = deg2rad( theta );

    rotate_mat_z.push_back(cos(thetaInRads));
    rotate_mat_z.push_back(-1 * sin(thetaInRads));
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(sin(thetaInRads));
    rotate_mat_z.push_back(cos(thetaInRads));
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(0);
    rotate_mat_z.push_back(1);

    // TODO: Define the rotation matrix about the z-axis

    return rotate_mat_z;
}

// Perform matrix multiplication for A B
vector<float> mat_mult(vector<float> A, vector<float> B) {
    vector<float> result;
    int aRows = floor(A.size()/4);
    int aCurrentRow = 0;
    int aIndex = 0;
    int bIndex = 0;

    while (aCurrentRow < aRows) {
        int element = 0;
        while ( bIndex < B.size() ) {
            element += A[aIndex]*B[bIndex];
            cout <<
            aIndex++;
            bIndex++;
            if ( (aIndex+1) % 4 == 0 ) {
                result.push_back(element);
                element = 0;
                aIndex = 0;
            }
        }
        aCurrentRow++;
    }

    return result;
}

void setup() {
    // Enable the vertex array functionality
    // glEnableClientState(GL_VERTEX_ARRAY);
    // // Enable the color array functionality (so we can specify a color for each vertex)
    // glEnableClientState(GL_COLOR_ARRAY);
    // // Enable depth test
    // glEnable(GL_DEPTH_TEST);
    // // Accept fragment if it closer to the camera than the former one
    // glDepthFunc(GL_LESS);
    // // Set up some default base color
    // glColor3f(0.5, 0.5, 0.5);
    // // Set up white background
    // glClearColor(1.0, 1.0, 1.0, 0.0);
}

void init_camera() {
    // // Camera parameters
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // // Define a 50 degree field of view, 1:1 aspect ratio, near and far planes at 3 and 7
    // gluPerspective(50.0, 1.0, 2.0, 10.0);
    // // Position camera at (2, 3, 5), attention at (0, 0, 0), up at (0, 1, 0)
    // gluLookAt(2.0, 6.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void display_func() {
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    // // World model parameters
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();

    vector<float> points = {
        // Front plane
        +1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        +1.0,   -1.0,   +1.0,
        // Back plane
        +1.0,   +1.0,   -1.0,
        -1.0,   +1.0,   -1.0,
        -1.0,   -1.0,   -1.0,
        +1.0,   -1.0,   -1.0,
        // Right
        +1.0,   +1.0,   -1.0,
        +1.0,   +1.0,   +1.0,
        +1.0,   -1.0,   +1.0,
        +1.0,   -1.0,   -1.0,
        // Left
        -1.0,   +1.0,   -1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   -1.0,
        // Top
        +1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   -1.0,
        +1.0,   +1.0,   -1.0,
        // Bottom
        +1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   -1.0,
        +1.0,   -1.0,   -1.0,
    };

    float colors[] = {
        // Front plane
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        1.0,    0.0,    0.0,
        // Back plane
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        0.0,    1.0,    0.0,
        // Right
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        0.0,    0.0,    1.0,
        // Left
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        1.0,    1.0,    0.0,
        // Top
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        1.0,    0.0,    1.0,
        // Bottom
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
        0.0,    1.0,    1.0,
    };

    // TODO: Apply rotation(s) to the set of points


    float* vertices = vector2array(points);

    // glVertexPointer(3,          // 3 components (x, y, z)
    //                 GL_FLOAT,   // Vertex type is GL_FLOAT
    //                 0,          // Start position in referenced memory
    //                 vertices);  // Pointer to memory location to read from
    //
    // //pass the color pointer
    // glColorPointer(3,           // 3 components (r, g, b)
    //                GL_FLOAT,    // Vertex type is GL_FLOAT
    //                0,           // Start position in referenced memory
    //                colors);     // Pointer to memory location to read from
    //
    // // Draw quad point planes: each 4 vertices
    // glDrawArrays(GL_QUADS, 0, 4*6);
    //
    // glFlush();            //Finish rendering
    // glutSwapBuffers();
}

void idle_func() {
    theta = theta+0.3;
    display_func();
}

int main (int argc, char **argv) {
    vector<float> points = {
        // Front plane
        +1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        +1.0,   -1.0,   +1.0,
        // Back plane
        +1.0,   +1.0,   -1.0,
        -1.0,   +1.0,   -1.0,
        -1.0,   -1.0,   -1.0,
        +1.0,   -1.0,   -1.0,
        // Right
        +1.0,   +1.0,   -1.0,
        +1.0,   +1.0,   +1.0,
        +1.0,   -1.0,   +1.0,
        +1.0,   -1.0,   -1.0,
        // Left
        -1.0,   +1.0,   -1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   -1.0,
        // Top
        +1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   +1.0,
        -1.0,   +1.0,   -1.0,
        +1.0,   +1.0,   -1.0,
        // Bottom
        +1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   +1.0,
        -1.0,   -1.0,   -1.0,
        +1.0,   -1.0,   -1.0,
    };
    // vector<float> homs = to_homogenous_coord(points);
    // printVector(homs,4);
    // cout << "Array size: " << homs.size();
    // cout << endl;
    //
    // vector<float> carts = to_cartesian_coord(homs);
    // printVector(carts,3);
    // cout << "Array size: " << carts.size();
    // cout << endl;

    vector<float> A;
    A.push_back(1);
    A.push_back(2);
    A.push_back(3);
    A.push_back(4);
    A.push_back(5);
    A.push_back(6);
    A.push_back(7);
    A.push_back(8);

    vector<float> B;
    B.push_back(1);
    B.push_back(2);
    B.push_back(3);
    B.push_back(4);
    B.push_back(5);
    B.push_back(6);
    B.push_back(7);
    B.push_back(8);

    vector<float> mult = mat_mult(A,B);
    printVector(mult,4);
    // printVector(mult,4);

    // Initialize GLUT
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//    glutInitWindowSize(800, 600);
//    // Create a window with rendering context and everything else we need
//    glutCreateWindow("Assignment 2");
//
//    setup();
//    init_camera();
//
//    // Set up our display function
//    glutDisplayFunc(display_func);
//    glutIdleFunc(idle_func);
//    // Render our world
//    glutMainLoop();
    return 0;
}
