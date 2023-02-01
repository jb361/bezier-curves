/*!
    @file Settings.h @author Joel Barrett @date 01/01/12 @brief Settings for the application.
*/

#ifndef APPLICATION_SETTINGS_H
#define APPLICATION_SETTINGS_H

#if _MSC_VER > 1000
    #pragma once
#endif

#include <vector>
#include <string>

#define TIXML_USE_STL
#include "tinyxml.h"
#include "Vector.h"

namespace Application
{
    using namespace Framework::Maths;

    struct CurveSettings
    {
        std::vector< Vector3f > ctrlPoints;
    };

    struct WindowSettings
    {
        std::string title;
        int xPos, yPos, width, height;
    };

    struct LightSettings
    {
        Vector4f ambient, diffuse;
        Vector3f position;
    };

    struct CameraSettings
    {
        Vector3f position, lookAt;
    };

    class Settings
    {
    public:
        void Load(const char* pFilename);

    public:
        std::string name_;
        WindowSettings window_;
        std::string modelFilename_;
        CameraSettings camera_;
        std::vector< CurveSettings > track_;
        LightSettings light_;
    };
}

#endif // APPLICATION_SETTINGS_H
