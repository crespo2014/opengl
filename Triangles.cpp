/*
 * Triangles.cpp
 *
 *  Created on: 23 Jan 2015
 *      Author: lester
 */

#include "Triangles.h"

#include "GL/freeglut.h"
#include "GL/gl.h"

EquilateralTriangle::EquilateralTriangle(float len) : len_(len)
{
}

EquilateralTriangle::~EquilateralTriangle()
{
    // TODO Auto-generated destructor stub
}

void EquilateralTriangle::Draw() const
{
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(0,0);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(len_,0.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(len_/2,len_/2);
    glEnd();
}
