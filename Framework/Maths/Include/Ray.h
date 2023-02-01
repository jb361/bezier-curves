/*!
    @file Ray.h @author Joel Barrett @date 27/11/09 @brief A generic ray type.
*/

#ifndef RAY_H_
#define RAY_H_

#if _MSC_VER > 1000
    #pragma once
#endif

#include "Vector.h"

namespace Framework
{
    namespace Maths
    {
        /*!
            A class for rays in 2, 3 or 4 dimensions.
        */
        template < std::size_t N = 3, typename T = float >
        class Ray
        {
        public:
            Vector<N,T> m_Origin;
            Vector<N,T> m_Direction;
            
            T m_Depth;
            
        public:
            //! default/overloaded ctor
            Ray(const Vector<N,T> &o = Vector<N,T>::ZERO, const Vector<N,T> &d = Vector<N,T>::UNIT_Z)
                : m_Origin(o), m_Direction(d), m_Depth(0){}
            
            //! basic assignment
            const Ray& operator = (const Ray &r);
            
            //! ray-sphere collision test (note: ray direction must be normalised)
            T TestSphere(const Vector<3,T> &v, T r);
        };
    }
}

#include "..\source\Ray.inl"

#endif // RAY_H_
