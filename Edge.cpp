/*
 * Edge.cpp
 *
 *  Created on: 21 Jan 2015
 *      Author: lester
 */

#include "GL/freeglut.h"
#include "GL/gl.h"
#include "Edge.h"

Edge::Edge()
{
    // TODO Auto-generated constructor stub

}

Edge::~Edge()
{
    // TODO Auto-generated destructor stub
}

void Edge::Draw()
{
    glBegin(GL_LINES);
    glVertex2f(start_x_,0);
    glVertex2f(start_x_ + size_x_, 100);
    glVertex2f(0,start_y_);
    glVertex2f(0,start_y_ + size_y_);

    for (int x = start_x_ + size_x_ ; x > start_x_ ; x -= step_x_)
    {
        glVertex2f(x,-step_half_size_);
        glVertex2f(x,step_half_size_);
    }

    for (int y = start_y_ + size_y_ ; y > start_y_ ; y -= step_y_)
    {
        glVertex2f(-step_half_size_,y);
        glVertex2f(step_half_size_,y);
    }
    glEnd();
}
