/*!
    @file Vector.h @author Joel Barrett @date 27/11/09 @brief A generic vector type.
*/

#ifndef VECTOR_H_
#define VECTOR_H_

#if _MSC_VER > 1000
    #pragma once
#endif

#include "Maths.h"
#include "Typedefs.h"

namespace Framework
{
    namespace Maths
    {
        /*!
            A class for 2, 3 or 4 element vectors and points. As template parameters 
            are known at compile-time, for loops should be unrolled.
        */
        template < std::size_t N = 3, typename T = float >
        class Vector
        {
            typedef T Degree, Radian;
            
        private:
            T e_[N];
            
        public:
            //! default ctor
            Vector();
            
            //! copy ctor
            template < typename U > Vector(const Vector<N,U> &v);
            
            // overloaded ctors
            Vector(const T &x);
            Vector(const T &x, const T &y);
            Vector(const T &x, const T &y, const T &z);
            Vector(const T &x, const T &y, const T &z, const T &w);
            
        public:
            // mutator methods
            void SetXY(const T &x, const T &y);
            void SetXYZ(const T &x, const T &y, const T &z);
            void SetXYZW(const T &x, const T &y, const T &z, const T &w);
            
            // const accessor methods
            const T& x() const { return (*this)[0]; }
            const T& y() const { return (*this)[1]; }
            const T& z() const { return (*this)[2]; }
            const T& w() const { return (*this)[3]; }
            
            // accessor methods
            T& x() { return (*this)[0]; }
            T& y() { return (*this)[1]; }
            T& z() { return (*this)[2]; }
            T& w() { return (*this)[3]; }
            
            // assignment operators :: vector
            template < typename U > const Vector & operator += (const Vector<N,U> &v);
            template < typename U > const Vector & operator -= (const Vector<N,U> &v);
            template < typename U > const Vector & operator *= (const Vector<N,U> &v);
            template < typename U > const Vector & operator /= (const Vector<N,U> &v);
            
            // assignment operators :: scalar
            const Vector & operator *= (const T &s);
            const Vector & operator /= (const T &s);
            
            // subscript operator
            const T& operator[] (std::size_t i) const;
            T& operator[] (std::size_t i);
            
            // canonical rotations
            const Vector & RotateX(Degree angle);
            const Vector & RotateY(Degree angle);
            const Vector & RotateZ(Degree angle);
            
            //! unitise vector
            const Vector & Normalise();
            
            //! pointer to first element
            T* GetPointer() { return &(*this)[0]; }
            
        public:
            // unit vectors
            static const Vector<3,T> UNIT_X; //!< i-versor
            static const Vector<3,T> UNIT_Y; //!< j-versor
            static const Vector<3,T> UNIT_Z; //!< k-versor
            
            // null & one vectors
            static const Vector<3,T> ZERO;
            static const Vector<3,T> ONE;
        };
    }
}

#include "..\source\Vector.inl"

#endif // VECTOR_H_
