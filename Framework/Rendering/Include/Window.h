/*!
    @file Window.h @author Joel Barrett @date 12/04/09 @brief 32-bit window class.
*/

#ifndef FRAMEWORK_RENDERING_WINDOW_H
#define FRAMEWORK_RENDERING_WINDOW_H

#if _MSC_VER > 1000
    #pragma once
#endif

#ifndef VC_EXTRALEAN
    #define VC_EXTRALEAN
#endif

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <windowsx.h>

namespace Framework
{
    namespace Rendering
    {
        void SetProcessorAffinity();

        class Win32Window
        {
        protected:
            Win32Window(): windowVisible_(true),
                hinst_(GetModuleHandle(NULL)),
                hwnd_(NULL){}

            //! virtual message router that the derived class supplies
            virtual LRESULT CALLBACK MsgRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

            //! static message router that the WNDCLASSEX structure needs access to
            static LRESULT CALLBACK st_MsgRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

            void SetTitle(LPCTSTR lpszTitle);
            void SetClassID(LPCTSTR lpszClassID);

            const HWND& GetHandle() const { return hwnd_; }

            //! set up and register the WNDCLASSEX structure
            bool Initialise(LPCTSTR lpszClassID = "WinClass",
                UINT style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW,
                HICON hIcon = LoadIcon(NULL, IDI_APPLICATION),
                HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW),
                HBRUSH hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
                LPCTSTR lpszMenuID = NULL);

            //! set up some parameters and create the window for display
            bool Display(LPCTSTR lpszTitle = "Win32 Window",
                DWORD dwStyles = WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                UINT xPos = 200, UINT yPos = 200,
                UINT width = 800, UINT height = 600);

        protected:
            CHAR title_[256];
            CHAR classID_[256];

            UINT width_, height_;
            bool windowVisible_;
            HINSTANCE hinst_;
            HWND hwnd_;
        };
    }
}

#endif // FRAMEWORK_RENDERING_WINDOW_H
