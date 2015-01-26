/*
 * Edge.h
 *
 *  Created on: 21 Jan 2015
 *      Author: lester
 */

#ifndef EDGE_H_
#define EDGE_H_

class Edge
{
public:
    Edge(float start_x,float start_y,float size_x,float size_y,unsigned ticks_x = 10,unsigned ticks_y = 10,float tick_width = 10);
    Edge();
    ~Edge();
    // Draw the edge
    void Draw() const;
private:
    int start_x_ = -50;       // start point on x
    int start_y_ = -50;       // start point on x
    unsigned size_x_ = 100;   // length of x axis
    unsigned size_y_ = 100;   // length of y axis
    unsigned ticks_x_ = 10;   // distance between mark on x axis
    unsigned ticks_y_ = 10;   // distance between mark on y axis
    unsigned ticks_size_ = 2;  // size of mark on axis

};

#endif /* EDGE_H_ */
