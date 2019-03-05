#include <math.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

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
// vector<GLfloat> to_cartesian_coord(vector<GLfloat> homogenous_coords) {
//     vector<GLfloat> result;
//
//     // TODO: Remove the 1 in the 4th dimension to generate Cartesian coordinates
//
//     return result;
// }
//
// // Definition of a rotation matrix about the x-axis theta degrees
// vector<GLfloat> rotation_matrix_x (float theta) {
//     vector<GLfloat> rotate_mat_x;
//
//     // TODO: Define the rotation matrix about the x-axis
//
//     return rotate_mat_x;
// }
//
//
// // Definition of a rotation matrix along the y-axis by theta degrees
// vector<GLfloat> rotation_matrix_y (float theta) {
//     vector<GLfloat> rotate_mat_y;
//
//     // TODO: Define the rotation matrix about the y-axis
//
//     return rotate_mat_y;
// }
//
//
// // Definition of a rotation matrix along the z-axis by theta degrees
// vector<GLfloat> rotation_matrix_z (float theta) {
//     vector<GLfloat> rotate_mat_z;
//
//     // TODO: Define the rotation matrix about the z-axis
//
//     return rotate_mat_z;
// }
//
// Perform matrix multiplication for A B
vector<float> mat_mult(vector<float> A, vector<float> B) {
    vector<float> result;

    // TODO: Compute matrix multiplication of A B
    // for ( int )

    return result;
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

    vector<float> myCoordsA;
    myCoordsA.push_back(1);
    myCoordsA.push_back(2);
    myCoordsA.push_back(3);

    vector<float> myCoordsB;
    myCoordsB.push_back(1);
    myCoordsB.push_back(2);
    myCoordsB.push_back(3);

    vector<float> myCoordsC = to_homogenous_coord(points);
    // cout << points.size() << " " << myCoordsC.size() << " ";
    for (int i = 0; i < myCoordsC.size(); i++) {
        cout << myCoordsC[i] << " ";
        if ((i+1) % 4 == 0) {
            cout << "|";
        }
    }
}
