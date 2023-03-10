/*!
    @file Curve.h @author Joel Barrett @date 01/01/12 @brief Abstract base class for a curve.
*/

#ifndef FRAMEWORK_MATHS_CURVE_H
#define FRAMEWORK_MATHS_CURVE_H

#if _MSC_VER > 1000
    #pragma once
#endif

#include <vector>
#include "Vector.h"

namespace Framework
{
    namespace Maths
    {
        /*!
            A class for parametric curves in 2 or 3-dimensional space.
        */
        template < std::size_t N = 3, typename T = float >
        class Curve
        {
        public:
            Curve(std::size_t deg = 4, std::size_t res = 100)
                : degree_(deg), resolution_(res)
            {
                ctrlPoints_.reserve(deg);
                polylineVerts_.reserve(res);
            }
            virtual ~Curve(){}

            std::size_t GetDegree() const { return degree_; }
            std::size_t GetResolution() const { return resolution_; }

            const Vector<N,T> & GetCtrlPoint(std::size_t i) const { return ctrlPoints_[i]; }
            const Vector<N,T> & GetPolylineVert(std::size_t i) const { return polylineVerts_[i]; }

            void SetCtrlPoint(std::size_t i, const Vector<N,T> &v) { ctrlPoints_[i] = v; }

            virtual void Compute() = 0;

        protected:
            std::vector< Vector<N,T> > ctrlPoints_;
            std::vector< Vector<N,T> > polylineVerts_;

            std::size_t degree_; //!< number of control points
            std::size_t resolution_; //!< number of polyline vertices
        };
    }
}

#endif // FRAMEWORK_MATHS_CURVE_H
