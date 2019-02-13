/***
 assignment1.cpp
 Assignment-1: Cartoonify
 
 Name: Tolliver, Moriah (Please write your name in Last Name, First Name format)
 
 Collaborators: Doe, John; Doe, Jane
 ** Note: although the assignment should be completed individually
 you may speak with classmates on high level algorithmic concepts. Please
 list their names in this section
 
 Project Summary: A short paragraph (3-4 sentences) describing the work you
 did for the project: e.g. did you use the Chaikin's or Bezier's algorithm?
 Did you take an iterative or recursive approach?
 ***/


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
        float x = 0.5 * (points.at(i).get_x() + points.at(i+1).get_x());
        float y = 0.5 * (points.at(i).get_y() + points.at(i+1).get_y());
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
        Vertex(-.1f,-.9f)
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
        Vertex(-.357f,-.006f)
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
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set our color to black (R, G, B)
    glColor3f(0.0f, 0.0f, 0.0f);
    
    draw_curve(hair, 20);
    draw_curve(face, 1);
    draw_curve(left_eyebrow, 20);
    draw_curve(right_eyebrow, 20);


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

