/*!
    @file Camera.cpp @author Joel Barrett @date 01/01/12 @brief Euler angle camera.
*/

#include "Camera.h"

namespace Framework
{
    namespace Rendering
    {
        void Camera::SetView(const Vector3f &position, const Vector3f &look)
        {
            position_ = position;
            look_ = look;
        }

        void Camera::SetViewByMouse(float deltaX, float deltaY)
        {
            static float yaw = 180.0f, pitch = 180.0f, speed = 3.0f;

            yaw += deltaX / speed;
            pitch += deltaY / speed;

            // lock the pitch and yaw ranges from 0 to 360 degrees
            yaw = (yaw <= 0.0f) ? 360.0f : ((yaw >= 360.0f) ? 0.0f : yaw);
            pitch = (pitch <= 0.0f) ? 360.0f : ((pitch >= 360.0f) ? 0.0f : pitch);

            position_.x() = 24 * sin(yaw * Const<float>::TO_RAD);
            //position_.y() = 24 * cos(pitch * Const<float>::TO_RAD);
            position_.z() = 24 * cos(yaw * Const<float>::TO_RAD);
        }

        void Camera::SetViewToTarget(const Vector3f &targetPos, const Vector3f &cameraLook)
        {
            static float stiffness = 0.2f, dampening = 5.0f, mass = 20.0f;
            static Vector3f stretch, force, acceleration, velocity;

            // Hooke's law (F = -kx)
            stretch = (targetPos - position_) * stiffness;
            force = stretch - velocity * dampening;

            // Newton's 2nd law (F = ma)
            acceleration = force / mass;

            velocity += acceleration;
            position_ += velocity;
            look_ = cameraLook;
        }

        void Camera::ComputeFRU()
        {
            forward_ = Normalised(look_ - position_);
            right_ = Normalised(Cross(forward_, up_));
            down_ = Cross(forward_, right_); // Up
        }

        void Camera::Pitch(Radian angle)
        {
            // rotate the camera about its x-axis
            look_.y() += angle;

            // lock camera to the extremities of rotation
            if ((look_.y() - position_.y()) > maxPitch_) look_.y() = position_.y() + maxPitch_;
            else if ((look_.y() - position_.y()) < -maxPitch_) look_.y() = position_.y() - maxPitch_;
        }

        void Camera::Yaw(Radian angle)
        {
            // compute the current view vector
            Vector3f View = look_ - position_;

            // rotate the camera about its y-axis
            look_.x() = position_.x() + cos(angle) * View.x() - sin(angle) * View.z();
            look_.z() = position_.z() + sin(angle) * View.x() + cos(angle) * View.z();
        }

        void Camera::Roll(Radian angle)
        {
            // compute the current view vector
            Vector3f View = look_ - position_;

            // rotate the camera about its z-axis
            up_.x() = position_.x() + cos(angle) * View.x() - sin(angle) * View.y();
            up_.y() = position_.y() + sin(angle) * View.x() + cos(angle) * View.y();
        }
    }
}
