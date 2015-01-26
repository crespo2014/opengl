/*
 * main.cpp
 * apt-get install freeglut3 freeglut3-dev libghc-opengl-dev libglew-dev
 *
 *  Created on: 13 Nov 2014
 *      Author: lester
 */

// disable hardware acceleration to fix segmentation fault
// LIBGL_ALWAYS_INDIRECT=1
// source http://www.codeproject.com/Articles/182109/Setting-up-an-OpenGL-development-environment-in-Ub
#include "GL/freeglut.h"
#include "GL/gl.h"

#include <math.h>
#include <unistd.h>
#include <iostream>
#include "GraphBuffer.h"
#include "numeric_check.h"
#include "Edge.h"
#include "Triangles.h"
#include "utils.h"

#include <functional>

Edge    ed;

/* display function - code from:
 http://fly.cc.fer.hr/~unreal/theredbook/chapter01.html
 This is the actual usage of the OpenGL library.
 The following code is the same for any platform */
void renderFunction() {
    glLoadIdentity();

	static unsigned r = 0;
	glClearColor(0.5, 0.5, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	//ed.Draw();
	EquilateralTriangle(50).Draw();
	Edge(-10,100,-10,100).Draw();

//	glBegin(GL_TRIANGLES);
//	glColor3f(0.0, 0.0, 1.0);
//	glVertex2d(0,0);
//	glColor3f(1.0, 0.0, 0.0);
//	glVertex2d(10,0);
//	glColor3f(0.0, 1.0, 0.0);
//	glVertex2d(0,10);
//	glEnd();


	//glRotatef(r, 0.01, 1,0);
	++r %= 360;
	glBegin(GL_LINE_STRIP);
	for (unsigned i = 0; i < 100; ++i) {
		glVertex2f(i, 5*sin(i * 2 * 3.14 / 100));
	}
//        glVertex2f(1,1);
//        glVertex2f(0,1);
//        glVertex2f(1,0);
	glEnd();

	glFlush();
}

void timing(int v) {
	renderFunction();
	glutTimerFunc(1000 / 20, timing, 1);
}

void resized(int, int) {
	renderFunction();
}

/* Main method - main entry point of application
 the freeglut library does the window creation work for us,
 regardless of the platform. */
int main(int argc, char** argv) {

    const char blah[] = "LST";
    std::cout << path_to_name<blah>::value << std::endl;
    //std::cout << path_to_name<346>::value << std::endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL - First window demo");
	glutDisplayFunc(renderFunction);
	glutTimerFunc(1000 / 5, timing, 1);
	// Setup projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-50,100,-50,100);
	//glOrtho(-50, 50, -50, 50, -10, 10);
	//gluPerspective (50, 50/50 /*width/height*/, -10,10);
	//glViewport(0,0,200,200);   wich portion of the window is use by opengl
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	//glutIdleFunc(renderFunction);
	//glutReshapeFunc(resized);
	//glutPostRedisplay();
	glutMainLoop();
	return 0;
}
