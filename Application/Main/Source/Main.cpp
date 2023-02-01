/*!
    @file Main.cpp @author Joel Barrett @date 01/01/12 @brief Main entry point to application.
*/

#include "Scene.h"

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prevInst, LPSTR cmdLine, int cmdShow)
{
    Application::Scene bezierCurves;

    try {
        bezierCurves.Init();
        bezierCurves.Execute();
    }
    catch (std::exception &e) {
        MessageBox(NULL, e.what(), "An exception occurred!", 
            MB_OK | MB_ICONERROR | MB_TASKMODAL);
    }
    return 0;
}
