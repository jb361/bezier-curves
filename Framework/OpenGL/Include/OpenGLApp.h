/*!
    @file OpenGLApp.h @author Joel Barrett @date 01/01/12 @brief OpenGL application wrapper.
*/

#ifndef FRAMEWORK_OPENGL_OPENGLAPP_H
#define FRAMEWORK_OPENGL_OPENGLAPP_H

#if _MSC_VER > 1000
    #pragma once
#endif

#include "glew.h"
#include "wglext.h"
#include "Window.h"

namespace Framework
{
    namespace OpenGL
    {
        /*!
            A lightweight OpenGL application wrapper.
        */
        class OpenGLApp : public Rendering::Win32Window
        {
        protected:
            OpenGLApp(): hDC_(NULL), hRC_(NULL){}
            ~OpenGLApp();

            //! clear opengl resources
            void Release();

            //! alter opengl when window is resized
            void Resize(int w, int h);

            //! initialise opengl with MSAA (if supported)
            bool InitMultisample(int &pf);

            //! set pixel format descriptor
            void SetPixelFormatDescriptor(int pf);

            //! determine whether a particular extension is supported
            bool IsExtensionSupported(char* ext);

        protected:
            HDC	hDC_; //!< private GDI device context
            HGLRC hRC_; //!< permanent rendering context
        };
    }
}

#endif // FRAMEWORK_OPENGL_OPENGLAPP_H
