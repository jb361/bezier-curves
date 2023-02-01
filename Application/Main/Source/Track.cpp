/*!
    @file Track.cpp @author Joel Barrett @date 01/01/12 @brief A bezier spline track.
*/

#include "Track.h"

namespace Application
{
    Track::Track(): length_(0.0f), resolution_(75), ctrlPointRadius_(0.24f), ctrlPointSelected_(false)
    {
        curves_.reserve(4);
    }

    void Track::AddFirstCurve(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Vector3f &d)
    {
        assert(curves_.empty());

        curves_.push_back(BezierCurve<>(a, b, c, d, resolution_));
        length_ += curves_.back().GetLength();
    }

    void Track::AddCurveToEnd(const Vector3f &c, const Vector3f &d)
    {
        assert(!curves_.empty());

        // compute 2nd control point whilst maintaining C1 continuity
        Vector3f b = curves_.back().GetCtrlPoint(3) + curves_.back().GetCtrlPoint(3) - curves_.back().GetCtrlPoint(2);

        curves_.push_back(BezierCurve<>(curves_.back().GetCtrlPoint(3), b, c, d, resolution_));
        length_ += curves_.back().GetLength();
    }

    void Track::AddLastCurve()
    {
        assert(!curves_.empty());

        // compute 2nd and 3rd control points whilst maintaining C1 continuity
        Vector3f b = curves_.back().GetCtrlPoint(3) + curves_.back().GetCtrlPoint(3) - curves_.back().GetCtrlPoint(2);
        Vector3f c = curves_.front().GetCtrlPoint(0) + curves_.front().GetCtrlPoint(0) - curves_.front().GetCtrlPoint(1);

        curves_.push_back(BezierCurve<>(curves_.back().GetCtrlPoint(3), b, c, curves_.front().GetCtrlPoint(0), resolution_));
        length_ += curves_.back().GetLength();
    }

    void Track::AddCurve()
    {
        assert(!curves_.empty());
        
        // iterate through list to find curve with greatest arc length
        std::size_t i = 1, longestCurveIndex = 0;
        std::vector< BezierCurve<> >::iterator longestCurve = curves_.begin();
        for (std::vector< BezierCurve<> >::iterator it = ++curves_.begin(); it != curves_.end(); ++it, ++i)
        {
            if (it->GetLength() > longestCurve->GetLength())
            {
                longestCurve = it;
                longestCurveIndex = i;
            }
        }
        // maintain the current position of any ships affected
        for (std::vector< Ship >::iterator it = ships_.begin(); it != ships_.end(); ++it)
        {
            // if ship is on the curve that will be split
            if (it->currentCurve == longestCurveIndex)
            {
                if (it->t < 0.5f) {
                    it->t *= 2.0f;
                }
                else {
                    it->t = (it->t - 0.5f) * 2.0f; 
                    it->currentCurve++; 
                }
            }
            // if ship is beyond the curve that will be split
            else if (it->currentCurve > longestCurveIndex) {
                it->currentCurve++;
            }
        }

        // roughly bisect the longest curve using de Casteljau's method
        Vector3f left[4], right[4];

        longestCurve->Split(0.5f, left, right);
        longestCurve->SetCtrlPoints(right[0], right[1], right[2], right[3]);
        longestCurve->Compute();

        curves_.insert(longestCurve, BezierCurve<>(left[0], left[1], left[2], left[3], resolution_));
    }

    void Track::AddShip()
    {
        ships_.push_back(Ship(curves_.front().GetCtrlPoint(0), 
            curves_.front().TangentAt(0.0f)));
    }

    void Track::RemoveShip()
    {
        if (!ships_.empty()) {
            ships_.pop_back();
        }
    }

    void Track::Update(float dt)
    {
        static const float gravity = 14.0f;

        for (std::vector< Ship >::iterator it = ships_.begin(); it != ships_.end(); ++it)
        {
            // distance = speed * time
            float distance = it->speed * dt;

            // energy = 1/2*mv^2 + mgh (mass = 1)
            float energystart = 0.5f * Sqr(it->speed) + gravity * it->pos.y();

            while (distance > 0.0f)
            {
                it->t += 0.0001f;

                // if ship has reached end of current curve
                if (it->t > 1.0f)
                {
                    it->currentCurve++;
                    it->t = 0.0f;

                    // if ship has reached end of piecewise curve
                    if (it->currentCurve == curves_.size())
                    {
                        it->currentCurve = 0;
                        distance = 0.0f;
                    }
                }
                Vector3f newpos = curves_[it->currentCurve].PointAt(it->t);
                distance -= Mag(newpos - it->pos);

                it->pos = newpos;
                it->heading = curves_[it->currentCurve].TangentAt(it->t);
            }

            // by conservation of energy, v^2 = 2*(energy - gh)
            float vsqr = 2 * (energystart - gravity * it->pos.y());

            // test for positive square, and limit the minimum speed to 2
            //if (!constantSpeed)
            it->speed = (vsqr > 4) ? sqrt(vsqr) : 2;
        }
    }

    void Track::IncResolution()
    {
        if (resolution_ < 75)
        {
            ++resolution_;
            for (std::vector< BezierCurve<> >::iterator it = curves_.begin(); it != curves_.end(); ++it)
            {
                it->SetResolution(resolution_);
                it->Compute();
            }
        }
    }

    void Track::DecResolution()
    {
        if (resolution_ > 2)
        {
            --resolution_;
            for (std::vector< BezierCurve<> >::iterator it = curves_.begin(); it != curves_.end(); ++it)
            {
                it->SetResolution(resolution_);
                it->Compute();
            }
        }
    }

    void Track::SelectCtrlPoint(const Camera &cam, unsigned width, unsigned height, int x, int y)
    {
        static float redValue = 0.0f;
        glGetIntegerv(GL_VIEWPORT, &viewport_[0]);
        GLint gl_y = viewport_[3] - y - 1;

        glReadPixels(x, gl_y, 1, 1, GL_RED, GL_FLOAT, &redValue);
        glReadPixels(x, gl_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zDistToPixel_);

        if (redValue)
        {
            // compute the direction vector of ray in world space
            Vector3f temp = Normalised(ScreenToViewX<float>((float)x, (float)width, (float)height) * -cam.GetRight() + 
                                       ScreenToViewY<float>((float)y, (float)width, (float)height) * cam.GetDown() - cam.GetForward());

            Ray<3> ray(cam.GetPosition(), temp);
            float dmax = 1000.0f;

            for (std::size_t i = 0; i < curves_.size(); ++i)
            {
                for (std::size_t j = 0; j < 3; ++j)
                {
                    // test for a collision between ray and a control point sphere
                    if (ray.TestSphere(curves_[i].GetCtrlPoint(j), ctrlPointRadius_) && ray.m_Depth < dmax)
                    {
                        // pick the control point closest to viewer if there's multiple hits
                        dmax = ray.m_Depth;

                        ctrlPointSelected_ = true;
                        selectedCurve_ = i;
                        selectedCtrlPoint_ = j;
                    }
                }
            }
        }
    }

    void Track::DragCtrlPoint(int x, int y)
    {
        // if camera isn't too close to the sphere
        if (zDistToPixel_ > 0.8f)
        {
            static Vector3f dir;
            static Vector3d move;
            static Matrix4x4d view, proj;
            static float dist = 0.0f;

            // get view matrix and projection matrix
            glGetDoublev(GL_MODELVIEW_MATRIX, &view[0][0]);
            glGetDoublev(GL_PROJECTION_MATRIX, &proj[0][0]);

            // transform mouse coordinates into world space
            gluUnProject(x, viewport_[3] - y - 1, zDistToPixel_, &view[0][0], &proj[0][0], 
                &viewport_[0], &move.x(), &move.y(), &move.z());

            switch (selectedCtrlPoint_)
            {
            case 0: // 1st CP

                if (Dist(move, curves_[selectedCurve_ != 0 ? selectedCurve_ - 1 : GetNumCurves() - 1].GetCtrlPoint(0)) < 50.0f)
                {
                    // determine the other curve affected
                    affectedCurve_ = selectedCurve_ != 0 ? selectedCurve_ - 1 : GetNumCurves() - 1;

                    // compute the distance and direction to move the affected CPs
                    dist = Dist(curves_[selectedCurve_].GetCtrlPoint(0), curves_[selectedCurve_].GetCtrlPoint(1));
                    dir = Normalised(curves_[selectedCurve_].GetCtrlPoint(0) - curves_[selectedCurve_].GetCtrlPoint(1));

                    // move the selected and left control point
                    curves_[selectedCurve_].SetCtrlPoint(0, move);
                    curves_[selectedCurve_].SetCtrlPoint(1, curves_[selectedCurve_].GetCtrlPoint(0) - dir * dist);
                    curves_[selectedCurve_].Compute();

                    // move the selected and right control point
                    curves_[affectedCurve_].SetCtrlPoint(3, move);
                    curves_[affectedCurve_].SetCtrlPoint(2, curves_[selectedCurve_].GetCtrlPoint(0) + dir * dist);
                    curves_[affectedCurve_].Compute();
                }
                break;

            case 1: // 2nd CP

                if (Dist(move, curves_[selectedCurve_].GetCtrlPoint(0)) < 25.0f)
                {
                    // determine the other affected curve and move the selected CP
                    affectedCurve_ = selectedCurve_ != 0 ? selectedCurve_ - 1 : GetNumCurves() - 1;
                    curves_[selectedCurve_].SetCtrlPoint(1, move);

                    // compute the distance and direction to move the affected CP
                    dist = Dist(curves_[selectedCurve_].GetCtrlPoint(0), curves_[selectedCurve_].GetCtrlPoint(1));
                    dir = Normalised(curves_[selectedCurve_].GetCtrlPoint(0) - curves_[selectedCurve_].GetCtrlPoint(1));

                    // move the right control point and recompute affected curves
                    curves_[affectedCurve_].SetCtrlPoint(2, curves_[selectedCurve_].GetCtrlPoint(0) + dir * dist);
                    curves_[affectedCurve_].Compute(); curves_[selectedCurve_].Compute();
                }
                break;

            case 2: // 3rd CP

                if (Dist(move, curves_[selectedCurve_].GetCtrlPoint(3)) < 25.0f)
                {
                    // determine the other affected curve and move the selected CP
                    affectedCurve_ = selectedCurve_ != GetNumCurves() - 1 ? selectedCurve_ + 1 : 0;
                    curves_[selectedCurve_].SetCtrlPoint(2, move);

                    // compute the distance and direction to move the affected CP
                    dist = Dist(curves_[selectedCurve_].GetCtrlPoint(3), curves_[selectedCurve_].GetCtrlPoint(2));
                    dir = Normalised(curves_[selectedCurve_].GetCtrlPoint(3) - curves_[selectedCurve_].GetCtrlPoint(2));

                    // move the left control point and recompute affected curves
                    curves_[affectedCurve_].SetCtrlPoint(1, curves_[selectedCurve_].GetCtrlPoint(3) + dir * dist);
                    curves_[affectedCurve_].Compute(); curves_[selectedCurve_].Compute();
                }
                break;
            }
        }
    }
}
