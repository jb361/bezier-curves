/*!
    @file Track.h @author Joel Barrett @date 01/01/12 @brief A bezier spline track.
*/

#ifndef APPLICATION_TRACK_H
#define APPLICATION_TRACK_H

#if _MSC_VER > 1000
    #pragma once
#endif

#include <vector>

#include "BezierCurve.h"
#include "OpenGLApp.h"
#include "Camera.h"
#include "Ray.h"

namespace Application
{
    using namespace Framework::Maths;
    using namespace Framework::Rendering;

    /*!
        A piecewise bezier curve track.
    */
    class Track
    {
        struct Ship
        {
            Ship(const Point3f &p, const Vector3f &h)
                : t(0.0f), speed(2.0f), up(Vector3f::UNIT_Y), pos(p), heading(h), currentCurve(0){}

            float t, speed;
            Vector3f up, pos, oldpos, heading, oldhead;
            unsigned int currentCurve;
        };

    public:
        Track();

        void AddFirstCurve(const Vector3f &a, const Vector3f &b, const Vector3f &c, const Vector3f &d);
        void AddCurveToEnd(const Vector3f &c, const Vector3f &d);
        void AddLastCurve();
        void AddCurve(); //! add curve to circuit by splitting longest curve

        void AddShip();
        void RemoveShip();
        void Update(float dt);

        // accessor methods
        const BezierCurve<> & GetCurve(std::size_t i) const { assert(i < curves_.size()); return curves_[i]; }
        const Ship & GetShip(std::size_t i) const { assert(i < ships_.size()); return ships_[i]; }
        const float GetCtrlPointRadius() const { return ctrlPointRadius_; }
        std::size_t GetNumCurves() const { return curves_.size(); }
        std::size_t GetNumShips() const { return ships_.size(); }
        std::size_t GetResolution() const { return curves_.front().GetResolution(); }
        float GetLength() const { return length_; }

        void IncResolution();
        void DecResolution();
        void SetCtrlPointSelected(bool isSelected) { ctrlPointSelected_ = isSelected; }
        bool IsCtrlPointSelected() const { return ctrlPointSelected_; }

        // allow the user to pick and move a CP with the mouse
        void SelectCtrlPoint(const Camera &cam, unsigned width, unsigned height, int x, int y);
        void DragCtrlPoint(int x, int y);

    private:
        //! bezier curves making up the track
        std::vector< BezierCurve<> > curves_;

        //! ships locked to the track
        std::vector< Ship > ships_;

        //! cumulative arc lengths of curves
        float length_;

        const float ctrlPointRadius_; //!< radius of control point spheres
        bool ctrlPointSelected_; //!< is one of the curve's control points selected?

        unsigned int selectedCurve_, affectedCurve_, selectedCtrlPoint_;
        float zDistToPixel_; //!< range [0:1]

        std::size_t resolution_;
        Vector4i viewport_;
    };
}

#endif // APPLICATION_TRACK_H
