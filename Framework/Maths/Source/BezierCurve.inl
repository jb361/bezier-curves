/*!
    @file BezierCurve.inl @author Joel Barrett @date 01/01/12 @brief A cubic bezier curve.
*/

namespace Framework
{
    namespace Maths
    {
        template < std::size_t N, typename T >
        BezierCurve<N,T>::BezierCurve(): arcLength_(0.0f)
        {
            for (std::size_t i = 0; i < 4; ++i) {
                ctrlPoints_.push_back(Vector<N,T>::ZERO);
            }
            SetResolution(75);
            Compute();
        }
        
        template < std::size_t N, typename T >
        BezierCurve<N,T>::BezierCurve(const Vector<N,T> &a, const Vector<N,T> &b, const Vector<N,T> &c, 
                                      const Vector<N,T> &d, std::size_t res): Curve(4,res), arcLength_(0.0f)
        {
            SetCtrlPoints(a, b, c, d);
            SetResolution(res);
            
            Compute();
        }
        
        template < std::size_t N, typename T >
        void BezierCurve<N,T>::SetCtrlPoints(const Vector<N,T> &a, const Vector<N,T> &b, 
                                             const Vector<N,T> &c, const Vector<N,T> &d)
        {
            ctrlPoints_.clear();
            
            ctrlPoints_.push_back(a);
            ctrlPoints_.push_back(b);
            ctrlPoints_.push_back(c);
            ctrlPoints_.push_back(d);
        }
        
        template < std::size_t N, typename T >
        void BezierCurve<N,T>::SetResolution(std::size_t res)
        {
            assert(res > 0);
            resolution_ = res;
            
            h1_ = 1.0f / res;
            h2_ = h1_ * h1_;
            h3_ = h2_ * h1_;
        }
        
        template < std::size_t N, typename T >
        void BezierCurve<N,T>::Compute()
        {
            // polynomial coefficients
            d_ = ctrlPoints_[0];
            c_ = 3.0f * (ctrlPoints_[1] - ctrlPoints_[0]);
            b_ = 3.0f * (ctrlPoints_[2] - ctrlPoints_[1]) - c_;
            a_ = ctrlPoints_[3] - ctrlPoints_[0] - b_ - c_;
            
            // first derivative coefficients
            da_ = 3.0f * a_;
            db_ = b_ + b_;
            
            // forward differences
            Vector<N,T> fd3 = 6.0f * a_ * h3_;
            Vector<N,T> fd2 = fd3 + 2.0f * b_ * h2_;
            Vector<N,T> fd1 = a_ * h3_ + b_ * h2_ + c_ * h1_;
            Vector<N,T> fd0 = d_;
            
            polylineVerts_.clear();
            polylineVerts_.push_back(fd0);
            
            // compute the curve as a polyline
            for (std::size_t i = 1; i < resolution_; ++i)
            {
                fd0 += fd1;
                fd1 += fd2;
                fd2 += fd3;
                
                polylineVerts_.push_back(fd0);
            }
            ArcLength();
        }
        
        template < std::size_t N, typename T >
        void BezierCurve<N,T>::deCasteljau(T t, Vector<N,T>* v)
        {
            //  a | b
            // ---|---  c = a + (b-a)*t (0 ≤ t ≤ 1)
            //  c |
            assert(t >= 0.0f && t <= 1.0f);
            
            // 1st row
            v[0] = Lerp(ctrlPoints_[0], ctrlPoints_[1], t);
            v[1] = Lerp(ctrlPoints_[1], ctrlPoints_[2], t);
            v[2] = Lerp(ctrlPoints_[2], ctrlPoints_[3], t);
            
            // 2nd row
            v[3] = Lerp(v[0], v[1], t);
            v[4] = Lerp(v[1], v[2], t);
            
            // 3rd row
            v[5] = Lerp(v[3], v[4], t);
        }
        
        template < std::size_t N, typename T >
        void BezierCurve<N,T>::Split(T t, Vector<N,T>* left, Vector<N,T>* right)
        {
            assert(t >= 0.0f && t <= 1.0f);
            
            Vector<N,T> v[6];
            deCasteljau(t,v);
            
            // curve to left of split
            left[3] = v[5]; left[2] = v[3];
            left[1] = v[0]; left[0] = ctrlPoints_[0];
            
            // curve to right of split
            right[0] = v[5]; right[1] = v[4];
            right[2] = v[2]; right[3] = ctrlPoints_[3];
        }
        
        template < std::size_t N, typename T >
        inline Vector<N,T> BezierCurve<N,T>::PointAt(T t)
        {
            assert(t >= 0.0f && t <= 1.0f);
            return (((a_*t) + b_)*t + c_)*t + d_;
		}
        
        template < std::size_t N, typename T >
        inline Vector<N,T> BezierCurve<N,T>::TangentAt(T t)
        {
            assert(t >= 0.0f && t <= 1.0f);
            return Normalised(((da_*t) + db_)*t + c_);
        }
        
		template < std::size_t N, typename T >
        void BezierCurve<N,T>::ArcLength()
        {
            // distance around control polygon
            T PolyLength = Dist(ctrlPoints_[0], ctrlPoints_[1]) + Dist(ctrlPoints_[1], ctrlPoints_[2])
                + Dist(ctrlPoints_[2], ctrlPoints_[3]);
            
            // distance between endpoints
            T ChordLength = Dist(ctrlPoints_[0], ctrlPoints_[3]);

            arcLength_ = 0.5f * (PolyLength + ChordLength);
        }
    }
}
