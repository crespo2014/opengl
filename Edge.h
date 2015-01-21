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
    Edge();
    ~Edge();
    // Draw the edge
    void Draw();
private:
    unsigned size_x_ = 100;   // length of x axis
    unsigned size_y_ = 100;   // length of y axis
    unsigned step_x_ = 10;   // distance between mark on x axis
    unsigned step_y_ = 10;   // distance between mark on y axis
    unsigned step_half_size_ = 2;  // size of mark on axis
    int start_x_ = -50;       // start point on x
    int start_y_ = -50;       // start point on x
};

#endif /* EDGE_H_ */
