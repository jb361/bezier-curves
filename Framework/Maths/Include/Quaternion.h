/*!
    @file Quaternion.h @author Joel Barrett @date 01/01/12 @brief A generic quaternion type.
*/

#ifndef QUATERNION_H_
#define QUATERNION_H_

#if _MSC_VER > 1000
    #pragma once
#endif

#include "Vector.h"
#include "Matrix.h"

namespace Framework
{
    namespace Maths
    {
        /*!
            A class for generic quaternions.
        */
        template < typename T = float >
        class Quaternion
        {
        private:
            typedef T Quaternion<T>::* const quat[4];
            typedef T Degree, Radian;
            
            static const quat q;
            
        private:
            T w, x, y, z; // w = real part
                          // x, y, z = imaginary part
            
        public:
            //! default/overloaded ctor
            Quaternion<T>(const T &_w = 1.0f, const T &_x = T(), const T &_y = T(), const T &_z = T())
                : w(_w), x(_x), y(_y), z(_z){}
            
            //! overloaded ctor
            template < typename U > Quaternion<T>(const Vector<3,U> &axis, Degree angle);
            
            // mutator methods
            void SetXYZ(const T &x, const T &y, const T &z);
            void SetWXYZ(const T &w, const T &x, const T &y, const T &z);
            
            // const accessor methods
            const T& w() const { return (*this)[0]; }
            const T& x() const { return (*this)[1]; }
            const T& y() const { return (*this)[2]; }
            const T& z() const { return (*this)[3]; }
            
            // accessor methods
            T& w() { return (*this)[0]; }
            T& x() { return (*this)[1]; }
            T& y() { return (*this)[2]; }
            T& z() { return (*this)[3]; }
            
            template < typename U > void FromAxisAngle(const Vector<3,U> &axis, Degree angle);
            
            // assignment operators :: quaternion
            template < typename U > const Quaternion<T> & operator  = (const Quaternion<U> &q);
            template < typename U > const Quaternion<T> & operator += (const Quaternion<U> &q);
            template < typename U > const Quaternion<T> & operator -= (const Quaternion<U> &q);
            template < typename U > const Quaternion<T> & operator *= (const Quaternion<U> &q);
            
            // assignment operators :: scalar
            const Quaternion<T> & operator *= (const T &s);
            const Quaternion<T> & operator /= (const T &s);
            
            // subscript operator
            const T& operator[] (std::size_t i) const;
            T& operator[] (std::size_t i);
            
            // unitise this quaternion
            const Quaternion<T> & Normalise();
            
        public:
            static const Quaternion<T> IDENTITY;
            static const Quaternion<T> ZERO;
        };
    }
}

#include "..\source\Quaternion.inl"

#endif // QUATERNION_H_
