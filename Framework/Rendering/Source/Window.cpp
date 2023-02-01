/*!
    @file Window.cpp @author Joel Barrett @date 12/04/09 @brief Windows 32-bit window class.
*/

#include <stdexcept>
#include "Window.h"

namespace Framework
{
    namespace Rendering
    {
        // taken from http://www.dhpoware.com
        void SetProcessorAffinity()
        {
            // Assign the current thread to one processor. This ensures that timing
            // code runs on only one processor, and will not suffer any ill effects
            // from power management.
            //
            // Based on the DXUTSetProcessorAffinity() function in the DXUT framework.
            DWORD_PTR dwProcessAffinityMask = 0;
            DWORD_PTR dwSystemAffinityMask = 0;
            HANDLE hCurrentProcess = GetCurrentProcess();

            if (!GetProcessAffinityMask(hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask)) {
                return;
            }
            if (dwProcessAffinityMask)
            {
                // Find the lowest processor that our process is allowed to run against.
                DWORD_PTR dwAffinityMask = (dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1));

                // Set this as the processor that our thread must always run against.
                // This must be a subset of the process affinity mask.
                HANDLE hCurrentThread = GetCurrentThread();
                if (hCurrentThread != INVALID_HANDLE_VALUE)
                {
                    SetThreadAffinityMask(hCurrentThread, dwAffinityMask);
                    CloseHandle(hCurrentThread);
                }
            }
            CloseHandle(hCurrentProcess);
        }

        bool Win32Window::Initialise(LPCTSTR lpszClassID, UINT style, HICON hIcon, 
            HCURSOR hCursor, HBRUSH hbrBackground, LPCTSTR lpszMenuID)
        {
            WNDCLASSEX wcx = {0};

            wcx.lpszClassName = lpszClassID;
            wcx.cbSize = sizeof(WNDCLASSEX);
            wcx.style = style;
            wcx.lpfnWndProc = st_MsgRouter;
            wcx.hInstance = hinst_;
            wcx.hIcon = hIcon;
            wcx.hIconSm = hIcon;
            wcx.hCursor = hCursor;
            wcx.hbrBackground = hbrBackground;
            wcx.lpszMenuName = lpszMenuID;
            wcx.cbClsExtra = 0;
            wcx.cbWndExtra = 0;

            if (!RegisterClassEx(&wcx)) {
                throw std::runtime_error("The function RegisterClassEx() failed");
                return false;
            }
            strcpy(classID_, lpszClassID);
            return true;
        }

        bool Win32Window::Display(LPCTSTR lpszTitle, DWORD dwStyles, 
            UINT xPos, UINT yPos, UINT width, UINT height)
        {
            hwnd_ = CreateWindowEx(NULL, classID_, lpszTitle, dwStyles, xPos, yPos,
                width, height, NULL, NULL, hinst_, (void*)this);

            if (!hwnd_) {
                throw std::runtime_error("The function CreateWindowEx() failed");
                return false;
            }
            return true;
        }

        void Win32Window::SetTitle(LPCTSTR lpszTitle)
        {
            ZeroMemory(title_, sizeof(title_));
            strcpy(title_, lpszTitle);
        }

        void Win32Window::SetClassID(LPCTSTR lpszClassID)
        {
            ZeroMemory(classID_, sizeof(classID_));
            strcpy(classID_, lpszClassID);
        }
         
        LRESULT CALLBACK Win32Window::st_MsgRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            if (uMsg == WM_NCCREATE)
            {
                // get pointer to the window from lpCreateParams
                SetWindowLongPtr(hWnd, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
            }
            // get pointer to the window
            Win32Window* pWnd = (Win32Window*)GetWindowLongPtr(hWnd, GWL_USERDATA);

            // if a window exists, use message router of window; otherwise, use DefWindowProc
            return pWnd ? pWnd->MsgRouter(hWnd, uMsg, wParam, lParam)
                : DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }
}
