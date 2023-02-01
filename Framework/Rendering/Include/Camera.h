/*!
    @file Camera.h @author Joel Barrett @date 01/01/12 @brief Euler angle camera.
*/

#ifndef FRAMEWORK_RENDERING_CAMERA_H
#define FRAMEWORK_RENDERING_CAMERA_H

#if _MSC_VER > 1000
    #pragma once
#endif

#include "Vector.h"
#include "Matrix.h"

namespace Framework
{
    namespace Rendering
    {
        using namespace Maths;

        /*!
            A simple Euler angle camera.
        */
        class Camera
        {
            typedef float Degree, Radian;

        public:
            enum CameraMode { CAMERA_MODE_GOD, CAMERA_MODE_1ST, CAMERA_MODE_3RD };

            Camera(): position_(0.0f, 0.0f, 0.0f), look_(0.0f, 0.0f, 1.0f), up_(0.0f, 1.0f, 0.0f), 
                down_(0.0f, -1.0f, 0.0f), forward_(0.0f, 0.0f, 1.0f), right_(1.0f, 0.0f, 0.0f), 
                maxPitch_(60.0f), mode_(CAMERA_MODE_GOD){}

            void SetView(const Vector3f &position, const Vector3f &look);
            void SetViewByMouse(float deltaX, float deltaY);

            //! track an entity by attaching a virtual spring from camera->object
            void SetViewToTarget(const Vector3f &targetPos, const Vector3f &cameraLook);
            
            const Vector3f & GetPosition() const { return position_; }
            const Vector3f & GetLook() const { return look_; }
            const Vector3f & GetUp() const { return up_; }
            const Vector3f & GetDown() const { return down_; }
            const Vector3f & GetForward() const { return forward_; }
            const Vector3f & GetRight() const { return right_; }

            void SetMode(CameraMode mode) { mode_ = mode; }
            const CameraMode & GetMode() const { return mode_; }
            bool ModeEquals(CameraMode mode) const { return mode_ == mode; }

            void ComputeFRU();

            //! rotate camera about its x, y or z-axis respectively
            void Pitch(Radian angle);
            void Yaw(Radian angle);
            void Roll(Radian angle);

        private:
            float maxPitch_;

            Vector3f position_, look_, up_, down_, forward_, right_;
            Matrix4x4f view_, proj_;

            CameraMode mode_;
        };
    }
}

#endif // FRAMEWORK_RENDERING_CAMERA_H
