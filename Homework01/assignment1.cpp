/***
 assignment1.cpp
 Assignment-1: Cartoonify
 
 Name: Tolliver, Moriah
 
 Collaborators: Patterson, Josh
 
 Project Summary: This project uses Chaikin's algorithm to cartoonify a picture of Bob Ross. I used Desmos, an online graphing calculator, to find the control points (https://www.desmos.com/calculator, DOA: 2/13/2019). I took an iterative approach, in which the algorithm is repeated according to iteration number specified in the display method.
*/

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <vector>
#include <iostream>
using namespace std;

class Vertex {
    GLfloat x, y;
public:
    Vertex(GLfloat, GLfloat);
    GLfloat get_y() { return y; };
    GLfloat get_x() { return x; };
};

Vertex::Vertex(GLfloat X, GLfloat Y) {
    x = X;
    y = Y;
}

void setup() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

vector<Vertex> generate_points(vector<Vertex> control_points) {
    vector<Vertex> points;
    points = control_points;
    vector<Vertex> midpoints;
    Vertex initial_point = points.front();
    Vertex end_point = points.back();

    for ( int i =0; i < points.size()-1; i++ ) {
        float x = (0.25 * points.at(i).get_x()) + (0.75 * points.at(i+1).get_x());
        float y = (0.25 * points.at(i).get_y()) + (0.75 * points.at(i+1).get_y());
        float x2 = (0.75 * points.at(i).get_x()) + (0.25 * points.at(i+1).get_x());
        float y2 = (0.75 * points.at(i).get_y()) + (0.25 * points.at(i+1).get_y());
        midpoints.push_back(Vertex(x2,y2));
        midpoints.push_back(Vertex(x,y));
    }
    
    points = midpoints;
    points.insert(points.begin(), initial_point);
    points.insert(points.end(), end_point);

    return points;
}

void draw_curve(vector<Vertex> control_points, int n_iter) {
    vector<Vertex> points;
    points = control_points;
    
    for ( int i =0; i < n_iter; i++ ) {
        points = generate_points(points);
    }
    
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    for (int i =0; i < points.size()-1; i++) {
        glVertex2f(points.at(i).get_x(), points.at(i).get_y());
        glVertex2f(points.at(i+1).get_x(), points.at(i+1).get_y());
    }
    glEnd();
    
}

void display() {
    vector<Vertex> hair =
    {Vertex(-.095f,-.905f),
        Vertex(-.2f,-.8f),
        Vertex(-.3f,-.7f),
        Vertex(-.4f,-.6f),
        Vertex(-.55f,-.5f),
        Vertex(-.65f,-.35f),
        Vertex(-.75f,-.2f),
        Vertex(-.8f,0),
        Vertex(-.8f,.2f),
        Vertex(-.8f,.35f),
        Vertex(-.75f,.55f),
        Vertex(-.6f,.7f),
        Vertex(-.3f,.9f),
        Vertex(.25f,.98f),
        Vertex(.5f,.9f),
        Vertex(.7f,.8f),
        Vertex(.904f,.507f),
        Vertex(.97f,.3f),
        Vertex(.95f,0),
        Vertex(.9f,-.2f),
        Vertex(.85f,-.4f),
        Vertex(.7f,-.5f),
        Vertex(.6f,-.45f),
        Vertex(.6f,-.55f),
        Vertex(.5f,-.7f),
        Vertex(.35f, -.85f),
        Vertex(.25f,-.95f),
        Vertex(.1f,-.95f),
        Vertex(-.1f,-.9f),
    };
    
    vector<Vertex> face = {
        Vertex(-.34f,.02f),
        Vertex(-.226f,.164f),
        Vertex(.27f,.17f),
        Vertex(.523f,.077f),
        Vertex(.365f,-.375f),
        Vertex(.16f,-.34f),
        Vertex(.067f,-.383f),
        Vertex(.004f,-.33f),
        Vertex(-.206f,-.506f),
        Vertex(-.385f,-.244f),
        Vertex(-.357f,-.006f),
        Vertex(-.34f,.02f)
    };
    
    vector<Vertex> left_eyebrow = {
        Vertex(-.315f,-.035f),
        Vertex(-.23f,.028f),
        Vertex(-.11f,.04f),
        Vertex(-.015f,.016f)
    };
    
    vector<Vertex> right_eyebrow = {
        Vertex(.18f,.063f),
        Vertex(.317f,.086f),
        Vertex(.402f,.08f),
        Vertex(.477f,.043f)
    };
    
    vector<Vertex> right_eye = {
        Vertex(.17f,-.042f),
        Vertex(.22f,.004f),
        Vertex(.285f,.02f),
        Vertex(.37f,-.003f),
        Vertex(.313f,-.04f),
        Vertex(.24f,-.054f),
        Vertex(.17f, -.04f)
    };
    
    vector<Vertex> left_eye = {
        Vertex(-.24f,-.078f),
        Vertex(-.187f,-.023f),
        Vertex(-.1f,-.02f),
        Vertex(-.03f,-.06f),
        Vertex(-.116f,-.08f),
        Vertex(-.175f, -.085f),
        Vertex(-.24f,-.078f)
    };
    
    vector<Vertex> left_left_pupil = {
        Vertex(-.187f, -.023f),
        Vertex(-.175f, -.085f)
    };
    
    vector<Vertex> right_left_pupil = {
        Vertex(-.1f, -.02f),
        Vertex(-.116f, -.08f)
    };
    
    vector<Vertex> left_right_pupil = {
        Vertex(.22f, .004f),
        Vertex(.24f, -.054f)
    };
    
    vector<Vertex> right_right_pupil = {
        Vertex(.305, .016f),
        Vertex(.284f, -.051f)
    };
    
    vector<Vertex> upper_lip = {
        Vertex(-.128f,-.48f),
        Vertex(-.003f,-.436f),
        Vertex(.106f,-.46f),
        Vertex(.176f,-.43f),
        Vertex(.332f, -.436f)
    };
    
    vector<Vertex> lower_lip_top = {
        Vertex(-.128f,-.48f),
        Vertex(0.0f, -.53f),
        Vertex(.11f, -.538f),
        Vertex(.24f, -.51f),
        Vertex(.332f, -.436f)
    };
    
    vector<Vertex> lower_lip_bottom = {
        Vertex(-.128f,-.48f),
        Vertex(-.066f, -.58f),
        Vertex(0.028f, -.616f),
        Vertex(.19f, -.61f),
        Vertex(.305f, -.522f),
        Vertex(.332f, -.436f)
    };
    
    vector<Vertex> left_nose = {
        Vertex(-.015f,-.167f),
        Vertex(-.05f, -.23f),
        Vertex(-.093f,-.257f),
        Vertex(-.073f,-.34f),
        Vertex(.004f, -.33f)
    };
    
    vector<Vertex> right_nose = {
        Vertex(.16f, -.34f),
        Vertex(.23f,-.304f),
        Vertex(.23f,-.234f),
        Vertex(.18f, -.206f),
        Vertex(.153f,-.148f),
    };
    
    vector<Vertex> nose_bottom = {
        Vertex(.004f, -.33f),
        Vertex(0.067f,-.383f),
        Vertex(.16f, -.34f)
    };
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set our color to black (R, G, B)
    glColor3f(0.0f, 0.0f, 0.0f);
    
    int n_iter = 5;
    draw_curve(hair, n_iter);
    draw_curve(face, n_iter);
    draw_curve(left_eyebrow, n_iter);
    draw_curve(right_eyebrow, n_iter);
    draw_curve(left_eye, n_iter);
    draw_curve(right_eye, n_iter);
    draw_curve(upper_lip, n_iter);
    draw_curve(lower_lip_top, n_iter);
    draw_curve(lower_lip_bottom, n_iter);
    draw_curve(left_nose, n_iter);
    draw_curve(right_nose, n_iter);
    draw_curve(nose_bottom, n_iter);
    draw_curve(left_left_pupil, n_iter);
    draw_curve(right_left_pupil, n_iter);
    draw_curve(left_right_pupil, n_iter);
    draw_curve(right_right_pupil, n_iter);

    glutSwapBuffers();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800,600); // Set your own window size
    glutCreateWindow("Moriah Tolliver Assignment 1");
    setup();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
