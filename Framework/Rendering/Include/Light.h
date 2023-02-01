/*!
    @file Light.h @author Joel Barrett @date 01/01/12 @brief A light struct.
*/

#ifndef FRAMEWORK_RENDERING_LIGHT_H
#define FRAMEWORK_RENDERING_LIGHT_H

#if _MSC_VER > 1000
    #pragma once
#endif

#include "Vector.h"

namespace Framework
{
    using namespace Maths;
    
    namespace Rendering
    {
        /*!
            A struct for directional, point or spot lights.
        */
        struct Light
        {
            bool on;

            Vector4f ambient;
            Vector4f diffuse;
            Vector4f specular;

            Vector3f position;
            Vector3f direction;
            Vector3f attenuation;

            float power;
        };
    }
}

#endif // FRAMEWORK_RENDERING_LIGHT_H
