#include "Window.h"


void (*_DrawFunction)(void);

int _O_WindowWidth;
int _O_WindowHeight;
int frameCount;
float fpsTimer;

RECT _desktop;
WNDCLASSEX _wc;


// my linker genuinely has no idea what a variable is, so i have to do this the complicated way.
// if you know how to fix this, please make an issue or update request.

void Setup(void (*df)(void), int ww, int wh)
{
    _DrawFunction = df;
    _O_WindowWidth = ww;
    _O_WindowHeight = wh;
}


void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    if (pd3dDevice->Reset(&d3dpp) == D3DERR_INVALIDCALL)
        IM_ASSERT(0);

    ImGui_ImplDX9_CreateDeviceObjects();
}


void UpdateFPS() {
    static float fps = 0.0f;
    const float deltaTime = ImGui::GetIO().DeltaTime;

    frameCount++;
    fpsTimer += deltaTime;

    if (fpsTimer >= 1.0f) {
        fps = static_cast<float>(frameCount) / fpsTimer;
        frameCount = 0;
        fpsTimer = 0.0f;
    }

    ImGui::Text("FPS: %.1f", fps);
}


LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_SIZE:
            if (pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
            {
                d3dpp.BackBufferWidth = LOWORD(lParam);
                d3dpp.BackBufferHeight = HIWORD(lParam);

                ResetDevice();
            }

            return 0;

        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU)
                return 0;

            break;

        /*  stops the window from rendering when dragged. (currently disabled)
        case WM_NCHITTEST:
        {
            ImVec2 Mouse = ImGui::GetMousePos();

            if (Mouse.y < 25 && Mouse.x < _O_WindowWidth - 25)
            {
                LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);

                if (hit == HTCLIENT)
                    hit = HTCAPTION;

                return hit;
            }

            else
                break;
        }
        */

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}


void StartOverlay(LPCWSTR AppClass, LPCWSTR AppName, const char* Title, bool fps)
{
    GetWindowRect(GetDesktopWindow(), &_desktop);
    _wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, AppClass, NULL };
 
    RegisterClassEx(&_wc);

    hwnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED, 
                          AppClass, AppName, WS_POPUP,
                          (_desktop.right / 2) - (_O_WindowWidth / 2),
                          (_desktop.bottom / 2) - (_O_WindowHeight / 2),
                          _O_WindowWidth, _O_WindowHeight, 0, 0, _wc.hInstance, 0);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, ULW_COLORKEY);

    if (CreateDeviceD3D(hwnd) < 0)
    {
        CleanupDeviceD3D();
        UnregisterClass(_wc.lpszClassName, _wc.hInstance);
        return;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(pd3dDevice);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    static bool open = true;
    DWORD dwFlag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        if (!open)
            ExitProcess(EXIT_SUCCESS);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();

        ImGui::NewFrame();
        ImGui::Begin(Title);

        if (fps)
            UpdateFPS();
            
        _DrawFunction();


        ImGui::End();
        ImGui::EndFrame();

        pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        
        if (pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            pd3dDevice->EndScene();
        }

        HRESULT result = pd3dDevice->Present(NULL, NULL, NULL, NULL);
        
        if (result == D3DERR_DEVICELOST && pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(hwnd);

    UnregisterClass(_wc.lpszClassName, _wc.hInstance);

    return;
}