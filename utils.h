/*
 * utils.h
 *
 *  Created on: 23 Jan 2015
 *      Author: lester
 */

#ifndef UTILS_H_
#define UTILS_H_


// Get file name from FILE
template<const char *prev,const char* next>
struct file_name
{
    static constexpr const char * getName()
    {
        return (*next == 0) ? prev : (*next != '\\') ? file_name<prev,next+1>::getName() : file_name<next+1,next+1>::getName();
    }
};

template<const char* f>
struct path_to_name
{
    static constexpr const char* value = file_name<f,f>::getName();
};



#endif /* UTILS_H_ */
