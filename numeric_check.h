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
#include <type_traits>
#include <stdexcept>
#include <sstream>
#include <complex>
#include "stddef.h"
#include "common.h"
#include <cstdlib>
#include <climits>

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
     *  if A is unsigned cast to A else cast to B
     */
    template<class A, class B>
    struct cast_cmp_
    {
        using T = typename std::conditional<(sizeof(A) > sizeof(B)) || ((sizeof(A) == sizeof(B) && std::is_unsigned<A>::value)), A, B>::type;
        static inline bool le(A a,B b)
        {
            return static_cast<T>(a) <= static_cast<T>(b);
        }

		static inline bool eq(A a, B b)
		{
			return static_cast<T>(a) == static_cast<T>(b);
		}
    };
    /*
    * signed unsigned comparator
    * Parameter
    * is A int, is B int
    */
    template<bool intA, bool intB>
    struct signed_unsigned_cmp_
    {
        template<class A, class B>
        static inline bool less_equal(A a, B b)
        {
            return a <= b;
        }
        template<class A,class B>
		static inline bool equal(A a,B b)
		{
			return a == b;
        }
    };
    // Specialization for uint int
    template<>
    template<class A, class B>
    inline bool signed_unsigned_cmp_<false,true>::less_equal(A a,B b)
    {
        return (b >=0) && cast_cmp_<A,B>::le(a,b);
    }
    template<>
	template<class A,class B>
	inline bool signed_unsigned_cmp_<false,true>::equal(A a,B b)
	{
		return (b >=0) && cast_cmp_<A,B>::eq(a,b);
    }
    // Specialization for int uint
    template<>
    template<class A, class B>
    inline bool signed_unsigned_cmp_<true,false>::less_equal(A a,B b)
    {
       return (a <=0) || cast_cmp_<A,B>::le(a,b);
    }

    template<>
    template<class A, class B>
	inline bool signed_unsigned_cmp_<true,false>::equal(A a,B b)
    {
	   return (a >=0) && cast_cmp_<A,B>::eq(a,b);
    }

    template<bool is_float,class T>
    struct float_impl_
    {
    	constexpr const static T epsilon = 0;	// not float number
    	static bool equal(T a,T b,const T)
    	{
    		return a == b;
    }

    	static bool less_equal(T a,T b,const T)
    {
    		return a <= b;
    }
	};
    template<class T>
    struct float_impl_<true,T>
    {
    	constexpr const static T epsilon = std::numeric_limits<T>::epsilon();
    	static bool equal(const T a, const T b,const T tolerance = epsilon)
		{
    		return std::abs(a-b) < tolerance;
		}
		static bool less_equal(const T a,const T b,const T tolerance = epsilon)
		{
			return ( a < b) || equal(a,b,tolerance);
		}

    };
    /**
     * Forward declarations
     */
    template<class V, class T>
    static bool check(V v, T start, T end);
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
           if (!Range::check(input,min,max))
               throw std::out_of_range((std::ostringstream() << "Value " << input << " is not between " << +min << " and " << +max).str());
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
    /**
     * Global functions start here
     */
    template<class A, class B>
	static inline bool less_equal(A a, B b)
	{
		return signed_unsigned_cmp_<std::is_signed<A>::value,std::is_signed<B>::value>::less_equal(a, b);
	}
    template<class A, class B>
	static inline bool equal(A a, B b)
	{
		return signed_unsigned_cmp_<std::is_signed<A>::value,std::is_signed<B>::value>::equal(a, b);
	}

	template<class V, class T>
	static bool check(V v, T start, T end)
	{
		return less_equal(start, v) && less_equal(v, end);
	}

    template<class A>
    static bool flt_equal(A a,A b,const A tolerance = float_impl_<std::is_floating_point<A>::value,A>::epsilon)
    {
    	return float_impl_<std::is_floating_point<A>::value,A>::equal(a,b,tolerance);
    }
	template<class A>
	static bool flt_not_equal(const A a, const A b,const A tolerance = float_impl_<std::is_floating_point<A>::value,A>::epsilon)
	{
		return !flt_equal(a, b,tolerance);
	}
    template<class A>
    static bool flt_less_equal(A a,A b,const A tolerance = float_impl_<std::is_floating_point<A>::value,A>::epsilon)
    {
    	return float_impl_<std::is_floating_point<A>::value,A>::less_equal(a,b,tolerance);
    }

    // Template argument deduction only works on function arguments, not on the return type
    template<class O, class I>
    O get_safe(I input,const O&) {        // template can not be deduce by the out type
        return safe_cast_<O,I>::get(input);
    }
    // Template argument deduction only works on function arguments, not on the return type
    template<class O, class I>
    O get_safe(I input) {        // template can not be deduce by the out type
        return safe_cast_<O,I>::get(input);
    }

    template<class T>
    T string_to_uint(const char* str)
    {
        auto t = std::stoul(str);
        return get_safe<T>(t,T());
    }
    /*
	 * Implementation of string to number
	 * a template for float double, signed and unsigned is needed
	 */
	template<class T>
	struct str_to_num_impl_
	{
		constexpr static const T min = std::is_floating_point<T>::value ? -std::numeric_limits<T>::max() : std::numeric_limits<T>::min();
		constexpr static const T max = std::numeric_limits<T>::max();
		// Read float number
		struct float_impl_
		{
			using D = decltype(strtod(nullptr,nullptr));
			static D  get(const char* str,char** end)
			{
				return strtod(str,end);
			}
		};
		// Read signed number
		struct signed_impl_
		{
			using D = decltype(strtol(nullptr,nullptr,0));
			static D get(const char* str,char** end)
			{
				return strtol(str,end,10);
			}
		};
		// read unsigned number
		struct unsigned_impl_
		{
			using D = decltype(strtoul(nullptr,nullptr,0));
			static D get(const char* str,char** end)
			{
				D v = strtoul(str,end,10);		// ULONG_MAX is returned for negative numbers but it is a valid unsigned
				const char *t = str;
				while (*t != 0 && *t == ' ') ++t;
				if ((v == ULONG_MAX) || (*t == '-'))
					throw std::out_of_range((FormatString() << str << " is not a valid unsigned number").str());
				return v;
			}
		};
		// depends on number type different structure is to be use
		using S = typename std::conditional<std::is_floating_point<T>::value,float_impl_,typename
						   std::conditional<std::is_signed<T>::value,signed_impl_,unsigned_impl_>::type>::type;

		static T get(const char* str)
		{
			char* end;
			auto v = S::get(str,&end);
			if (end == str)
				throw std::invalid_argument(FormatString()<<"can not extract number from "<<str);
			return get_safe(v,(T)0);
		}
		static bool get(const char* str,T& v,const std::nothrow_t)
		{
			return false;	//FIXME to be implement
		}
	};
	/*
    	convert string to number
    	std::out_of_range for valid numbers,
    	std::invalid_argument there is not number.
     	 template has to be explicit specified in other to use the function in anidate functions call
     	 it valid to use
     	 int i = str_to<decltype(i)>("456");
     */
	template<class T>
	T str_to(const char* str)
	{
		return str_to_num_impl_<T>::get(str);
	}

	 /*
     * Do safe convertion between different numbers type
     *
     * std::out_of_range      the input is out of range
     * std::invalid_argument  the input string does not represented any number
     */
    template<class T>
    struct safe
    {
        template<class I>
        static T get(const I& input)
        {
            return get_safe<T,I>(input);
        }
        static T get(const char* input)
        {
            return str_to<T>(input);
        }
		static T get(const std::string& input)
        {
            return str_to<T>(input.c_str());
        }
    };

    static void test()
    {
        volatile char c = 7;
        volatile unsigned char uc = 0xAA;
        volatile int i = -1;
        volatile unsigned int ui = 0xF000;

        std::cout << std::boolalpha;

        std::cout << less_equal(c, uc) << std::endl;
        std::cout << less_equal(uc, c)<< std::endl;

        std::cout << less_equal(c, i)<< std::endl;
        std::cout << less_equal(i, c)<< std::endl;

        std::cout << less_equal(ui, uc)<< std::endl;
        std::cout << less_equal(uc, ui)<< std::endl;

        std::cout << less_equal(i, uc)<< std::endl;
        std::cout << less_equal(uc, i)<< std::endl;

        std::cout << less_equal(ui, c)<< std::endl;
        std::cout << less_equal(c, ui)<< std::endl;

        std::cout << less_equal(ui, i)<< std::endl;
        std::cout << less_equal(i, ui)<< std::endl;

        std::cout << ((c <=0) || (static_cast<unsigned int>(c) <= ui))<<std::endl;
        std::cout << ((c <=0) || (c <= static_cast<char>(ui)))<<std::endl;
    }
}


#endif /* NUMERIC_CHECK_H_ */
