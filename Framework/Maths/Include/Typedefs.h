/*!
    @file Typedefs.h @author Joel Barrett @date 27/11/09 @brief Type definitions.
*/

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#if _MSC_VER > 1000
    #pragma once
#endif

namespace Framework
{
    namespace Maths
    {
        template < std::size_t N, typename T >
        class Vector;
        
        template < std::size_t R, std::size_t C, typename T >
        class Matrix;
        
        #define VECTOR_TYPE(T,N)                                            \
            typedef Vector<N,int> T ## N ## i;                              \
            typedef Vector<N,long> T ## N ## l;                             \
            typedef Vector<N,float> T ## N ## f;                            \
            typedef Vector<N,double> T ## N ## d;                           \
            typedef Vector<N,unsigned> T ## N ## u;
        
        VECTOR_TYPE(Vector,2)
        VECTOR_TYPE(Vector,3)
        VECTOR_TYPE(Vector,4)
        
        VECTOR_TYPE(Point,2)
        VECTOR_TYPE(Point,3)
        VECTOR_TYPE(Point,4)
        
        #undef VECTOR_TYPE
        
        #define MATRIX_TYPE(R,C)                                            \
            typedef Matrix<R,C,int> Matrix ## R ## x ## C ## i;             \
            typedef Matrix<R,C,long> Matrix ## R ## x ## C ## l;            \
            typedef Matrix<R,C,float> Matrix ## R ## x ## C ## f;           \
            typedef Matrix<R,C,double> Matrix ## R ## x ## C ## d;          \
            typedef Matrix<R,C,unsigned> Matrix ## R ## x ## C ## u;
        
        MATRIX_TYPE(2,2)
        MATRIX_TYPE(3,3)
        MATRIX_TYPE(4,4)
        
        #undef MATRIX_TYPE
    }
}

#endif // TYPEDEFS_H_
