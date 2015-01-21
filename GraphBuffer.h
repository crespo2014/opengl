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

/*
 * to do safe range check.
 * We are going to implement less_equal function in two phases
 * a <=b with promotions. both numbers has to be possitive
 *
 * a <=b
 * int uint --> (a <=0) || ( b>= (B)a && a<= (A)b)
 * uint int --> (b >=0) && a <= (A)b && b>= (B)a
 *
 * all comparation is done from positive numbers
 *
 * if numbers have same size. int is safe to be uint
 * int < uint is safe to uint
 * uint < int is safe to be int
 * int = uint is safe to cast uint
 *
 * a <=b
 * int uint --> (a <=0) ||  [ (uint)a <= b | a <= (int)b ]
 */
namespace Range
{
    template<size_t a,size_t b>
    struct _size_compare
    {
        constexpr static const char value = (a == b) ? 0 : (a > b) ? 1 : 2;
    };
    /*
    promotion will compare doing cast of the first or second type
    if both parameter are equal promotions is done to uint
    */
    template<char size_cmp>
    struct _impl2_
    {
        // is it uint less equal than int
        template<class A,class B>
        inline static bool uint_le_int(A a,B b);

        // is int number less equal than uint
        template<class A,class B>
        inline static bool int_le_uint(A a,B b);
    };
    /*
     * Parameter
     * is A int, is B int
     */
    template<bool intA,bool intB>
    struct _impl1_
    {
        template<class A,class B>
        static inline bool less_than(A a,B b)
        {
            return a <= b;
        }
    };
    // Specialization for uint int
    template<>
    struct _impl1_<false,true>
    {
        template<class A,class B>
        static inline bool less_than(A a,B b)
        {
            return (b >=0) && _impl2_< _size_compare<sizeof(A),sizeof(B)>::value >::uint_le_int(a,b);
        }
    };
    // Specialization for int uint
    template<>
    struct _impl1_<true,false>
    {
        template<class A,class B>
        static inline bool less_than(A a,B b)
        {
            return (a <=0) || _impl2_< _size_compare<sizeof(A),sizeof(B)>::value >::int_le_uint(a,b);
        }
    };

    template<class V, class T>
    static bool check(V v, T start, T end)
    {
        return _impl1_<std::is_signed<T>::value,std::is_signed<V>::value>::less_than(start, v) &&
               _impl1_<std::is_signed<V>::value,std::is_signed<T>::value>::less_than(v, end);
    }
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
