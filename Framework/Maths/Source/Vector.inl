/*!
    @file Vector.inl @author Joel Barrett @date 27/11/09 @brief A generic vector type.
*/

#ifndef VECTOR_INL_
#define VECTOR_INL_

#if _MSC_VER > 1000
    #pragma once
#endif

namespace Framework
{
    namespace Maths
    {
        // static data member initialisations
        template < std::size_t N, typename T > const Vector<3,T> Vector<N,T>::UNIT_X(1, 0, 0);
        template < std::size_t N, typename T > const Vector<3,T> Vector<N,T>::UNIT_Y(0, 1, 0);
        template < std::size_t N, typename T > const Vector<3,T> Vector<N,T>::UNIT_Z(0, 0, 1);
        template < std::size_t N, typename T > const Vector<3,T> Vector<N,T>::ZERO(0, 0, 0);
        template < std::size_t N, typename T > const Vector<3,T> Vector<N,T>::ONE(1, 1, 1);
        
        //! default ctor
        template < std::size_t N, typename T >
        Vector<N,T>::Vector()
        {
            assert(N >= 2 && N <= 4);
            
            for (std::size_t i = 0; i < N; ++i)
                e_[i] = T(0);
        }
        
        //! copy ctor
        template < std::size_t N, typename T > template < typename U >
        Vector<N,T>::Vector(const Vector<N,U> &v)
        {
            assert(N >= 2 && N <= 4);
            
            for (std::size_t i = 0; i < N; ++i)
                e_[i] = T(v[i]);
        }
        
        //! overloaded ctor
        template < std::size_t N, typename T >
        Vector<N,T>::Vector(const T &x)
        {
            assert(N >= 2 && N <= 4);
            
            for (std::size_t i = 0; i < N; ++i)
                e_[i] = x;
        }
        
        //! overloaded ctor
        template < std::size_t N, typename T >
        Vector<N,T>::Vector(const T &x, const T &y)
        {
            assert(N == 2);
            SetXY(x,y);
        }
        
        //! overloaded ctor
        template < std::size_t N, typename T >
        Vector<N,T>::Vector(const T& x, const T& y, const T& z)
        {
            assert(N == 3);
            SetXYZ(x,y,z);
        }
        
        //! overloaded ctor
        template < std::size_t N, typename T >
        Vector<N,T>::Vector(const T &x, const T &y, const T &z, const T &w)
        {
            assert(N == 4);
            SetXYZW(x,y,z,w);
        }
        
        //! set 2 components
        template < std::size_t N, typename T >
        void Vector<N,T>::SetXY(const T &x, const T &y)
        {
            this->x() = x;
            this->y() = y;
        }
        
        //! set 3 components
        template < std::size_t N, typename T >
        void Vector<N,T>::SetXYZ(const T &x, const T &y, const T &z)
        {
            this->x() = x;
            this->y() = y;
            this->z() = z;
        }
        
        //! set 4 components
        template < std::size_t N, typename T >
        void Vector<N,T>::SetXYZW(const T &x, const T &y, const T &z, const T &w)
        {
            this->x() = x;
            this->y() = y;
            this->z() = z;
            this->w() = w;
        }
        
        //! component-wise compound assignment operators
        #define VECTOR_ASSIGNMENT(OP)                                                   \
        template < std::size_t N, typename T > template < typename U >                  \
        const Vector<N,T> & Vector<N,T>::operator OP (const Vector<N,U> &v)             \
        {                                                                               \
            for (std::size_t i = 0; i < N; ++i)                                         \
                e_[i] OP v[i];                                                          \
                                                                                        \
            return *this;                                                               \
        }
        
        VECTOR_ASSIGNMENT(+=)
        VECTOR_ASSIGNMENT(-=)
        VECTOR_ASSIGNMENT(*=)
        VECTOR_ASSIGNMENT(/=)
        
        #undef VECTOR_ASSIGNMENT
        
        //! assignment by scaling
        template < std::size_t N, typename T >
        const Vector<N,T> & Vector<N,T>::operator *= (const T &s)
        {
            for (std::size_t i = 0; i < N; ++i)
                e_[i] *= s;
            
            return *this;
        }
        
        //! assignment by inverse scaling
        template < std::size_t N, typename T >
        const Vector<N,T> & Vector<N,T>::operator /= (const T &s)
        {
            assert(s != 0); T inv = 1 / s;
            for (std::size_t i = 0; i < N; ++i)
                e_[i] *= inv;
            
            return *this;
        }
        
        //! const indexing
        template < std::size_t N, typename T >
        inline const T& Vector<N,T>::operator[] (std::size_t i) const
        {
            assert(i < N);
            return e_[i];
        }
        
        //! indexing
        template < std::size_t N, typename T >
        inline T& Vector<N,T>::operator[] (std::size_t i)
        {
            // invoke the const version of operator[]
            return const_cast<T&>(static_cast<const Vector&>(*this)[i]);
        }
        
        //! rotate about x-axis
        template < std::size_t N, typename T >
        const Vector<N,T> & Vector<N,T>::RotateX(Degree angle)
        {
            Radian s = sin(angle * Const<T>::TO_RAD);
            Radian c = cos(angle * Const<T>::TO_RAD);
            
            y() = c * y() - s * z();
            z() = c * z() + s * y();
            
            return *this;
        }
        
        //! rotate about y-axis
        template < std::size_t N, typename T >
        const Vector<N,T> & Vector<N,T>::RotateY(Degree angle)
        {
            Radian s = sin(angle * Const<T>::TO_RAD);
            Radian c = cos(angle * Const<T>::TO_RAD);
            
            x() = c * x() + s * z();
            z() = c * z() - s * x();
            
            return *this;
        }
        
        //! rotate about z-axis
        template < std::size_t N, typename T >
        const Vector<N,T> & Vector<N,T>::RotateZ(Degree angle)
        {
            Radian s = sin(angle * Const<T>::TO_RAD);
            Radian c = cos(angle * Const<T>::TO_RAD);
            
            x() = c * x() - s * y();
            y() = c * y() + s * x();
            
            return *this;
        }
        
        //! unitise vector
        template < std::size_t N, typename T >
        const Vector<N,T> & Vector<N,T>::Normalise()
        {
            return !IsZero(*this) ? *this /= Mag(*this) : *this;
        }
        
        //! unary minus / negation
        template < std::size_t N, typename T >
        const Vector<N,T> operator - (Vector<N,T> v)
        {
            return v *= T(-1);
        }
        
        //! component-wise arithmetic operators
        #define VECTOR_ARITHMETIC(OP)                                                   \
        template < std::size_t N, typename T, typename U >                              \
        const Vector<N,T> operator OP (Vector<N,T> v1, const Vector<N,U> &v2)           \
        {                                                                               \
            return v1 OP ## = v2;                                                       \
        }
        
        VECTOR_ARITHMETIC(+)
        VECTOR_ARITHMETIC(-)
        VECTOR_ARITHMETIC(*)
        VECTOR_ARITHMETIC(/)
        
        #undef VECTOR_ARITHMETIC
        
        //! multiplication by scalar
        template < std::size_t N, typename T, typename U >
        const Vector<N,T> operator * (Vector<N,T> v, const U &s)
        {
            return v *= s;
        }
        
        //! multiplication by scalar
        template < std::size_t N, typename T, typename U >
        const Vector<N,T> operator * (const U &s, Vector<N,T> v)
        {
            return v *= s;
        }
        
        //! division by scalar
        template < std::size_t N, typename T, typename U >
        const Vector<N,T> operator / (Vector<N,T> v, const U &s)
        {
            return v /= s;
        }
        
        //! component-wise relational operators
        #define VECTOR_COMPARISON(OP1,OP2)                                              \
        template < std::size_t N, typename T, typename U >                              \
        const bool operator OP1 (const Vector<N,T> &v1, const Vector<N,U> &v2)          \
        {                                                                               \
            for (std::size_t i = 0; i < N; ++i)                                         \
                if (v1[i] OP2 v2[i])                                                    \
                    return false;                                                       \
                                                                                        \
            return true;                                                                \
        }
        
        VECTOR_COMPARISON(<,>=)
        VECTOR_COMPARISON(<=,>)
        VECTOR_COMPARISON(>,<=)
        VECTOR_COMPARISON(>=,<)
        VECTOR_COMPARISON(==,!=)
        VECTOR_COMPARISON(!=,==)
        
        #undef VECTOR_COMPARISON
        
        //! modulus / length / magnitude
        template < std::size_t N, typename T >
        const T Mag(const Vector<N,T> &v)
        {
            return sqrt(Dot(v,v));
        }
        
        //! modulus squared (removes square root for faster comparisons)
        template < std::size_t N, typename T >
        const T MagSqr(const Vector<N,T> &v)
        {
            return Dot(v,v);
        }
        
        //! euclidean distance
        template < std::size_t N, typename T, typename U >
        const T Dist(const Vector<N,T> &v1, const Vector<N,U> &v2)
        {
            return Mag(v1-v2);
        }
        
        //! euclidean distance squared (again, for faster comparisons)
        template < std::size_t N, typename T, typename U >
        const T DistSqr(const Vector<N,T> &v1, const Vector<N,U> &v2)
        {
            return MagSqr(v1-v2);
        }
        
        //! randomise a vector within the given range
        template < std::size_t N, typename T >
        const Vector<N,T> Random(Vector<N,T> &v, const T &low, const T &high)
        {
            for (std::size_t i = 0; i < N; ++i)
                v[i] = Random(low, high);
            
            return v;
        }
        
        //! midpoint of two vectors
        template < std::size_t N, typename T, typename U >
        const Vector<N,T> Midpoint(const Vector<N,T> &v1, const Vector<N,U> &v2)
        {
            Vector<N,T> ret;
            for (std::size_t i = 0; i < N; ++i)
                ret[i] = (v1[i] + v2[i]) / T(2);
            
            return ret;
        }
        
        //! component-wise absolute value
        template < std::size_t N, typename T, typename U >
        const Vector<N,T> Abs(const Vector<N,T> &v)
        {
            Vector<N,T> ret;
            for (std::size_t i = 0; i < N; ++i)
                ret[i] = Abs(v[i]);
            
            return ret;
        }
        
        //! component-wise minimum vector
        template < std::size_t N, typename T, typename U >
        const Vector<N,T> Min(const Vector<N,T> &v1, const Vector<N,U> &v2)
        {
            Vector<N,T> ret;
            for (std::size_t i = 0; i < N; ++i)
                ret[i] = Min(v1[i], v2[i]);
            
            return ret;
        }
        
        //! component-wise maximum vector
        template < std::size_t N, typename T, typename U >
        const Vector<N,T> Max(const Vector<N,T> &v1, const Vector<N,U> &v2)
        {
            Vector<N,T> ret;
            for (std::size_t i = 0; i < N; ++i)
                ret[i] = Max(v1[i], v2[i]);
            
            return ret;
        }
        
        //! linear interpolation
        template < std::size_t N, typename T, typename U >
        const Vector<N,T> Lerp(const Vector<N,T> &v1, const Vector<N,U> &v2, const T &t)
        {
            return v1 + (v2 - v1) * t;
        }
        
        //! dot product
        template < std::size_t N, typename T, typename U >
        const T Dot(const Vector<N,T> &v1, const Vector<N,U> &v2)
        {
            T ret = 0;
            for (std::size_t i = 0; i < N; ++i)
                ret += v1[i] * v2[i];
            
            return ret;
        }
        
        //! 2D cross product
        template < typename T, typename U >
        const T Cross(const Vector<2,T> &v1, const Vector<2,U> &v2)
        {
            return v1[0] * v2[1] - v1[1] * v2[0];
        }
        
        //! 3D cross product
        template < std::size_t N, typename T, typename U >
        const Vector<3,T> Cross(const Vector<N,T> &v1, const Vector<N,U> &v2)
        {
            return Vector<3,T>(v1[1] * v2[2] - v1[2] * v2[1],
                               v1[2] * v2[0] - v1[0] * v2[2],
                               v1[0] * v2[1] - v1[1] * v2[0]);
        }
        
        //! scalar triple product / mixed product
        template < std::size_t N, typename T, typename U, typename V >
        const T ScaTrip(const Vector<N,T> &v1, const Vector<N,U> &v2, const Vector<N,V> &v3)
        {
            return Dot(v1,Cross(v2,v3));
        }
        
        //! vector triple product
        template < std::size_t N, typename T, typename U, typename V >
        const Vector<N,T> VecTrip(const Vector<N,T> &v1, const Vector<N,U> &v2, const Vector<N,V> &v3)
        {
            // lagrange's formula [a x (b x c) = b(a . c) - c(a . b)]
            return v2 * Dot(v1,v3) - v3 * Dot(v1,v2);
        }
        
        //! angle between vectors in radians
        template < std::size_t N, typename T >
        const T Angle(const Vector<N,T> &v1, const Vector<N,T> &v2)
        {
            return acos(Dot(v1,v2));
        }
        
        //! project a vector onto another
        template < std::size_t N, typename T, typename U >
        const Vector<N,T> Project(const Vector<N,T> &v1, const Vector<N,U> &v2)
        {
            assert(Dot(v2,v2) != 0);
            return (v2 * Dot(v1,v2)) / Dot(v2,v2);
        }
        
        //! reflect a vector about its normal
        template < std::size_t N, typename T, typename U >
        const Vector<N,T> Reflect(const Vector<N,T> &v, const Vector<N,U> &n)
        {
            return v - (T(2) * Dot(v,n) * n);
        }
        
        //! unitised vector copy
        template < std::size_t N, typename T >
        const Vector<N,T> Normalised(Vector<N,T> v)
        {
            return v.Normalise();
        }
        
        //! test for null vector
        template < std::size_t N, typename T >
        const bool IsZero(const Vector<N,T> &v)
        {
            for (std::size_t i = 0; i < N; ++i)
                if (!IsZero(v[i], Abs(v[i])))
                    return false;
            
            return true;
        }
        
        //! test for unit vector
        template < std::size_t N, typename T >
        const bool IsUnit(const Vector<N,T> &v)
        {
            return IsEqual(T(1), Mag(v), Mag(v));
        }
        
        //! test for parallel vectors
        template < std::size_t N, typename T, typename U >
        const bool IsParallel(const Vector<N,T> &v1, const Vector<N,U> &v2)
        {
            return IsZero(Cross(v1,v2));
        }
        
        //! test for perpendicular vectors
        template < std::size_t N, typename T, typename U >
        const bool IsPerp(const Vector<N,T> &v1, const Vector<N,U> &v2)
        {
            return IsZero(Dot(v1,v2), Abs(Dot(v1,v2)));
        }
        
        //! cast to a vector of different dimension
        template < std::size_t N, typename T, std::size_t O >
        const Vector<N,T> Cast(const Vector<O,T> &v)
        {
            Vector<N,T> ret;
            std::size_t i = 0;
            
            // copy existing elements
            for ( ; i < (N < O ? N : O); ++i)
                ret[i] = v[i];
            
            // fill any new elements
            for ( ; i < N; ++i)
                ret[i] = T(0);
            
            return ret;
        }
        
#ifdef MATHS_IO
        //! input a vector from console
        template < std::size_t N, typename T >
        std::istream & operator >> (std::istream &is, Vector<N,T> &v)
        {
            for (std::size_t i = 0; i < N; ++i) is >> v[i];
            return is;
        }
        
        //! output a vector to console
        template < std::size_t N, typename T >
        std::ostream & operator << (std::ostream &os, const Vector<N,T> &v)
        {
            os << "("; for (std::size_t i = 0; i < N-1; ++i) os << v[i] << ", "; os << v[N-1] << ")\n";
            return os;
        }
#endif
    }
}

#endif // VECTOR_INL_
