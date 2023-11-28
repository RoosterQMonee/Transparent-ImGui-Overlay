#pragma once


#include <Windows.h>

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")



extern HWND                     hwnd;
extern LPDIRECT3D9              pD3D;
extern LPDIRECT3DDEVICE9        pd3dDevice;
extern D3DPRESENT_PARAMETERS    d3dpp;


HRESULT CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();