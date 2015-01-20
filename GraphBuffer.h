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

// Range validation
class Range
{
    template<bool VI, bool TI,class V,class T>
    static bool check_impl_(V v, T start, T end);

    // true means signed number
    template<bool VI, bool TI,class V,class T>
    class range_impl_
    {
        friend class Range;
        //template<class V, class T>
        static bool check(V v, T start, T end);
//        {
//            return (v >= start) && (v <= end);
//        }
    };
public:
    template<class V, class T>
    static bool check(V v, T start, T end)
    {
        return range_impl_<std::is_signed<V>::value,std::is_signed<T>::value,V,T>::check(v, start, end);
    }
};

template<class V, class T>
bool Range::check_impl_<true,false,V,T>(V v, T start, T end)
{
    return false;
}

template<class T>		// value type
class GraphBuffer
{
public:
    GraphBuffer(unsigned size);
    virtual ~GraphBuffer();
    class rd_it
    {
        GraphBuffer& buffer_;
        T* rd_ptr;		// current read pos
        T* end_;
        unsigned max;	// max number of bytes to read
        friend class GraphBuffer;
    protected:
        rd_it(GraphBuffer& buffer) :
                buffer_(buffer), //
                rd_ptr(buffer.data_.get() + //
                        (buffer.wr_pos_ + buffer.gap_) % buffer.size_), //
                end_(buffer.data_.get() + buffer.size_), //
                max(buffer.size_ - buffer.gap_)
        {
        }
    public:
        bool next();	// return false for eof
        T get() const;	// get value
    };
private:
    std::unique_ptr<T[]> data_;		// fill up to zero
    unsigned size_;					// buffer size
    unsigned gap_;					// how many data is not displayed
    volatile unsigned wr_pos_;		// current write pos

};

template<class T>
inline GraphBuffer<T>::GraphBuffer(unsigned size) :
        data_(new T[size]), size_(size), gap_(size / 3), wr_pos_(0)
{
}

template<class T>
GraphBuffer<T>::~GraphBuffer()
{
    // TODO Auto-generated destructor stub
}

template<class T>
inline bool GraphBuffer<T>::rd_it::next()
{
    rd_ptr++;
    if (rd_ptr == end_)
        rd_ptr = buffer_.data_.get();
    return (max-- == 0);
}

template<class T>
inline T GraphBuffer<T>::rd_it::get() const
{
    return *rd_ptr;
}

#endif /* GRAPHBUFFER_H_ */
