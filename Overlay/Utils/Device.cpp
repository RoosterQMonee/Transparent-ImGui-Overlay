#include "Device.h"


HWND hwnd = NULL;
LPDIRECT3D9 pD3D = NULL;
LPDIRECT3DDEVICE9 pd3dDevice = NULL;
D3DPRESENT_PARAMETERS d3dpp = {};



HRESULT CreateDeviceD3D(HWND hWnd)
{
    if ((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return E_FAIL;

    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    d3dpp.Windowed = TRUE;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // change if you want to use vsync!

    if (pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice) < 0)
        return E_FAIL;

    return S_OK;
}


void CleanupDeviceD3D()
{
    if (pd3dDevice) {
        pd3dDevice->Release();
        pd3dDevice = NULL;
    }

    if (pD3D)
    {
        pD3D->Release();
        pD3D = NULL;
    }
}