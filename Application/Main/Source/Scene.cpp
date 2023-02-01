/*!
    @file Scene.cpp @author Joel Barrett @date 01/01/12 @brief Bézier curve scene.
*/

#include "Scene.h"

namespace Application
{
    Scene::~Scene()
    {
        if (gridDisplayList_)
        {
            glDeleteLists(gridDisplayList_, 1);
            gridDisplayList_ = NULL;
        }
        if (sphereDisplayList_)
        {
            glDeleteLists(sphereDisplayList_, 1);
            sphereDisplayList_ = NULL;
        }
    }

    void Scene::Init()
    {
        settings_.Load("Assets/Settings.xml");

        InitWindow();
        InitEntities();
        InitOpenGL();
        InitDisplayLists();
    }

    void Scene::Execute()
    {
        MSG msg = {0};

        while (msg.message != WM_QUIT)
        {
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                Update(0.01f);
                Render();
            }
        }
    }

    void Scene::InitWindow()
    {
        Initialise("WinClass", CS_OWNDC | CS_VREDRAW | CS_HREDRAW, LoadIcon(hinst_, (LPCTSTR)IDI_BEZIER_ICON), 
            LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH), NULL);

        Display(settings_.window_.title.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, GetSystemMetrics(SM_CXSCREEN) / 4, 
            GetSystemMetrics(SM_CYSCREEN) / 4, settings_.window_.width, settings_.window_.height);

        // generate a replacement window with 8xMSAA if supported. There doesn't
        // seem to be a better way of doing this...
        int pixelFormat = 0;
        SetPixelFormatDescriptor(pixelFormat);
        if (InitMultisample(pixelFormat))
        {
            Release();
            DestroyWindow(hwnd_);
            Display(settings_.window_.title.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, GetSystemMetrics(SM_CXSCREEN) / 4, 
                GetSystemMetrics(SM_CYSCREEN) / 4, settings_.window_.width, settings_.window_.height);
            SetPixelFormatDescriptor(pixelFormat);
        }
        Framework::Rendering::SetProcessorAffinity();
    }

    void Scene::InitEntities()
    {
        if (!shipModel_.LoadModelData(settings_.modelFilename_.c_str())) {
            throw std::runtime_error("The file '" + settings_.modelFilename_ + "' couldn't be found");
        }
        camera_.SetView(settings_.camera_.position, settings_.camera_.lookAt);
        camera_.ComputeFRU();

        // generate the track based on curves described in Settings.xml
        std::vector< CurveSettings >::iterator it = settings_.track_.begin();
        track_.AddFirstCurve(it->ctrlPoints[0], it->ctrlPoints[1], it->ctrlPoints[2], it->ctrlPoints[3]);
        for (++it; it != settings_.track_.end(); ++it)
        {
            track_.AddCurveToEnd(it->ctrlPoints[0], it->ctrlPoints[1]);
        }
        track_.AddLastCurve();

        light_.ambient = settings_.light_.ambient;
        light_.diffuse = settings_.light_.diffuse;
        light_.position = settings_.light_.position;
    }

    void Scene::InitOpenGL()
    {
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glShadeModel(GL_SMOOTH);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHT1);

        glLightfv(GL_LIGHT1, GL_AMBIENT, &light_.ambient[0]);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, &light_.diffuse[0]);
        glLightfv(GL_LIGHT1, GL_POSITION, &light_.position[0]);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, width_ / static_cast<GLdouble>(height_), 0.1, 500.0);
    }

    void Scene::InitDisplayLists()
    {
        // setup control point spheres display list
        sphereDisplayList_ = glGenLists(1);
        glNewList(sphereDisplayList_, GL_COMPILE);
        GLUquadric* ctrlPointQuadric = gluNewQuadric();
        gluSphere(ctrlPointQuadric, track_.GetCtrlPointRadius(), 32, 16);
        gluDeleteQuadric(ctrlPointQuadric);
        glEndList();

        // setup grid display list
        gridDisplayList_ = glGenLists(1);
        glNewList(gridDisplayList_, GL_COMPILE);
        glBegin(GL_LINES);
        glPushMatrix();
        glColor3f(0.2f, 0.2f, 0.2f);

        for (int i = -20; i <= 20; ++i)
        {
            // x-axis
            glVertex3i(i, 0, -20);
            glVertex3i(i, 0,  20);

            // z-axis
            glVertex3i(-20, 0, i);
            glVertex3i( 20, 0, i);
        }
        glPopMatrix();
        glEnd();
        glEndList();
    }

    void Scene::Update(float dt)
    {
        static Vector3f newCameraPos;
        switch (camera_.GetMode())
        {
        case Camera::CAMERA_MODE_1ST:
            newCameraPos = track_.GetShip(0).pos + Vector3f(0.0f, 0.4f, 0.0f);
            camera_.SetView(newCameraPos, newCameraPos + Normalised(track_.GetShip(0).heading));
            break;

        case Camera::CAMERA_MODE_3RD:
            camera_.SetViewToTarget(track_.GetShip(0).pos - track_.GetShip(0).heading * 
                4.0f + Vector3f(0.0f, 2.0f, 0.0f), track_.GetShip(0).pos);
            break;
        }
        track_.Update(dt);
    }

    void Scene::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //glLoadMatrixf(camera_.GetViewMatrix());
        gluLookAt(camera_.GetPosition().x(), camera_.GetPosition().y(), camera_.GetPosition().z(),
            camera_.GetLook().x(), camera_.GetLook().y(), camera_.GetLook().z(),
            camera_.GetUp().x(), camera_.GetUp().y(), camera_.GetUp().z());
        glCallList(gridDisplayList_);

        RenderTrack();
        RenderCPLines();
        RenderCPSpheres();
        RenderShips();

        SwapBuffers(hDC_);
    }

    void Scene::RenderTrack()
    {
        glBegin(GL_LINE_STRIP);
        glPushMatrix();
        glColor3f(0.8f, 0.8f, 0.8f);
        for (std::size_t i = 0; i < track_.GetNumCurves(); ++i)
        {
            for (std::size_t j = 0; j < track_.GetCurve(i).GetResolution(); ++j) {
                glVertex3fv(&track_.GetCurve(i).GetPolylineVert(j).x());
            }
        }
        glVertex3fv(&track_.GetCurve(0).GetPolylineVert(0).x());
        glPopMatrix();
        glEnd();
    }

    void Scene::RenderCPLines()
    {
        glBegin(GL_LINES);
        glPushMatrix();
        glColor3fv(Colour::red);
        for (std::size_t i = 0; i < track_.GetNumCurves(); ++i)
        {
            glVertex3fv(&track_.GetCurve(i).GetCtrlPoint(0).x());
            glVertex3fv(&track_.GetCurve(i).GetCtrlPoint(1).x());
            glVertex3fv(&track_.GetCurve(i).GetCtrlPoint(2).x());
            glVertex3fv(&track_.GetCurve(i).GetCtrlPoint(3).x());
        }
        glPopMatrix();
        glEnd();
    }

    void Scene::RenderCPSpheres()
    {
        glMaterialfv(GL_FRONT, GL_EMISSION, Colour::black);
        glColor3fv(Colour::red);

        glEnable(GL_LIGHTING);
        for (std::size_t i = 0; i < track_.GetNumCurves(); ++i)
        {
            for (std::size_t j = 0; j < (track_.GetCurve(i).GetDegree() - 1); ++j)
            {
                glPushMatrix();
                glTranslatef(track_.GetCurve(i).GetCtrlPoint(j).x(), track_.GetCurve(i).GetCtrlPoint(j).y(), 
                    track_.GetCurve(i).GetCtrlPoint(j).z());
                glCallList(sphereDisplayList_);
                glPopMatrix();
            }
        }
        glDisable(GL_LIGHTING);
    }

    void Scene::RenderShips()
    {
        Vector3f axis;
        float angle = 0.0f, anglep = 0.0f, angley = 0.0f;

        glEnable(GL_LIGHTING);
        glColor3f(0.6f, 0.6f, 0.6f);

        for (std::size_t i = 0; i < track_.GetNumShips(); ++i)
        {
            // angle between direction of ship and x-axis in degrees
            angle = Angle(track_.GetShip(i).heading, Vector3f::UNIT_X) * Const<float>::TO_DEG;

            // axis of rotation is cross product of ship's heading and negative unit x
            axis = Normalised(Cross(track_.GetShip(i).heading, -Vector3f::UNIT_X));

            glPushMatrix();
            glTranslatef(track_.GetShip(i).pos.x(), track_.GetShip(i).pos.y(), track_.GetShip(i).pos.z());
            glRotatef(angle, axis.x(), axis.y(), axis.z());

            // don't render the first ship when player is looking out its cockpit
            if (!(camera_.GetMode() == Camera::CAMERA_MODE_1ST && i == 0)) {
                shipModel_.Draw();
            }
            glPopMatrix();
        }
        glDisable(GL_LIGHTING);
    }

    LRESULT CALLBACK Scene::MsgRouter(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        static POINT mousePos, prevMousePos;
        static Vector3f cameraPos, cameraLook;
        static bool mouseRight;

        switch (msg)
        {
        case WM_MOUSEMOVE:

            if (camera_.ModeEquals(Camera::CAMERA_MODE_GOD))
            {
                mousePos.x = LOWORD(lParam);
                mousePos.y = HIWORD(lParam);

                if (track_.IsCtrlPointSelected()) {
                    track_.DragCtrlPoint(mousePos.x, mousePos.y);
                }
                if (mouseRight) {
                    camera_.SetViewByMouse(static_cast<float>(prevMousePos.x) - mousePos.x, 
                        static_cast<float>(prevMousePos.y) - mousePos.y);
                    camera_.ComputeFRU();
                }
                prevMousePos = mousePos;
            }
            break;

        case WM_LBUTTONDOWN:

            // test for control point selection if in god mode and not moving camera
            if (camera_.ModeEquals(Camera::CAMERA_MODE_GOD) && !mouseRight) {
                track_.SelectCtrlPoint(camera_, width_, height_, mousePos.x, mousePos.y);
            }
            break;

        case WM_LBUTTONUP:
            track_.SetCtrlPointSelected(false);
            break;

        case WM_RBUTTONDOWN:
            track_.SetCtrlPointSelected(false);
            mouseRight = true;
            break;

        case WM_RBUTTONUP:
            mouseRight = false;
            break;

        case WM_MOUSELEAVE:
            track_.SetCtrlPointSelected(false);
            break;

        case WM_KEYDOWN:

            switch (wParam)
            {
            case VK_ESCAPE:
                PostQuitMessage(0);
                break;

            case VK_SPACE:

                switch (camera_.GetMode())
                {
                case Camera::CAMERA_MODE_GOD:

                    camera_.SetMode(Camera::CAMERA_MODE_1ST);

                    if (!track_.GetNumShips()) {
                        track_.AddShip();
                    }
                    cameraPos = camera_.GetPosition();
                    cameraLook = camera_.GetLook();

                    track_.SetCtrlPointSelected(false);
                    SetCursor(NULL);
                    break;

                case Camera::CAMERA_MODE_1ST:

                    camera_.SetMode(Camera::CAMERA_MODE_3RD);

                    SetCursor(NULL);
                    break;

                case Camera::CAMERA_MODE_3RD:

                    camera_.SetMode(Camera::CAMERA_MODE_GOD);
                    camera_.SetView(cameraPos, cameraLook);

                    SetCursor(LoadCursor(NULL, IDC_ARROW));
                    break;
                }
                break;

            case VK_UP:
                if (camera_.ModeEquals(Camera::CAMERA_MODE_GOD)) {
                    track_.IncResolution();
                }
                break;

            case VK_DOWN:
                if (camera_.ModeEquals(Camera::CAMERA_MODE_GOD)) {
                    track_.DecResolution();
                }
                break;

            case 0x43: // 'C'
                if (camera_.ModeEquals(Camera::CAMERA_MODE_GOD)) {
                    track_.AddCurve();
                }
                break;

            case 0x45: // 'E'
                if (camera_.ModeEquals(Camera::CAMERA_MODE_GOD)) {
                    track_.AddShip();
                }
                break;

            case 0x51: // 'Q'
                PostQuitMessage(0);
                break;

            case 0x52: // 'R'
                if (camera_.ModeEquals(Camera::CAMERA_MODE_GOD)) {
                    track_.RemoveShip();
                }
                break;
            }
            break;

        case WM_SIZE:

            width_ = LOWORD(lParam);
            height_ = HIWORD(lParam);

            switch (wParam)
            {
            case SIZE_MINIMIZED:
                windowVisible_ = false;
                break;

            case SIZE_MAXIMIZED:
                windowVisible_ = true;
                Resize(width_, height_);
                break;

            case SIZE_RESTORED:
                windowVisible_ = true;
                Resize(width_, height_);
                break;
            }
            break;

        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_DESTROY:
            break;

        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
