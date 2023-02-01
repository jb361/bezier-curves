/*!
    @file OpenGLApp.cpp @author Joel Barrett @date 01/01/12 @brief OpenGL application wrapper.
*/

#include "OpenGLApp.h"

namespace Framework
{
    namespace OpenGL
    {
        OpenGLApp::~OpenGLApp()
        {
            Release();
        }

        void OpenGLApp::Release()
        {
            if (hDC_)
            {
                if (hRC_)
                {
                    wglMakeCurrent(hDC_, NULL);
                    wglDeleteContext(hRC_);
                    hRC_ = NULL;
                }
                ReleaseDC(hwnd_, hDC_);
                hDC_ = NULL;
            }
        }

        void OpenGLApp::Resize(int w, int h)
        {
            glViewport(0, 0, (GLsizei)w, (GLsizei)h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 0.1, 500.0);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }

        bool OpenGLApp::InitMultisample(int &pf)
        {
            PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = 
                (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

            if (wglChoosePixelFormatARB)
            {
                unsigned int n = 0;
                int attributes[] = { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                                     WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                                     WGL_ACCELERATION_ARB,
                                     WGL_FULL_ACCELERATION_ARB,
                                     WGL_COLOR_BITS_ARB, 16,
                                     WGL_ALPHA_BITS_ARB, 8,
                                     WGL_DEPTH_BITS_ARB, 16,
                                     WGL_STENCIL_BITS_ARB, 0,
                                     WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
                                     WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
                                     WGL_SAMPLES_ARB, 8, 0 };

                BOOL bResult = wglChoosePixelFormatARB(hDC_, attributes, NULL, 1, &pf, &n);
                if (bResult && n > 0) {
                    return true;
                }
            }
            return false;
        }

        void OpenGLApp::SetPixelFormatDescriptor(int pf)
        {
            PIXELFORMATDESCRIPTOR pfd = {0};

            pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.iPixelType = PFD_TYPE_RGBA;
            pfd.cColorBits = 24;
            pfd.cDepthBits = 24;

            hDC_ = GetDC(GetHandle());
            if (!pf) {
                pf = ChoosePixelFormat(hDC_, &pfd);
            }
            SetPixelFormat(hDC_, pf, &pfd);
            hRC_ = wglCreateContext(hDC_);
            wglMakeCurrent(hDC_, hRC_);
        }

        // taken from http://www.dhpoware.com
        bool OpenGLApp::IsExtensionSupported(char* ext)
        {
            static const char* GLExtensions = 0;
            static const char* WGLExtensions = 0;

            if (!GLExtensions) {
                GLExtensions = reinterpret_cast< const char *>(glGetString(GL_EXTENSIONS));
            }
            if (!WGLExtensions)
            {
                // WGL_ARB_extensions_string
                typedef const char* (WINAPI* PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC);

                PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB =
                    reinterpret_cast< PFNWGLGETEXTENSIONSSTRINGARBPROC >
                    (wglGetProcAddress("wglGetExtensionsStringARB"));

                if (wglGetExtensionsStringARB) {
                    WGLExtensions = wglGetExtensionsStringARB(wglGetCurrentDC());
                }
            }

            if (!strstr(GLExtensions, ext) && !strstr(WGLExtensions, ext)) {
                return false;
            }
            return true;
        }
    }
}
