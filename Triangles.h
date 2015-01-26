/*
 * Triangles.h
 *
 *  Created on: 23 Jan 2015
 *      Author: lester
 */

#ifndef TRIANGLES_H_
#define TRIANGLES_H_

class EquilateralTriangle
{
    float len_;
public:
    EquilateralTriangle(float len);     // side len
    virtual ~EquilateralTriangle();
    void Draw() const;
};

#endif /* TRIANGLES_H_ */
