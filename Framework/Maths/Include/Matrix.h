/*!
    @file Matrix.h @author Joel Barrett @date 27/11/09 @brief A generic matrix type.
*/

#ifndef MATRIX_H_
#define MATRIX_H_

#if _MSC_VER > 1000
    #pragma once
#endif

#include "Vector.h"

namespace Framework
{
    namespace Maths
    {
        /*!
            A class for m by n row-major matrices. As template paramaters are known
            at compile-time, for loops should be unrolled.
        */
        template < std::size_t R = 4, std::size_t C = 4, typename T = float >
        class Matrix : public Vector < R, Vector<C,T> >
        {
            typedef T Degree, Radian;
            
        public:
            //! default ctor
            Matrix(){}
            
            //! overloaded ctors
            Matrix(const Vector<2,T> &v1, const Vector<2,T> &v2): Vector(v1,v2){}
            Matrix(const Vector<3,T> &v1, const Vector<3,T> &v2, const Vector<3,T> &v3): Vector(v1,v2,v3){}
            Matrix(const Vector<4,T> &v1, const Vector<4,T> &v2, const Vector<4,T> &v3, const Vector<4,T> &v4): Vector(v1,v2,v3,v4){}
            
            //! 2x2 overloaded ctor
            Matrix(const T &_00, const T &_01,
                   const T &_10, const T &_11);
            
            //! 3x3 overloaded ctor
            Matrix(const T &_00, const T &_01, const T &_02,
                   const T &_10, const T &_11, const T &_12,
                   const T &_20, const T &_21, const T &_22);
            
            //! 4x4 overloaded ctor
            Matrix(const T &_00, const T &_01, const T &_02, const T &_03,
                   const T &_10, const T &_11, const T &_12, const T &_13,
                   const T &_20, const T &_21, const T &_22, const T &_23,
                   const T &_30, const T &_31, const T &_32, const T &_33);
            
        public:
            // canonical rotations (note: matrix should be an identity matrix)
            const Matrix & RotateX(Degree angle);
            const Matrix & RotateY(Degree angle);
            const Matrix & RotateZ(Degree angle);
            
            // scaling (note: matrix should be an identity matrix)
            const Matrix& Scale(const T &sx, const T &sy, const T &sz);
            
            // translation (note: matrix should be an identity matrix)
            const Matrix& Translate(const T &tx, const T &ty, const T &tz);
            
            // reflection (note: matrix should be an identity matrix)
            const Matrix& Reflect(const T &rx, const T &ry, const T &rz);
            
            //! make this matrix an identity matrix
            void Identity();
            
            //! pointer to first element
            T* GetPointer() { return &(*this)[0][0]; }
            
        public:
            //! identity matrix
            static const Matrix<4,4,T> IDENTITY;
        };
    }
}

#include "..\source\Matrix.inl"

#endif // MATRIX_H_
