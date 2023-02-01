/*!
    @file Maths.h @author Joel Barrett @date 27/11/09 @brief Scalar functions.
*/

#ifndef MATHS_H_
#define MATHS_H_

#if _MSC_VER > 1000
    #pragma once
#endif

#ifdef MATHS_IO
    #include <iostream>
#endif

#include <cmath>
#include <cstdlib>
#include <cassert>

#include "Constants.h"

namespace Framework
{
    namespace Maths
    {
        //! the value of a number squared
        template < typename T >
        inline const T Sqr(const T& x)
        {
            return x * x;
        }
        
        //! the value of a number cubed
        template < typename T >
        inline const T Cube(const T& x)
        {
            return x * x * x;
        }
        
        //! absolute value of a number
        template < typename T >
        inline const T Abs(const T& x)
        {
            return x < T(0) ? -x : x;
        }
        
        //! the lower of two numbers
        template < typename T >
        inline const T& Min(const T& x, const T& y)
        {
            return x < y ? x : y;
        }
        
        //! the lower of three numbers
        template < typename T >
        inline const T& Min(const T& x, const T& y, const T& z)
        {
            return x < y ? Min(x,z) : Min(y,z);
        }
        
        //! the higher of two numbers
        template < typename T >
        inline const T& Max(const T& x, const T& y)
        {
            return x > y ? x : y;
        }
        
        //! the higher of three numbers
        template < typename T >
        inline const T& Max(const T& x, const T& y, const T& z)
        {
            return x > y ? Max(x,z) : Max(y,z);
        }
        
        //! the average of two numbers
        template < typename T >
        inline const T Average(const T& x, const T& y)
        {
            return (x + y) / T(2);
        }
        
        //! exchange the values of two numbers
        template < typename T >
        inline void Swap(T& x, T& y)
        {
            T temp = x; x = y; y = temp;
        }
        
        //! exchange the values of two integers
        inline void Swap(int& x, int& y)
        {
            x ^= y; y ^= x; x ^= y;
        }
        
        //! find the sign of a number
        template < typename T >
        inline const T Sign(const T& x)
        {
            return x < T(0) ? T(-1) : (x > T(0) ? T(1) : T(0));
        }
        
        //! recursively compute the value of a factorial
        template < typename T >
        const T Factorial(const T& f)
        {
            return f < T(2) ? T(1) : Factorial(f-1) * f;
        }
        
        //! pseudorandom number in the specified range
        template < typename T >
        const T Random(const T& low, const T& high)
        {
            return rand() % high + low;
        }
        
        //! derivative of a function f from first principles
        template < typename T >
        const T Derivative(T(*f)(T), const T& x, const T& h)
        {
            return (f(x + h) - f(x)) / h;
        }
        
        //! convert x screen coordinate to view space
        template < typename T >
        inline const T ScreenToViewX(T x, T width, T height)
        {
	        return (x-width*0.5f) / ((width*0.5f) / (0.41421f * (width / height)));
        }
        
        //! convert y screen coordinate to view space
        template < typename T >
        inline const T ScreenToViewY(T y, T width, T height)
        {
	        return ((height-y) - height*0.5f) / ((height*0.5f) / 0.41421f);
        }
        
        //! test for approx. equality to zero with an absolute or relative tolerance
        template < typename T >
        inline bool IsZero(const T& x, const T& err = Max(1.0f, Abs(x)))
        {
            return Abs(x) <= (Const<T>::EPSILON * err);
        }
        
        //! test for approx. equality with an absolute or relative tolerance
        template < typename T >
        inline bool IsEqual(const T& x, const T& y, const T& err = Max(1.0f, Abs(x), Abs(y)))
        {
            return Abs(x-y) <= (Const<T>::EPSILON * err);
        }
        
        //! limit a number to the specified range
        template < typename T >
        inline const T& Clamp(const T& x, const T& low, const T& high)
        {
            return Max(low, Min(x, high));
        }
    }
}

#endif // MATHS_H_
