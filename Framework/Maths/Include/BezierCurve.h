/*!
    @file BezierCurve.h @author Joel Barrett @date 01/01/12 @brief A generic cubic bezier curve.
*/

#ifndef FRAMEWORK_MATHS_BEZIERCURVE_H
#define FRAMEWORK_MATHS_BEZIERCURVE_H

#if _MSC_VER > 1000
    #pragma once
#endif

#include "Curve.h"

namespace Framework
{
    namespace Maths
    {
        /*!
            A class for cubic bezier curves in 2 or 3-dimensional space.
        */
        template < std::size_t N = 3, typename T = float >
        class BezierCurve : public Curve < N,T >
        {
        public:
            BezierCurve();
            BezierCurve(const Vector<N,T> &a, const Vector<N,T> &b, const Vector<N,T> &c, 
                const Vector<N,T> &d, std::size_t res = 75);
            
            const T& GetLength() const { return arcLength_; }
            
            void SetCtrlPoints(const Vector<N,T> &a, const Vector<N,T> &b, 
                const Vector<N,T> &c, const Vector<N,T> &d);

            void SetResolution(std::size_t res);
            
            void Compute();
            
            //! recursive subdivision
            void deCasteljau(T t, Vector<N,T>* v);
            
            //! split the curve using de Casteljau's method
            void Split(T t, Vector<N,T>* left, Vector<N,T>* right);
            
            //! find a point on curve using Horner's scheme
            Vector<N,T> PointAt(T t);
            
            //! find tangent at point by differentation
            Vector<N,T> TangentAt(T t);
            
        private:
            //! approximate arc length with Jens Gravesen's method
            void ArcLength();
            
        private:
            // polynomial coefficients
            Vector<N,T> a_, b_, c_, d_;
            Vector<N,T> da_, db_;
            
            // polyline length
            T arcLength_;

            // strides
            T h1_, h2_, h3_;
        };
    }
}

#include "..\Source\BezierCurve.inl"

#endif // FRAMEWORK_MATHS_BEZIERCURVE_H
