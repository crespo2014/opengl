/*
 * main.cpp
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


void DrawEdges() {
	const static float mark_size = 0.2;
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2f(0, -mark_size);
	glVertex2f(0, 100);
	glVertex2f(-mark_size, 0);
	glVertex2f(100, 0);

	for (unsigned i = 0; i < 100; i += 10) {
		glVertex2f(i,-mark_size);
		glVertex2f(i, mark_size);
		glVertex2f(-mark_size,i);
		glVertex2f(mark_size,i);
	}

	glColor3f(0.0, 1.0, 0.0);

	glVertex3i(0, 0,0);
	glVertex3i(0,0,100);
	glEnd();
}


/* display function - code from:
 http://fly.cc.fer.hr/~unreal/theredbook/chapter01.html
 This is the actual usage of the OpenGL library.
 The following code is the same for any platform */
void renderFunction() {
	glPushMatrix();
	static unsigned r = 0;
	glClearColor(0.5, 0.5, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glOrtho(-100.0, 100.0, -10.0, 10.0, -100.0, 100.0);
	glRotatef(r, 0.01, 1,0);
	++r %= 360;
	glBegin(GL_LINE_STRIP);
	for (unsigned i = 0; i < 100; ++i) {
		glVertex2f(i, 5*sin(i * 2 * 3.14 / 100));
	}
//        glVertex2f(1,1);
//        glVertex2f(0,1);
//        glVertex2f(1,0);
	glEnd();
	DrawEdges();
	glFlush();
	glPopMatrix();
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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL - First window demo");
	glutDisplayFunc(renderFunction);
	glutTimerFunc(1000 / 5, timing, 1);
	//glutIdleFunc(renderFunction);
	//glutReshapeFunc(resized);
	//glutPostRedisplay();
	glutMainLoop();
	return 0;
}
