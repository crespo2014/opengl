/*
* numeric_check.h
*
*  Created on: 21 Jan 2015
*      Author: lester
*/

#ifndef NUMERIC_CHECK_H_
#define NUMERIC_CHECK_H_

#include <numeric>
#include <limits>
#include "stddef.h"

/*
 * Signed unsigned safe less equal function implementation
 *
 * a <=b
 * int uint --> (a <= 0) || ( b >= (B)a  && a <= (A)b )
 * uint int --> (b >= 0) && ( a <= (A)b  && b >= (B)a )
 *
 * all comparations are done using positive numbers
 * If integral promotion is used then one condition can be remove
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
     * if sizeof A > sizeof B then casting to A
     * if sizeof A < sizeof B then casting to B
     * if sizeof A == sizeof A then
     *  if A is signed cast to B else cast to A
     *
     * true means static cast to A
     */
    template<size_t a,size_t b,bool intA>
    struct is_le_cast_to_a_
    {
        constexpr static const bool value = (a > b) ? true : (a < b) ? false : intA ? false : true;
    };
    // comparation function using cast
    template<bool>
    struct cast_cmp_
    {
        template <class A,class B>
        static inline bool le(A a,B b);
    };
    // cast to A implementation
    template<>
    template<class A,class B>
    inline bool cast_cmp_<true>::le(A a,B b)
    {
        return (a <= static_cast<A>(b));
    }
    // cast to b implementation
    template<>
    template<class A,class B>
    inline bool cast_cmp_<false>::le(A a,B b)
    {
        return (static_cast<B>(a) <= b);
    }
    /*
     * signed unsigned comparator
     * Parameter
     * is A int, is B int
     */
    template<bool intA,bool intB>
    struct signed_unsigned_cmp_
    {
        template<class A,class B>
        static inline bool less_equal(A a,B b)
        {
            return a <= b;
        }
    };
    // Specialization for uint int
    template<>
    template<class A,class B>
    inline bool signed_unsigned_cmp_<false,true>::less_equal(A a,B b)
    {
        return (b >=0) && cast_cmp_< is_le_cast_to_a_<sizeof(A),sizeof(B),std::is_signed<A>::value >::value >::le(a,b);
    }
    // Specialization for int uint
    template<>
    template<class A,class B>
    inline bool signed_unsigned_cmp_<true,false>::less_equal(A a,B b)
    {
       return (a <=0) || cast_cmp_< is_le_cast_to_a_<sizeof(A),sizeof(B),std::is_signed<A>::value >::value >::le(a,b);
    }

    template<class A, class B>
    static inline bool less_equal(A a, B b)
    {
        return signed_unsigned_cmp_<std::is_signed<A>::value,std::is_signed<B>::value>::less_equal(a, b);
    }

    template<class V, class T>
    static bool check(V v, T start, T end)
    {
        return less_equal(start, v) && less_equal(v, end);
    }

    template<class A, class B>
    static bool double_equal(A a,B b)
    {
        static_assert(std::is_floating_point<A>::value && std::is_floating_point<B>::value,"Only floating point arguments are allowed");

    }
    /*
     * Struct for number convertion
     */
    template<class O,class I>
    struct safe_cast_
    {
        /**
         * Convert one type to another in a safe way
         */
        static O get(I input)
        {
           constexpr O min = std::is_floating_point<O>::value ? -std::numeric_limits<O>::max() : std::numeric_limits<O>::min();
           constexpr O max = std::numeric_limits<O>::max();
           if (!check(input,min,max))
               throw std::out_of_range((Faz::FormatString() << "Value " << input << " is not between " << +min << " and " << +max).str());
           return static_cast<O>(input);
        }
    };
    //specialization for the same type
    template<class T>
    struct safe_cast_<T,T>
    {
        static inline T  get(T input)
        {
            return input;
        }
    };

    // Template argument deduction only works on function arguments, not on the return type
    template<class O, class I>
    O get_safe(I input,const O&) {        // template can not be deduce by the out type
        return safe_cast_<O,I>::get(input);
    }

    template<class T>
    T string_to_uint(const char* str)
    {
        auto t = std::stoul(str);
        return get_safe<T>(t,T());
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
