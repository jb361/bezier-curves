/*!
    @file Quaternion.inl @author Joel Barrett @date 01/01/12 @brief A generic quaternion type.
*/

#ifndef QUATERNION_INL_
#define QUATERNION_INL_

#if _MSC_VER > 1000
    #pragma once
#endif

namespace Framework
{
    namespace Maths
    {
        // static data member initialisations
        template < typename T > const Quaternion<T> Quaternion<T>::ZERO(0, 0, 0, 0);
        template < typename T > const Quaternion<T> Quaternion<T>::IDENTITY(1, 0, 0, 0);
        
        template < typename T > const typename Quaternion<T>::quat Quaternion<T>::q
            = { &Quaternion<T>::w, &Quaternion<T>::x, &Quaternion<T>::y, &Quaternion<T>::z };
        
        //! overloaded ctor (note: assumes axis is a unit vector)
        template < typename T > template < typename U >
        Quaternion<T>::Quaternion(const Vector<3,U> &axis, Degree angle)
        {
            FromAxisAngle(axis, angle);
        }
        
        template < typename T > template < typename U >
        void Quaternion<T>::FromAxisAngle(const Vector<3,U> &axis, Degree angle)
        {
            // compute half the angle in radians
            Radian angh = angle * Const<T>::TO_HALF_RAD;
            
            // compute sine of the half angle
            Radian sinha = sin(angh);
            
            w = cos(angh);
            x = axis.x * sinha;
            y = axis.y * sinha;
            z = axis.z * sinha;
        }
        
        //! set 3 components
        template < typename T >
        void Quaternion<T>::SetXYZ(const T &x, const T &y, const T &z)
        {
            this->x() = x;
            this->y() = y;
            this->z() = z;
        }
        
        //! set 4 components
        template < typename T >
        void Quaternion<T>::SetWXYZ(const T &w, const T &x, const T &y, const T &z)
        {
            this->w() = w;
            this->x() = x;
            this->y() = y;
            this->z() = z;
        }
        
        //! basic assignment
        template < typename T > template < typename U >
        const Quaternion<T> & Quaternion<T>::operator = (const Quaternion<U> &q)
        {
            if (this != &q) {
                w = q.w; x = q.x; y = q.y; z = q.z;
            }
            return *this;
        }
        
        //! component-wise compound assignment operators
        #define QUAT_ASSIGNMENT(OP)                                                     \
        template < typename T > template < typename U >                                 \
        const Quaternion<T> & Quaternion<T>::operator OP (const Quaternion<U> &q)       \
        {                                                                               \
            w OP q.w; x OP q.x; y OP q.y; z OP q.z;                                     \
            return *this;                                                               \
        }
        
        QUAT_ASSIGNMENT(+=)
        QUAT_ASSIGNMENT(-=)
        
        #undef QUAT_ASSIGNMENT
        
        //! assignment by multiplication (note: not commutative)
        template < typename T, typename U >
        const Quaternion<T> & Quaternion<T>::operator *= (const Quaternion<U> &q)
        {
            w *= w * q.w - x * q.x - y * q.y - z * q.z;
            x *= w * q.x + x * q.w + y * q.z - z * q.y;
            y *= w * q.y - x * q.z + y * q.w + z * q.x;
            z *= w * q.z + x * q.y - y * q.x + z * q.w;
            
            return *this;
        }
        
        //! assignment by scaling
        template < typename T >
        const Quaternion<T> & Quaternion<T>::operator *= (const T &s)
        {
            w *= s; x *= s; y *= s; z *= s;
            return *this;
        }
        
        //! assignment by inverse scaling
        template < typename T >
        const Quaternion<T> & Quaternion<T>::operator /= (const T &s)
        {
            assert(s != 0.0f); T inv = 1.0f / s;
            w *= inv; x *= inv; y *= inv; z *= inv;
            return *this;
        }
        
        //! const indexing
        template < typename T >
        inline const T & Quaternion<T>::operator[] (std::size_t i) const
        {
            assert(i < 4);
            return this->*q[i];
        }
        
        //! indexing
        template < typename T >
        inline T & Quaternion<T>::operator[] (std::size_t i)
        {
            // invoke the const version of operator[]
            return const_cast<T&>(static_cast<const Quaternion&>(*this)[i]);
        }
        
        //! unitise quaternion
        template < typename T >
        const Quaternion<T> & Quaternion<T>::Normalise()
        {
            return !IsZero(*this) ? *this /= Mag(*this) : *this;
        }
        
        //! unary minus / negation
        template < typename T >
        const Quaternion<T> operator - (const Quaternion<T> &q)
        {
            return Quaternion<T>(-q.w, -q.x, -q.y, -q.z);
        }
        
        //! component-wise arithmetic operators
        #define QUAT_ARITHMETIC(OP)                                                     \
        template < typename T > template < typename U >                                 \
        const Quaternion<T> operator OP (Quaternion<T> q1, const Quaternion<U> &q2)     \
        {                                                                               \
            return q1 OP ## = q2;                                                       \
        }
        
        QUAT_ARITHMETIC(+)
        QUAT_ARITHMETIC(-)
        
        #undef QUAT_ARITHMETIC
        
        //! multiplication (note: not commutative)
        template < typename T, typename U >
        const Quaternion<T> operator * (const Quaternion<T> &q1, const Quaternion<U> &q2)
        {
            return Quaternion<T>(q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
                                 q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
                                 q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
                                 q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w);
        }
        
        //! multiplication by scalar
        template < typename T, typename U >
        const Quaternion<T> operator * (const Quaternion<T> &q, const U &s)
        {
            return Quaternion<T>(q.w * s, q.x * s, q.y * s, q.z * s);
        }
        
        //! multiplication by scalar
        template < typename T, typename U >
        const Quaternion<T> operator * (const U &s, const Quaternion<T> &q)
        {
            return Quaternion<T>(q.w * s, q.x * s, q.y * s, q.z * s);
        }
        
        //! equality operator
        template < typename T, typename U >
        bool operator == (const Quaternion<T> &q1, const Quaternion<U> &b)
        {
            return (q1.w == b.w) && (q1.x == b.x) && (q1.y == b.y) && (q1.z == b.z);
        }
        
        //! non-equality operator
        template < typename T, typename U >
        bool operator != (const Quaternion<T> &q1, const Quaternion<U> &q2)
        {
            return (q1.w != q2.w) && ((q1.x != q2.x) || (q1.y != q2.y) || (q1.z != q2.z));
        }
        
        //! modulus / length / magnitude
        template < typename T >
        const float Mag(const Quaternion<T> &q)
        {
            return sqrt(MagSqr(v));
        }
        
        //! modulus squared (removes square root for faster comparisons)
        template < typename T >
        const float MagSqr(const Quaternion<T> &q)
        {
            return Dot(q,q);
        }
        
        //! conjugate
        template < typename T >
        const Quaternion<T> Conjugate(Quaternion<T> &q)
        {
            return Quaternion<T>(q.w, -q.x, -q.y, -q.z);
        }
        
        //! inverse
        template < typename T >
        const Quaternion<T> Inverse(Quaternion<T> &q)
        {
            T inv = 1.0f / LengthSqr(q);
            return Quaternion<T>(q.w * inv, -q.x * inv, -q.y * inv, -q.z * inv);
        }
        
        //! dot product
        template < typename T, typename U >
        T Dot(const Quaternion<T> &q1, const Quaternion<U> &q2)
        {
            return (q1.w * q2.w) + (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z);
        }
        
        //! convert to rotation matrix
        template < typename T >
        const Matrix<4,4,T> ToRotationMatrix4x4(const Quaternion<T> &q)
        {
            return Matrix<4,4,T>(1 - 2 * q.y * q.y - 2 * q.z * q.z, 2 * q.x * q.y + 2 * q.w * q.z, 2 * q.x * q.z - 2 * q.w * q.y, 0,
                                 2 * q.x * q.y - 2 * q.w * q.z, 1 - 2 * q.x * q.x - 2 * q.z * q.z, 2 * q.y * q.z + 2 * q.w * q.x, 0,
                                 2 * q.x * q.z + 2 * q.w * q.y, 2 * q.y * q.z - 2 * q.w * q.x, 1 - 2 * q.x * q.x - 2 * q.y * q.y, 0,
                                 0, 0, 0, 1);
        }
        
#ifdef MATHS_IO
        //! input a quaternion from console
        template < typename T >
        inline std::istream & operator >> (std::istream &is, Quaternion<T> &q)
        {
            char t; is >> t >> q.w >> t >> q.x >> t >> q.y >> t >> q.z >> t;
            return is;
        }
        
        //! output a quaternion to console
        template < typename T >
        inline std::ostream & operator << (std::ostream &os, const Quaternion<T> &q)
        {
            os << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")\n";
            return os;
        }
#endif
    }
}

#endif // QUATERNION_INL_
