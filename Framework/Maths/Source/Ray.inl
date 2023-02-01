/*!
    @file Ray.inl @author Joel Barrett @date 27/11/09 @brief A generic ray type.
*/

#ifndef RAY_INL_
#define RAY_INL_

#if _MSC_VER > 1000
    #pragma once
#endif

namespace Framework
{
    namespace Maths
    {
        //! basic assignment
        template < std::size_t N, typename T >
        const Ray<N,T> & Ray<N,T>::operator = (const Ray<N,T> &r)
        {
            if (this != &r)
            {
                m_Origin = r.m_Origin;
                m_Direction = r.m_Direction;
            }
            return *this;
        }
        
        //! ray-sphere collision test (note: ray direction must be normalised)
        template < std::size_t N, typename T >
        T Ray<N,T>::TestSphere(const Vector<3,T> &s, T r)
        {
            Vector<N,T> dist = m_Origin - s;
            
            T b = Dot(dist, m_Direction);
            T c = Dot(dist, dist) - Sqr(r);
            T d = Sqr(b) - c;
            
            return m_Depth = (d >= 0.0f) ? b + sqrt(d) : 0;
        }
    }
}

#endif // RAY_INL_
