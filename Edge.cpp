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

Edge::Edge(float start_x, float start_y, float size_x, float size_y, unsigned ticks_x, unsigned ticks_y, float tick_width) :
        start_x_(start_x), start_y_(start_y),size_x_(size_x), size_y_(size_y),ticks_x_(ticks_x),ticks_y_(ticks_y),ticks_size_(tick_width)
{
}

void Edge::Draw() const
{
    glBegin(GL_LINES);
    glVertex2f(start_x_, 0);
    glVertex2f(start_x_ + size_x_, 0);

    glVertex2f(0, start_y_);
    glVertex2f(0, start_y_ + size_y_);

    for (float x = start_x_ + size_x_; x > start_x_; x -= size_x_/ticks_x_)
    {
        glVertex2f(x, -ticks_size_/2);
        glVertex2f(x, ticks_size_/2);
    }

    for (float y = start_y_ + size_y_; y > start_y_; y -= size_y_/ticks_y_)
    {
        glVertex2f(-ticks_size_/2, y);
        glVertex2f(ticks_size_/2, y);
    }
    glEnd();
}
