/*!
    @file Scene.h @author Joel Barrett @date 01/01/12 @brief Bézier curve scene.
*/

#ifndef APPLICATION_SCENE_H
#define APPLICATION_SCENE_H

#if _MSC_VER > 1000
    #pragma once
#endif

#include "Track.h"
#include "Light.h"
#include "Colour.h"
#include "MS3DModel.h"
#include "Settings.h"
#include "Resource.h"

namespace Application
{
    using namespace Framework::Maths;
    using namespace Framework::OpenGL;

    /*!
        Class for initialising and arranging the scene.
    */
    class Scene : public OpenGLApp
    {
    public:
        ~Scene();

        //! loads settings from xml file and initialises each aspect of the scene
        void Init();

        //! enters the windows message loop - our "game loop"
        void Execute();

    private:
        void InitWindow();
        void InitEntities();
        void InitOpenGL();
        void InitDisplayLists();

        //! updates the state of any entities within the scene
        void Update(float dt);

        void Render();
        void RenderTrack();
        void RenderCPLines();
        void RenderCPSpheres();
        void RenderShips();

        //! overriden message router for this application
        LRESULT CALLBACK MsgRouter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        GLuint gridDisplayList_, sphereDisplayList_;

        Settings settings_;
        MS3DModel shipModel_;
        Camera camera_;
        Track track_;
        Light light_;
    };
}

#endif // APPLICATION_SCENE_H
