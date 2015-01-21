/*
 * numeric_check.h
 *
 *  Created on: 21 Jan 2015
 *      Author: lester
 */

#ifndef NUMERIC_CHECK_H_
#define NUMERIC_CHECK_H_


/*
 * To do safe range check.
 * We are going to implement less_equal function
 *
 * a <=b
 * int uint --> (a <= 0) || ( b >= (B)a  && a <= (A)b )
 * uint int --> (b >= 0) && ( a <= (A)b  && b >= (B)a )
 *
 * all comparation is done using positive numbers
 *
 * if numbers have same size. int can be safe promoted to uint
 *
 *  a <= b
 * int  uint --> (a <=0) ||  [ (uint)a <= b | a <= (int)b ]
 * uint int  --> (b >=0) ||  [ (uint)a <= b | a <= (int)b ]
 *
 * The question is. cast to B or to A?
 *
 *  size A = size B
 *  uint <= int  A
 *  int  <= uint B
 *
 *  size A > size B
 *  uint <= int  A
 *  int  <= uint A
 *
 *  size A < size B
 *  uint <= int B
 *  int <= uint B
 */
namespace Range
{
    /*
     * a <= b resumes to do a casting to a or b base on sizeof B,A and typeof A
     */
    template<size_t a,size_t b,bool intA>
    struct _le_cast_to_a_
    {
        constexpr static const bool value = (a > b) ? true : (a < b) ? false : intA ? false : true;
    };

    // Casting to A default implementation
    template<bool cast_to_a = true>
    struct _less_equal
    {
        template <class A,class B>
        static inline bool le(A a,B b)
        {
            return (a <= static_cast<A>(b));
        }
    };
    // Casting to b specialization
    template<>
    struct _less_equal<false>
    {
        template <class A,class B>
        static inline bool le(A a,B b)
        {
            return (static_cast<B>(a) <= b);
        }
    };
    /*
     * Parameter
     * is A int, is B int
     */
    template<bool intA,bool intB>
    struct _impl1_
    {
        template<class A,class B>
        static inline bool less_equal(A a,B b)
        {
            return a <= b;
        }
    };
    // Specialization for uint int
    template<>
    struct _impl1_<false,true>
    {
        template<class A,class B>
        static inline bool less_equal(A a,B b)
        {
            return (b >=0) && _less_equal< _le_cast_to_a_<sizeof(A),sizeof(B),std::is_signed<A>::value >::value >::le(a,b);
        }
    };
    // Specialization for int uint
    template<>
    struct _impl1_<true,false>
    {
        template<class A,class B>
        static inline bool less_equal(A a,B b)
        {
            return (a <=0) || _less_equal< _le_cast_to_a_<sizeof(A),sizeof(B),std::is_signed<A>::value >::value >::le(a,b);
        }
    };

    template<class A, class B>
    static bool less_equal(A a, B b)
    {
        return _impl1_<std::is_signed<A>::value,std::is_signed<B>::value>::less_equal(a, b);
    }

    template<class V, class T>
    static bool check(V v, T start, T end)
    {
        return less_equal(start, v) && less_equal(v, end);
    }

    void test()
    {
        volatile char c = 7;
        volatile unsigned char uc = 0xAA;
        volatile int i = -1;
        volatile unsigned int ui = 0xF000;
        volatile bool b;

        b = less_equal(c, uc);
        b = less_equal(uc, c);

        b = less_equal(c, i);
        b = less_equal(i, c);

        b = less_equal(ui, uc);
        b = less_equal(uc, ui);

        b = less_equal(i, uc);
        b = less_equal(uc, i);
        b = less_equal(ui, c);

        b = less_equal(c, ui);
        b= (c <=0) || (static_cast<unsigned int>(c) <= ui);
        b= (c <=0) || (c <= static_cast<char>(ui));
    }
}


#endif /* NUMERIC_CHECK_H_ */
