/*!
    @file Matrix.inl @author Joel Barrett @date 27/11/09 @brief A generic matrix type.
*/

#ifndef MATRIX_INL_
#define MATRIX_INL_

#if _MSC_VER > 1000
    #pragma once
#endif

namespace Framework
{
    namespace Maths
    {
        //! identity matrix
        template < std::size_t R, std::size_t C, typename T >
        const Matrix<4,4,T> Matrix<R,C,T>::IDENTITY(1, 0, 0, 0,
                                                    0, 1, 0, 0,
                                                    0, 0, 1, 0,
                                                    0, 0, 0, 1);
        
        //! 2x2 overloaded ctor
        template < std::size_t R, std::size_t C, typename T >
        Matrix<R,C,T>::Matrix(const T &_00, const T &_01,
                              const T &_10, const T &_11)
        {
            (*this)[0][0] = _00; (*this)[0][1] = _01;
            (*this)[1][0] = _10; (*this)[1][1] = _11;
        }
        
        //! 3x3 overloaded ctor
        template < std::size_t R, std::size_t C, typename T >
        Matrix<R,C,T>::Matrix(const T &_00, const T &_01, const T &_02,
                              const T &_10, const T &_11, const T &_12,
                              const T &_20, const T &_21, const T &_22)
        {
            (*this)[0][0] = _00; (*this)[0][1] = _01; (*this)[0][2] = _02;
            (*this)[1][0] = _10; (*this)[1][1] = _11; (*this)[1][2] = _12;
            (*this)[2][0] = _20; (*this)[2][1] = _21; (*this)[2][2] = _22;
        }
        
        //! 4x4 overloaded ctor
        template < std::size_t R, std::size_t C, typename T >
        Matrix<R,C,T>::Matrix(const T &_00, const T &_01, const T &_02, const T &_03,
                              const T &_10, const T &_11, const T &_12, const T &_13,
                              const T &_20, const T &_21, const T &_22, const T &_23,
                              const T &_30, const T &_31, const T &_32, const T &_33)
        {
            (*this)[0][0] = _00; (*this)[0][1] = _01; (*this)[0][2] = _02; (*this)[0][3] = _03;
            (*this)[1][0] = _10; (*this)[1][1] = _11; (*this)[1][2] = _12; (*this)[1][3] = _13;
            (*this)[2][0] = _20; (*this)[2][1] = _21; (*this)[2][2] = _22; (*this)[2][3] = _23;
            (*this)[3][0] = _30; (*this)[3][1] = _31; (*this)[3][2] = _32; (*this)[3][3] = _33;
        }

        //! rotate about x-axis
        template < std::size_t R, std::size_t C, typename T >
        const Matrix<R,C,T> & Matrix<R,C,T>::RotateX(Degree angle)
        {
            Radian cosangle = cos(angle * Const<T>::TO_RAD);
            Radian sinangle = sin(angle * Const<T>::TO_RAD);
            
            (*this)[1][1] =  cosangle;
            (*this)[1][2] =  sinangle;
            (*this)[2][1] = -sinangle;
            (*this)[2][2] =  cosangle;
            
            return *this;
        }
        
        //! rotate about y-axis
        template < std::size_t R, std::size_t C, typename T >
        const Matrix<R,C,T> & Matrix<R,C,T>::RotateY(Degree angle)
        {
            Radian cosangle = cos(angle * Const<T>::TO_RAD);
            Radian sinangle = sin(angle * Const<T>::TO_RAD);
            
            (*this)[0][0] =  cosangle;
            (*this)[0][2] = -sinangle;
            (*this)[2][0] =  sinangle;
            (*this)[2][2] =  cosangle;
            
            return *this;
        }
        
        //! rotate about z-axis
        template < std::size_t R, std::size_t C, typename T >
        const Matrix<R,C,T> & Matrix<R,C,T>::RotateZ(Degree angle)
        {
            Radian cosangle = cos(angle * Const<T>::TO_RAD);
            Radian sinangle = sin(angle * Const<T>::TO_RAD);
            
            (*this)[0][0] =  cosangle;
            (*this)[0][1] =  sinangle;
            (*this)[1][0] = -sinangle;
            (*this)[1][1] =  cosangle;
            
            return *this;
        }
        
        //! scaling
        template < std::size_t R, std::size_t C, typename T >
        const Matrix<R,C,T> & Matrix<R,C,T>::Scale(const T &sx, const T &sy, const T &sz)
        {
            (*this)[0][0] = sx; (*this)[1][1] = sy; (*this)[2][2] = sz;
            return *this;
        }
        
        //! translation
        template < std::size_t R, std::size_t C, typename T >
        const Matrix<R,C,T> & Matrix<R,C,T>::Translate(const T &tx, const T &ty, const T &tz)
        {
            (*this)[0][3] = tx; (*this)[1][3] = ty; (*this)[2][3] = tz;
            return *this;
        }
        
        //! identity
        template < std::size_t R, std::size_t C, typename T >
        void Matrix<R,C,T>::Identity()
        {
           this = IDENTITY;
           return *this;
        }
        
        //! matrix-matrix product
        template < std::size_t R, std::size_t C, std::size_t N, typename T, typename U >
        inline const Matrix<R,C,T> operator * (const Matrix<R,C,T> &m1, const Matrix<C,N,U> &m2)
        {
            Matrix<R,C,T> ret;
            Matrix<N,C,T> t = Transpose(m2);
            
            for (std::size_t i = 0; i < R; ++i)
                for (std::size_t j = 0; j < C; ++j)
                    ret[i][j] = Dot(m1[i],t[j]);
            
            return ret;
        }
        
        //! matrix-row vector product (not commutative)
        template < std::size_t R, std::size_t C, typename T, typename U >
        inline const Vector<R,T> operator * (const Matrix<R,C,T> &m, const Vector<R,U> &v)
        {
            Vector<R,T> ret;
            
            for (std::size_t i = 0; i < R; ++i)
                ret[i] = Dot(m[i],v);
            
            return ret;
        }
        
        //! row vector-matrix product (not commutative)
        template < std::size_t R, std::size_t C, typename T, typename U >
        inline const Vector<R,T> operator * (const Vector<R,U> &v, const Matrix<R,C,T> &m)
        {
            Vector<N,T> ret;
            
            for (std::size_t i = 0; i < N; ++i)
                ret[i] += Dot(m[i],v);
            
            return ret;
        }
        
        //! transpose of a square matrix
        template < std::size_t D, typename T >
        const Matrix<D,D,T> Transpose(const Matrix<D,D,T> &m)
        {
            Matrix<D,D,T> ret;
            for (std::size_t i = 0; i < D; ++i)
                for (std::size_t j = 0; j < D; ++j)
                    ret[j][i] = m[i][j];
            
            return ret;
        }
        
        //! make all rows of a square matrix orthogonal and unit length
        template < std::size_t D, typename T >
        inline void Orthonormalise(Matrix<D,D,T> &m)
        {
            for (std::size_t i = 0; i < D; ++i)
            {
                for (std::size_t j = 0; j < i; ++j)
                    m[i] -= Project(m[i], m[j]);
                
                m[i].Normalise();
            }
        }
        
        //! get the matrix formed by removing a specified row and column
        template < std::size_t R, std::size_t C, typename T >
        inline const Matrix<R,C,T> Cofactor(const Matrix<R,C,T> &m, std::size_t row, std::size_t col)
        {
            Matrix<R-1,C-1,T> ret;
            for (std::size_t y = 0; y < R - 1; ++y)
            {
                for (std::size_t x = 0; x < C - 1; ++x)
                {
                    // if we are at or past the row or column removed, we offset by one
                    ret[y][x] = m[y < row ? y : y + 1][x < col ? x : x + 1];
                }
            }
            return ret;
        }
        
        //! determinant of a square matrix
        template < std::size_t D, typename T >
        inline const T Determinant(const Matrix<D,D,T> &m)
        {
            T ret(0);
            for (std::size_t i = 0; i < D; ++i)
            {
                // alternately add and subtract each element in the top row
                // times the determinant of the cofactor of that position
                T mul(i % 2 * -2 + 1);
                T a(mat[0][i]);
                T b(Determinant(Cofactor(mat, 0, i)));
                ret += mul * a * b;
            }
            return ret;
        }
        
        //! use cramer's method to compute inverse of a square matrix
        template < std::size_t D, typename T >
        inline const T Cramer(const Matrix<D,D,T> &m)
        {
            T det = Determinant(m);
            if (!det)
            {
                // Throw an exception.
                std::stringstream ss;
                ss << "Attempt to take inverse of a singular matrix.\n";
                for (std::size_t y = 0; y < Dim; ++y)
                {
                    ss << "{";
                    for (std::size_t x = 0; x < Dim; ++x)
                    {
                        ss << m[y][x] << (x + 1 < Dim ? ", " : "}\n");
                    }
                }
                throw std::logic_error(ss.str());
            }
            
            T flip = 1;
            Matrix<D,D,T> ans;
            for (std::size_t y = 0; y < D; ++y)
            {
                for (std::size_t x = 0; x < D; ++x)
                {
                    // Note: x and y are swapped in the cofactor
                    // call so that we generate the transpose of
                    // the matrix of cofactor determinants.
                    matrix<D-1,D-1,T> c(Cofactor(m, x, y));
                    flip = ((x + y) % 2) * Type(-2) + 1;
                    ans[y][x] = determinant(c) * flip;
                }
            }
            return ans / det;
        }
        
#ifdef MATHS_IO
        //! input a matrix from console
        template < std::size_t R, std::size_t C, typename T >
        inline std::istream & operator >> (std::istream &is, Matrix<R,C,T> &m)
        {
            for (std::size_t i = 0; i < R; ++i) is >> m[i];
            return is;
        }
        
        //! output a matrix to console
        template < std::size_t R, std::size_t C, typename T >
        inline std::ostream & operator << (std::ostream& os, Matrix<R,C,T> &m)
        {
            for (std::size_t i = 0; i < R; ++i) os << m[i];
            return os;
        }
#endif
    }
}

#endif // MATRIX_INL_
