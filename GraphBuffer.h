/*
 * GraphBuffer.h
 *
 *  Created on: 19 Jan 2015
 *      Author: lester
 */

#ifndef GRAPHBUFFER_H_
#define GRAPHBUFFER_H_

#include <vector>
#include <memory>

template<class T>		// value type
class GraphBuffer {
public:
	GraphBuffer();
	virtual ~GraphBuffer();
private:
	std::unique_ptr<T[]> data_;		// fill up to zero
	unsigned size_;					// buffer size
	unsigned gap_;					// how many data is not displayed
	volatile unsigned wr_pos_;		// current write pos

};

#endif /* GRAPHBUFFER_H_ */
