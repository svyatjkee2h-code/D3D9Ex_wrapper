// d3d9ex_wrapper.cpp
#include "d3d9ex_wrapper.h"
#include <windows.h>
#include <cassert>
#include <string>

// D3D9Ex
#ifndef D3DCREATE_ENABLE_PRESENTSTATS
#define D3DCREATE_ENABLE_PRESENTSTATS          0x00004000L
#endif
#ifndef D3DCREATE_DISABLE_PRINTSCREEN
#define D3DCREATE_DISABLE_PRINTSCREEN          0x00008000L
#endif
#ifndef D3DCREATE_SCREENSAVER
#define D3DCREATE_SCREENSAVER                  0x10000000L
#endif
#ifndef D3DCREATE_DISABLE_DRIVER_MANAGEMENT_EX
#define D3DCREATE_DISABLE_DRIVER_MANAGEMENT_EX 0x00010000L
#endif

// D3D9Ex flags mask
#define D3D9EX_ONLY_BEHAVIOR_FLAGS \
    (D3DCREATE_ENABLE_PRESENTSTATS | D3DCREATE_DISABLE_PRINTSCREEN | \
     D3DCREATE_SCREENSAVER | D3DCREATE_DISABLE_DRIVER_MANAGEMENT_EX)

// D3D9Ex usage flags
#ifndef D3DUSAGE_NONSECURE
#define D3DUSAGE_NONSECURE                     0x00000080L
#endif
#ifndef D3DUSAGE_TEXTAPI
#define D3DUSAGE_TEXTAPI                       0x00000040L
#endif
#ifndef D3DUSAGE_RESTRICTED_CONTENT
#define D3DUSAGE_RESTRICTED_CONTENT            0x00000800L
#endif
#ifndef D3DUSAGE_RESTRICT_SHARED_RESOURCE
#define D3DUSAGE_RESTRICT_SHARED_RESOURCE      0x00001000L
#endif
#ifndef D3DUSAGE_RESTRICT_SHARED_RESOURCE_DRIVER
#define D3DUSAGE_RESTRICT_SHARED_RESOURCE_DRIVER 0x00002000L
#endif

#define D3D9EX_ONLY_USAGE_FLAGS \
    (D3DUSAGE_NONSECURE | D3DUSAGE_TEXTAPI | D3DUSAGE_RESTRICTED_CONTENT | \
     D3DUSAGE_RESTRICT_SHARED_RESOURCE | D3DUSAGE_RESTRICT_SHARED_RESOURCE_DRIVER)

// D3D9Ex caps
#ifndef D3DPMISCCAPS_POSTBLENDSRGBCONVERT
#define D3DPMISCCAPS_POSTBLENDSRGBCONVERT      0x00200000L
#endif
#ifndef D3DPTFILTERCAPS_CONVOLUTIONMONO
#define D3DPTFILTERCAPS_CONVOLUTIONMONO        0x00040000L
#endif

// D3D FLAGS
#ifndef D3DERR_DEVICEHUNG
#define D3DERR_DEVICEHUNG                      MAKE_D3DHRESULT(2160)
#endif
#ifndef D3DERR_DEVICEREMOVED
#define D3DERR_DEVICEREMOVED                   MAKE_D3DHRESULT(2156)
#endif
#ifndef D3DERR_UNSUPPORTEDOVERLAY
#define D3DERR_UNSUPPORTEDOVERLAY              MAKE_D3DHRESULT(2171)
#endif
#ifndef D3DERR_UNSUPPORTEDOVERLAYFORMAT
#define D3DERR_UNSUPPORTEDOVERLAYFORMAT        MAKE_D3DHRESULT(2172)
#endif
#ifndef D3DERR_CANNOTPROTECTCONTENT
#define D3DERR_CANNOTPROTECTCONTENT            MAKE_D3DHRESULT(2173)
#endif
#ifndef D3DERR_UNSUPPORTEDCRYPTO
#define D3DERR_UNSUPPORTEDCRYPTO               MAKE_D3DHRESULT(2174)
#endif
#ifndef D3DERR_PRESENT_STATISTICS_DISJOINT
#define D3DERR_PRESENT_STATISTICS_DISJOINT     MAKE_D3DHRESULT(2175)
#endif

#ifndef S_NOT_RESIDENT
#define S_NOT_RESIDENT                         MAKE_D3DSTATUS(2161)
#endif
#ifndef S_RESIDENT_IN_SHARED_MEMORY
#define S_RESIDENT_IN_SHARED_MEMORY            MAKE_D3DSTATUS(2162)
#endif
#ifndef S_PRESENT_MODE_CHANGED
#define S_PRESENT_MODE_CHANGED                 MAKE_D3DSTATUS(2163)
#endif
#ifndef S_PRESENT_OCCLUDED
#define S_PRESENT_OCCLUDED                     MAKE_D3DSTATUS(2164)
#endif

// D3DPRESENT EX FLAGS
#ifndef D3DPRESENT_DONOTFLIP
#define D3DPRESENT_DONOTFLIP                   0x00000004L
#endif
#ifndef D3DPRESENT_VIDEO_RESTRICT_TO_MONITOR
#define D3DPRESENT_VIDEO_RESTRICT_TO_MONITOR   0x00000010L
#endif
#ifndef D3DPRESENT_UPDATEOVERLAYONLY
#define D3DPRESENT_UPDATEOVERLAYONLY           0x00000020L
#endif
#ifndef D3DPRESENT_HIDEOVERLAY
#define D3DPRESENT_HIDEOVERLAY                 0x00000040L
#endif
#ifndef D3DPRESENT_UPDATECOLORKEY
#define D3DPRESENT_UPDATECOLORKEY              0x00000080L
#endif
#ifndef D3DPRESENT_BACK_BUFFERS_MAX_EX
#define D3DPRESENT_BACK_BUFFERS_MAX_EX         30
#endif

#ifndef D3DPRESENTFLAG_LOCKABLE_BACKBUFFER
#define D3DPRESENTFLAG_LOCKABLE_BACKBUFFER     0x00000001L
#endif
#ifndef D3DPRESENTFLAG_NOAUTOROTATE
#define D3DPRESENTFLAG_NOAUTOROTATE            0x00000002L
#endif
#ifndef D3DPRESENTFLAG_UNPRUNEDMODE
#define D3DPRESENTFLAG_UNPRUNEDMODE            0x00000020L
#endif
#ifndef D3DPRESENTFLAG_OVERLAY_LIMITEDRGB
#define D3DPRESENTFLAG_OVERLAY_LIMITEDRGB      0x00000040L
#endif
#ifndef D3DPRESENTFLAG_OVERLAY_YCbCr_BT709
#define D3DPRESENTFLAG_OVERLAY_YCbCr_BT709     0x00000080L
#endif
#ifndef D3DPRESENTFLAG_OVERLAY_YCbCr_xvYCC
#define D3DPRESENTFLAG_OVERLAY_YCbCr_xvYCC     0x00000100L
#endif
#ifndef D3DPRESENTFLAG_RESTRICTED_CONTENT
#define D3DPRESENTFLAG_RESTRICTED_CONTENT      0x00000200L
#endif
#ifndef D3DPRESENTFLAG_RESTRICT_SHARED_RESOURCE_DRIVER
#define D3DPRESENTFLAG_RESTRICT_SHARED_RESOURCE_DRIVER 0x00000400L
#endif

typedef IDirect3D9* (WINAPI* Direct3DCreate9_t)(UINT SDKVersion);
static Direct3DCreate9_t Direct3DCreate9_original = nullptr;
static HMODULE hOriginalD3D9 = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        char systemDir[MAX_PATH];
        if (GetSystemDirectoryA(systemDir, MAX_PATH) == 0) {
            return FALSE;
        }
        std::string d3d9Path = std::string(systemDir) + "\\d3d9.dll";
        hOriginalD3D9 = LoadLibraryA(d3d9Path.c_str());
        if (!hOriginalD3D9) {
            return FALSE;
        }
        Direct3DCreate9_original = reinterpret_cast<Direct3DCreate9_t>(GetProcAddress(hOriginalD3D9, "Direct3DCreate9"));
        if (!Direct3DCreate9_original) {
            FreeLibrary(hOriginalD3D9);
            hOriginalD3D9 = nullptr;
            return FALSE;
        }
        break;
    }
    case DLL_PROCESS_DETACH:
        if (hOriginalD3D9) {
            FreeLibrary(hOriginalD3D9);
            hOriginalD3D9 = nullptr;
        }
        Direct3DCreate9_original = nullptr;
        break;
    }
    return TRUE;
}

extern "C" IDirect3D9* WINAPI Direct3DCreate9(UINT SDKVersion)
{
    return Direct3DCreate9_original(SDKVersion);
}

extern "C" HRESULT WINAPI Direct3DCreate9Ex(UINT SDKVersion, IDirect3D9Ex** ppD3D) {
    if (!ppD3D) return D3DERR_INVALIDCALL;
    *ppD3D = nullptr;

    if (!Direct3DCreate9_original) return D3DERR_NOTAVAILABLE;

    IDirect3D9* pD3D9 = Direct3DCreate9_original(SDKVersion);
    if (!pD3D9) return D3DERR_NOTAVAILABLE;

    MyDirect3D9Ex* pWrapper = new MyDirect3D9Ex(pD3D9, true);
    if (!pWrapper) {
        pD3D9->Release();
        return E_OUTOFMEMORY;
    }

    *ppD3D = pWrapper;
    return D3D_OK;
}

// MyDirect3D9Ex
MyDirect3D9Ex::MyDirect3D9Ex(IDirect3D9* pD3D9, bool isEx)
    : m_pD3D9(pD3D9), m_refCount(1), m_isExD3D9(isEx)
{
    assert(pD3D9 != nullptr);
}

MyDirect3D9Ex::~MyDirect3D9Ex() {
    if (m_pD3D9) m_pD3D9->Release();
}

HRESULT MyDirect3D9Ex::QueryInterface(REFIID riid, void** ppvObj) {
    if (!ppvObj) return E_POINTER;
    
    if (riid == IID_IUnknown || riid == IID_IDirect3D9 || riid == IID_IDirect3D9Ex) {
        *ppvObj = static_cast<IDirect3D9Ex*>(this);
        AddRef();
        return S_OK;
    }
    *ppvObj = nullptr;
    if (m_pD3D9) return m_pD3D9->QueryInterface(riid, ppvObj);
    return E_NOINTERFACE;
}

ULONG MyDirect3D9Ex::AddRef() { return (ULONG)InterlockedIncrement(reinterpret_cast<LONG*>(&m_refCount)); }
ULONG MyDirect3D9Ex::Release() {
    ULONG count = (ULONG)InterlockedDecrement(reinterpret_cast<LONG*>(&m_refCount));
    if (count == 0) delete this;
    return count;
}

HRESULT MyDirect3D9Ex::RegisterSoftwareDevice(void* pInitializeFunction) { return m_pD3D9->RegisterSoftwareDevice(pInitializeFunction); }
UINT MyDirect3D9Ex::GetAdapterCount() { return m_pD3D9->GetAdapterCount(); }
HRESULT MyDirect3D9Ex::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier) { return m_pD3D9->GetAdapterIdentifier(Adapter, Flags, pIdentifier); }
UINT MyDirect3D9Ex::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format) { return m_pD3D9->GetAdapterModeCount(Adapter, Format); }
HRESULT MyDirect3D9Ex::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode) { return m_pD3D9->EnumAdapterModes(Adapter, Format, Mode, pMode); }
HRESULT MyDirect3D9Ex::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode) { return m_pD3D9->GetAdapterDisplayMode(Adapter, pMode); }
HRESULT MyDirect3D9Ex::CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed) { return m_pD3D9->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed); }

HRESULT MyDirect3D9Ex::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) { 
    DWORD safeUsage = Usage & ~D3D9EX_ONLY_USAGE_FLAGS;
    return m_pD3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, safeUsage, RType, CheckFormat); 
}
HRESULT MyDirect3D9Ex::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels) { return m_pD3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels); }
HRESULT MyDirect3D9Ex::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) { return m_pD3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat); }
HRESULT MyDirect3D9Ex::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat) { return m_pD3D9->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat); }
HRESULT MyDirect3D9Ex::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps) {
    HRESULT hr = m_pD3D9->GetDeviceCaps(Adapter, DeviceType, pCaps);
    if (SUCCEEDED(hr) && pCaps) {
        pCaps->Caps2 |= D3DCAPS2_CANSHARERESOURCE | D3DCAPS2_CANCALIBRATEGAMMA;
        pCaps->Caps3 |= D3DCAPS3_DXVAHD | D3DCAPS3_LINEAR_TO_SRGB_PRESENTATION | D3DCAPS3_COPY_TO_VIDMEM;
        pCaps->PresentationIntervals |= D3DPRESENT_INTERVAL_IMMEDIATE | D3DPRESENT_INTERVAL_ONE | D3DPRESENT_INTERVAL_TWO;
        pCaps->TextureFilterCaps |= D3DPTFILTERCAPS_CONVOLUTIONMONO;
    }
    return hr;
}
HMONITOR MyDirect3D9Ex::GetAdapterMonitor(UINT Adapter) { return m_pD3D9->GetAdapterMonitor(Adapter); }

HRESULT MyDirect3D9Ex::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface) { return m_pD3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface); }

UINT MyDirect3D9Ex::GetAdapterModeCountEx(UINT Adapter, const D3DDISPLAYMODEFILTER* pFilter) {
    if (!pFilter) return 0;
    return m_pD3D9->GetAdapterModeCount(Adapter, pFilter->Format);
}

HRESULT MyDirect3D9Ex::EnumAdapterModesEx(UINT Adapter, const D3DDISPLAYMODEFILTER* pFilter, UINT Mode, D3DDISPLAYMODEEX* pMode) {
    if (!pFilter || !pMode) return D3DERR_INVALIDCALL;
    D3DDISPLAYMODE dm;
    HRESULT hr = m_pD3D9->EnumAdapterModes(Adapter, pFilter->Format, Mode, &dm);
    if (SUCCEEDED(hr)) {
        pMode->Size = sizeof(D3DDISPLAYMODEEX);
        pMode->Width = dm.Width;
        pMode->Height = dm.Height;
        pMode->RefreshRate = dm.RefreshRate;
        pMode->Format = dm.Format;
        pMode->ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE;
    }
    return hr;
}

HRESULT MyDirect3D9Ex::GetAdapterDisplayModeEx(UINT Adapter, D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation)
{
    if (!pMode || pMode->Size != sizeof(D3DDISPLAYMODEEX))
        return D3DERR_INVALIDCALL;

    if (!m_pD3D9)
        return D3DERR_INVALIDCALL;

    if (Adapter >= GetAdapterCount())
        return D3DERR_INVALIDCALL;

    D3DDISPLAYMODE dm;
    ZeroMemory(&dm, sizeof(dm));

    HRESULT hr = m_pD3D9->GetAdapterDisplayMode(Adapter, &dm);
    if (FAILED(hr))
        return hr;

    pMode->Width = dm.Width;
    pMode->Height = dm.Height;
    pMode->RefreshRate = dm.RefreshRate;
    pMode->Format = dm.Format;
    pMode->ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE;

    if (pRotation)
        *pRotation = D3DDISPLAYROTATION_IDENTITY;

    return D3D_OK;
}


HRESULT MyDirect3D9Ex::CreateDeviceEx(
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pPresentationParameters,
    D3DDISPLAYMODEEX* pFullscreenDisplayMode,
    IDirect3DDevice9Ex** ppReturnedDeviceInterface)
{
    if (!ppReturnedDeviceInterface || !pPresentationParameters)
        return D3DERR_INVALIDCALL;

    *ppReturnedDeviceInterface = nullptr;

    const DWORD originalBehaviorFlags = BehaviorFlags;
    D3DPRESENT_PARAMETERS basePP = *pPresentationParameters;

    if (basePP.BackBufferCount == 0)
        basePP.BackBufferCount = 1;

    if (basePP.BackBufferCount > 3)
        basePP.BackBufferCount = 3;

    if (basePP.SwapEffect >= 5 /*D3DSWAPEFFECT_FLIPEX*/)
        basePP.SwapEffect = D3DSWAPEFFECT_DISCARD;

    if (!basePP.Windowed && pFullscreenDisplayMode)
    {
        basePP.BackBufferWidth  = pFullscreenDisplayMode->Width;
        basePP.BackBufferHeight = pFullscreenDisplayMode->Height;
        basePP.BackBufferFormat = pFullscreenDisplayMode->Format;

        if (basePP.FullScreen_RefreshRateInHz == 0)
        {
            basePP.FullScreen_RefreshRateInHz =
                pFullscreenDisplayMode->RefreshRate ?
                pFullscreenDisplayMode->RefreshRate : 60;
        }
    }

    DWORD safeFlags = BehaviorFlags & ~D3D9EX_ONLY_BEHAVIOR_FLAGS;

    IDirect3DDevice9* pDevice = nullptr;
    HRESULT hr = D3DERR_INVALIDCALL;

    for (int attempt = 0; attempt < 2 && FAILED(hr); ++attempt)
    {
        DWORD attemptFlags = safeFlags;

        if (attempt == 0)
        {
            attemptFlags &= ~(D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING);
            attemptFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
        }
        else
        {
            attemptFlags &= ~(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MIXED_VERTEXPROCESSING);
            attemptFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
            
            if (basePP.BackBufferCount > 1)
                basePP.BackBufferCount = 1;
        }

        hr = m_pD3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, attemptFlags, &basePP, &pDevice);
    }

    if (SUCCEEDED(hr) && pDevice)
    {

        *pPresentationParameters = basePP;

        auto* wrapper = new (std::nothrow) MyDirect3DDevice9Ex(
            pDevice, this, originalBehaviorFlags, m_isExD3D9);

        if (!wrapper)
        {
            pDevice->Release();
            return E_OUTOFMEMORY;
        }

        *ppReturnedDeviceInterface = wrapper;
        return D3D_OK;
    }

    return hr;
}

HRESULT MyDirect3D9Ex::GetAdapterLUID(UINT Adapter, LUID* pLUID) {
    if (!pLUID) return D3DERR_INVALIDCALL;
    pLUID->LowPart = Adapter;
    pLUID->HighPart = 0;
    return S_OK;
}

// MyDirect3DDevice9Ex
MyDirect3DDevice9Ex::MyDirect3DDevice9Ex(IDirect3DDevice9* pDevice, MyDirect3D9Ex* pD3D9Ex,
                                         DWORD originalBehaviorFlags, bool isEx)
    : m_pDevice(pDevice), m_pD3D9Ex(pD3D9Ex), m_refCount(1),
      m_gpuThreadPriority(0), m_maxFrameLatency(3),
      m_OriginalBehaviorFlags(originalBehaviorFlags),
      m_isExD3D9(isEx),
      m_pImplicitSwapChain(nullptr)
{
    assert(pDevice != nullptr);
    if (m_pD3D9Ex) m_pD3D9Ex->AddRef();
}

MyDirect3DDevice9Ex::~MyDirect3DDevice9Ex() {
    if (m_pImplicitSwapChain) {
        m_pImplicitSwapChain->Release();
        m_pImplicitSwapChain = nullptr;
    }
    
    if (m_pD3D9Ex) m_pD3D9Ex->Release();
    if (m_pDevice) m_pDevice->Release();
}

HRESULT MyDirect3DDevice9Ex::QueryInterface(REFIID riid, void** ppvObj) {
    if (!ppvObj) return E_POINTER;
    
    if (riid == IID_IUnknown || riid == IID_IDirect3DDevice9 || riid == IID_IDirect3DDevice9Ex) {
        *ppvObj = static_cast<IDirect3DDevice9Ex*>(this);
        AddRef();
        return S_OK;
    }
    *ppvObj = nullptr;
    if (m_pDevice) return m_pDevice->QueryInterface(riid, ppvObj);
    return E_NOINTERFACE;
}

ULONG MyDirect3DDevice9Ex::AddRef() { return (ULONG)InterlockedIncrement(reinterpret_cast<LONG*>(&m_refCount)); }
ULONG MyDirect3DDevice9Ex::Release() {
    ULONG count = (ULONG)InterlockedDecrement(reinterpret_cast<LONG*>(&m_refCount));
    if (count == 0) delete this;
    return count;
}

HRESULT MyDirect3DDevice9Ex::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS* pParameters)
{
    if (!pParameters)
        return D3DERR_INVALIDCALL;

    HRESULT hr = m_pDevice->GetCreationParameters(pParameters);
    if (SUCCEEDED(hr))
    {
        pParameters->BehaviorFlags = m_OriginalBehaviorFlags;
    }
    return hr;
}

HRESULT MyDirect3DDevice9Ex::TestCooperativeLevel() { return m_pDevice->TestCooperativeLevel(); }
UINT MyDirect3DDevice9Ex::GetAvailableTextureMem() { return m_pDevice->GetAvailableTextureMem(); }
HRESULT MyDirect3DDevice9Ex::EvictManagedResources() { return m_pDevice->EvictManagedResources(); }
HRESULT MyDirect3DDevice9Ex::GetDirect3D(IDirect3D9** ppD3D9) {
    if (!ppD3D9) return D3DERR_INVALIDCALL;
    *ppD3D9 = m_pD3D9Ex;
    if (m_pD3D9Ex) m_pD3D9Ex->AddRef();
    return D3D_OK;
}
HRESULT MyDirect3DDevice9Ex::GetDeviceCaps(D3DCAPS9* pCaps) {
    HRESULT hr = m_pDevice->GetDeviceCaps(pCaps);
    if (SUCCEEDED(hr) && pCaps) {
        pCaps->Caps2 |= D3DCAPS2_CANSHARERESOURCE | D3DCAPS2_CANCALIBRATEGAMMA;
        pCaps->Caps3 |= D3DCAPS3_DXVAHD | D3DCAPS3_LINEAR_TO_SRGB_PRESENTATION | D3DCAPS3_COPY_TO_VIDMEM;
        pCaps->PresentationIntervals |= D3DPRESENT_INTERVAL_IMMEDIATE | D3DPRESENT_INTERVAL_ONE | D3DPRESENT_INTERVAL_TWO;
        pCaps->TextureFilterCaps |= D3DPTFILTERCAPS_CONVOLUTIONMONO;
    }
    return hr;
}
HRESULT MyDirect3DDevice9Ex::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode) { return m_pDevice->GetDisplayMode(iSwapChain, pMode); }
HRESULT MyDirect3DDevice9Ex::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap) { return m_pDevice->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap); }
void MyDirect3DDevice9Ex::SetCursorPosition(int X, int Y, DWORD Flags) { m_pDevice->SetCursorPosition(X, Y, Flags); }
BOOL MyDirect3DDevice9Ex::ShowCursor(BOOL bShow) { return m_pDevice->ShowCursor(bShow); }

HRESULT MyDirect3DDevice9Ex::CreateAdditionalSwapChain(
    D3DPRESENT_PARAMETERS* pPresentationParameters,
    IDirect3DSwapChain9** pSwapChain)
{
    if (!pSwapChain || !pPresentationParameters)
        return D3DERR_INVALIDCALL;

    *pSwapChain = nullptr;

    if (!m_pDevice)
        return D3DERR_INVALIDCALL;

    IDirect3DSwapChain9* realSC = nullptr;
    HRESULT hr = m_pDevice->CreateAdditionalSwapChain(pPresentationParameters, &realSC);
    if (FAILED(hr))
        return hr;

    if (m_isExD3D9)
    {
        MyDirect3DSwapChain9Ex* wrapper = new (std::nothrow) MyDirect3DSwapChain9Ex(realSC, this, true);
        if (!wrapper)
        {
            realSC->Release();
            return E_OUTOFMEMORY;
        }

        *pSwapChain = wrapper;
        realSC->Release();
        return D3D_OK;
    }

    *pSwapChain = realSC;
    return D3D_OK;
}


HRESULT MyDirect3DDevice9Ex::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
    if (!pSwapChain) return D3DERR_INVALIDCALL;
    *pSwapChain = nullptr;
    if (!m_pDevice) return D3DERR_INVALIDCALL;

    if (iSwapChain == 0 && m_pImplicitSwapChain) {
        *pSwapChain = m_pImplicitSwapChain;
        (*pSwapChain)->AddRef();
        return D3D_OK;
    }

    IDirect3DSwapChain9* realSC = nullptr;
    HRESULT hr = m_pDevice->GetSwapChain(iSwapChain, &realSC);
    if (FAILED(hr)) return hr;

    MyDirect3DSwapChain9Ex* wrapper = new (std::nothrow) MyDirect3DSwapChain9Ex(realSC, this, m_isExD3D9);
    if (!wrapper)
    {
        realSC->Release();
        return E_OUTOFMEMORY;
    }

    realSC->Release();

    if (iSwapChain == 0) {
        m_pImplicitSwapChain = wrapper;
        m_pImplicitSwapChain->AddRef(); 
    }

    *pSwapChain = wrapper;
    return D3D_OK;
}

UINT MyDirect3DDevice9Ex::GetNumberOfSwapChains() { return m_pDevice->GetNumberOfSwapChains(); }

HRESULT MyDirect3DDevice9Ex::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    if (m_pImplicitSwapChain) {
        m_pImplicitSwapChain->Release();
        m_pImplicitSwapChain = nullptr;
    }

    if (pPresentationParameters && pPresentationParameters->SwapEffect >= 5 /*D3DSWAPEFFECT_FLIPEX*/) {
        pPresentationParameters->SwapEffect = D3DSWAPEFFECT_DISCARD;
    }

    return m_pDevice->Reset(pPresentationParameters);
}

HRESULT MyDirect3DDevice9Ex::Present(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) { return m_pDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion); }
HRESULT MyDirect3DDevice9Ex::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) { return m_pDevice->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer); }
HRESULT MyDirect3DDevice9Ex::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) { return m_pDevice->GetRasterStatus(iSwapChain, pRasterStatus); }
HRESULT MyDirect3DDevice9Ex::SetDialogBoxMode(BOOL bEnableDialogs) { return m_pDevice->SetDialogBoxMode(bEnableDialogs); }
void MyDirect3DDevice9Ex::SetGammaRamp(UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp) { m_pDevice->SetGammaRamp(iSwapChain, Flags, pRamp); }
void MyDirect3DDevice9Ex::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp) { m_pDevice->GetGammaRamp(iSwapChain, pRamp); }
HRESULT MyDirect3DDevice9Ex::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) { 
    DWORD safeUsage = Usage & ~D3D9EX_ONLY_USAGE_FLAGS;
    return m_pDevice->CreateTexture(Width, Height, Levels, safeUsage, Format, Pool, ppTexture, pSharedHandle); 
}

HRESULT MyDirect3DDevice9Ex::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) { 
    DWORD safeUsage = Usage & ~D3D9EX_ONLY_USAGE_FLAGS;
    return m_pDevice->CreateVolumeTexture(Width, Height, Depth, Levels, safeUsage, Format, Pool, ppVolumeTexture, pSharedHandle); 
}

HRESULT MyDirect3DDevice9Ex::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) { 
    DWORD safeUsage = Usage & ~D3D9EX_ONLY_USAGE_FLAGS;
    return m_pDevice->CreateCubeTexture(EdgeLength, Levels, safeUsage, Format, Pool, ppCubeTexture, pSharedHandle); 
}

HRESULT MyDirect3DDevice9Ex::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) { 
    DWORD safeUsage = Usage & ~D3D9EX_ONLY_USAGE_FLAGS;
    return m_pDevice->CreateVertexBuffer(Length, safeUsage, FVF, Pool, ppVertexBuffer, pSharedHandle); 
}

HRESULT MyDirect3DDevice9Ex::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) { 
    DWORD safeUsage = Usage & ~D3D9EX_ONLY_USAGE_FLAGS;
    return m_pDevice->CreateIndexBuffer(Length, safeUsage, Format, Pool, ppIndexBuffer, pSharedHandle); 
}
HRESULT MyDirect3DDevice9Ex::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) { return m_pDevice->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle); }
HRESULT MyDirect3DDevice9Ex::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) { return m_pDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle); }
HRESULT MyDirect3DDevice9Ex::UpdateSurface(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, const POINT* pDestPoint) { return m_pDevice->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint); }
HRESULT MyDirect3DDevice9Ex::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) { return m_pDevice->UpdateTexture(pSourceTexture, pDestinationTexture); }
HRESULT MyDirect3DDevice9Ex::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) { return m_pDevice->GetRenderTargetData(pRenderTarget, pDestSurface); }
HRESULT MyDirect3DDevice9Ex::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface) { return m_pDevice->GetFrontBufferData(iSwapChain, pDestSurface); }
HRESULT MyDirect3DDevice9Ex::StretchRect(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) { return m_pDevice->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter); }
HRESULT MyDirect3DDevice9Ex::ColorFill(IDirect3DSurface9* pSurface, const RECT* pRect, D3DCOLOR color) { return m_pDevice->ColorFill(pSurface, pRect, color); }
HRESULT MyDirect3DDevice9Ex::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) { return m_pDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle); }
HRESULT MyDirect3DDevice9Ex::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) { return m_pDevice->SetRenderTarget(RenderTargetIndex, pRenderTarget); }
HRESULT MyDirect3DDevice9Ex::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) { return m_pDevice->GetRenderTarget(RenderTargetIndex, ppRenderTarget); }
HRESULT MyDirect3DDevice9Ex::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) { return m_pDevice->SetDepthStencilSurface(pNewZStencil); }
HRESULT MyDirect3DDevice9Ex::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface) { return m_pDevice->GetDepthStencilSurface(ppZStencilSurface); }
HRESULT MyDirect3DDevice9Ex::BeginScene() { return m_pDevice->BeginScene(); }
HRESULT MyDirect3DDevice9Ex::EndScene() { return m_pDevice->EndScene(); }
HRESULT MyDirect3DDevice9Ex::Clear(DWORD Count, const D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) { return m_pDevice->Clear(Count, pRects, Flags, Color, Z, Stencil); }
HRESULT MyDirect3DDevice9Ex::SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix) { return m_pDevice->SetTransform(State, pMatrix); }
HRESULT MyDirect3DDevice9Ex::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) { return m_pDevice->GetTransform(State, pMatrix); }
HRESULT MyDirect3DDevice9Ex::MultiplyTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix) { return m_pDevice->MultiplyTransform(State, pMatrix); }
HRESULT MyDirect3DDevice9Ex::SetViewport(const D3DVIEWPORT9* pViewport) { return m_pDevice->SetViewport(pViewport); }
HRESULT MyDirect3DDevice9Ex::GetViewport(D3DVIEWPORT9* pViewport) { return m_pDevice->GetViewport(pViewport); }
HRESULT MyDirect3DDevice9Ex::SetMaterial(const D3DMATERIAL9* pMaterial) { return m_pDevice->SetMaterial(pMaterial); }
HRESULT MyDirect3DDevice9Ex::GetMaterial(D3DMATERIAL9* pMaterial) { return m_pDevice->GetMaterial(pMaterial); }
HRESULT MyDirect3DDevice9Ex::SetLight(DWORD Index, const D3DLIGHT9* pLight) { return m_pDevice->SetLight(Index, pLight); }
HRESULT MyDirect3DDevice9Ex::GetLight(DWORD Index, D3DLIGHT9* pLight) { return m_pDevice->GetLight(Index, pLight); }
HRESULT MyDirect3DDevice9Ex::LightEnable(DWORD Index, BOOL Enable) { return m_pDevice->LightEnable(Index, Enable); }
HRESULT MyDirect3DDevice9Ex::GetLightEnable(DWORD Index, BOOL* pEnable) { return m_pDevice->GetLightEnable(Index, pEnable); }
HRESULT MyDirect3DDevice9Ex::SetClipPlane(DWORD Index, const float* pPlane) { return m_pDevice->SetClipPlane(Index, pPlane); }
HRESULT MyDirect3DDevice9Ex::GetClipPlane(DWORD Index, float* pPlane) { return m_pDevice->GetClipPlane(Index, pPlane); }
HRESULT MyDirect3DDevice9Ex::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) { return m_pDevice->SetRenderState(State, Value); }
HRESULT MyDirect3DDevice9Ex::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue) { return m_pDevice->GetRenderState(State, pValue); }
HRESULT MyDirect3DDevice9Ex::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) { return m_pDevice->CreateStateBlock(Type, ppSB); }
HRESULT MyDirect3DDevice9Ex::BeginStateBlock() { return m_pDevice->BeginStateBlock(); }
HRESULT MyDirect3DDevice9Ex::EndStateBlock(IDirect3DStateBlock9** ppSB) { return m_pDevice->EndStateBlock(ppSB); }
HRESULT MyDirect3DDevice9Ex::SetClipStatus(const D3DCLIPSTATUS9* pClipStatus) { return m_pDevice->SetClipStatus(pClipStatus); }
HRESULT MyDirect3DDevice9Ex::GetClipStatus(D3DCLIPSTATUS9* pClipStatus) { return m_pDevice->GetClipStatus(pClipStatus); }
HRESULT MyDirect3DDevice9Ex::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture) { return m_pDevice->GetTexture(Stage, ppTexture); }
HRESULT MyDirect3DDevice9Ex::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture) { return m_pDevice->SetTexture(Stage, pTexture); }
HRESULT MyDirect3DDevice9Ex::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) { return m_pDevice->GetTextureStageState(Stage, Type, pValue); }
HRESULT MyDirect3DDevice9Ex::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) { return m_pDevice->SetTextureStageState(Stage, Type, Value); }
HRESULT MyDirect3DDevice9Ex::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) { return m_pDevice->GetSamplerState(Sampler, Type, pValue); }
HRESULT MyDirect3DDevice9Ex::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) { return m_pDevice->SetSamplerState(Sampler, Type, Value); }
HRESULT MyDirect3DDevice9Ex::ValidateDevice(DWORD* pNumPasses) { return m_pDevice->ValidateDevice(pNumPasses); }
HRESULT MyDirect3DDevice9Ex::SetPaletteEntries(UINT PaletteNumber, const PALETTEENTRY* pEntries) { return m_pDevice->SetPaletteEntries(PaletteNumber, pEntries); }
HRESULT MyDirect3DDevice9Ex::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries) { return m_pDevice->GetPaletteEntries(PaletteNumber, pEntries); }
HRESULT MyDirect3DDevice9Ex::SetCurrentTexturePalette(UINT PaletteNumber) { return m_pDevice->SetCurrentTexturePalette(PaletteNumber); }
HRESULT MyDirect3DDevice9Ex::GetCurrentTexturePalette(UINT* PaletteNumber) { return m_pDevice->GetCurrentTexturePalette(PaletteNumber); }
HRESULT MyDirect3DDevice9Ex::SetScissorRect(const RECT* pRect) { return m_pDevice->SetScissorRect(pRect); }
HRESULT MyDirect3DDevice9Ex::GetScissorRect(RECT* pRect) { return m_pDevice->GetScissorRect(pRect); }
HRESULT MyDirect3DDevice9Ex::SetSoftwareVertexProcessing(BOOL bSoftware) { return m_pDevice->SetSoftwareVertexProcessing(bSoftware); }
BOOL MyDirect3DDevice9Ex::GetSoftwareVertexProcessing() { return m_pDevice->GetSoftwareVertexProcessing(); }
HRESULT MyDirect3DDevice9Ex::SetNPatchMode(float nSegments) { return m_pDevice->SetNPatchMode(nSegments); }
float MyDirect3DDevice9Ex::GetNPatchMode() { return m_pDevice->GetNPatchMode(); }
HRESULT MyDirect3DDevice9Ex::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) { return m_pDevice->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount); }
HRESULT MyDirect3DDevice9Ex::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) { return m_pDevice->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount); }
HRESULT MyDirect3DDevice9Ex::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return m_pDevice->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride); }
HRESULT MyDirect3DDevice9Ex::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride) { return m_pDevice->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride); }
HRESULT MyDirect3DDevice9Ex::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) { return m_pDevice->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags); }
HRESULT MyDirect3DDevice9Ex::CreateVertexDeclaration(const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) { return m_pDevice->CreateVertexDeclaration(pVertexElements, ppDecl); }
HRESULT MyDirect3DDevice9Ex::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) { return m_pDevice->SetVertexDeclaration(pDecl); }
HRESULT MyDirect3DDevice9Ex::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) { return m_pDevice->GetVertexDeclaration(ppDecl); }
HRESULT MyDirect3DDevice9Ex::SetFVF(DWORD FVF) { return m_pDevice->SetFVF(FVF); }
HRESULT MyDirect3DDevice9Ex::GetFVF(DWORD* pFVF) { return m_pDevice->GetFVF(pFVF); }
HRESULT MyDirect3DDevice9Ex::CreateVertexShader(const DWORD* pFunction, IDirect3DVertexShader9** ppShader) { return m_pDevice->CreateVertexShader(pFunction, ppShader); }
HRESULT MyDirect3DDevice9Ex::SetVertexShader(IDirect3DVertexShader9* pShader) { return m_pDevice->SetVertexShader(pShader); }
HRESULT MyDirect3DDevice9Ex::GetVertexShader(IDirect3DVertexShader9** ppShader) { return m_pDevice->GetVertexShader(ppShader); }
HRESULT MyDirect3DDevice9Ex::SetVertexShaderConstantF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount) { return m_pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
HRESULT MyDirect3DDevice9Ex::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) { return m_pDevice->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
HRESULT MyDirect3DDevice9Ex::SetVertexShaderConstantI(UINT StartRegister, const int* pConstantData, UINT Vector4iCount) { return m_pDevice->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
HRESULT MyDirect3DDevice9Ex::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) { return m_pDevice->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
HRESULT MyDirect3DDevice9Ex::SetVertexShaderConstantB(UINT StartRegister, const BOOL* pConstantData, UINT BoolCount) { return m_pDevice->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount); }
HRESULT MyDirect3DDevice9Ex::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) { return m_pDevice->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount); }
HRESULT MyDirect3DDevice9Ex::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) { return m_pDevice->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride); }
HRESULT MyDirect3DDevice9Ex::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride) { return m_pDevice->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride); }
HRESULT MyDirect3DDevice9Ex::SetStreamSourceFreq(UINT StreamNumber, UINT Setting) { return m_pDevice->SetStreamSourceFreq(StreamNumber, Setting); }
HRESULT MyDirect3DDevice9Ex::GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting) { return m_pDevice->GetStreamSourceFreq(StreamNumber, pSetting); }
HRESULT MyDirect3DDevice9Ex::SetIndices(IDirect3DIndexBuffer9* pIndexData) { return m_pDevice->SetIndices(pIndexData); }
HRESULT MyDirect3DDevice9Ex::GetIndices(IDirect3DIndexBuffer9** ppIndexData) { return m_pDevice->GetIndices(ppIndexData); }
HRESULT MyDirect3DDevice9Ex::CreatePixelShader(const DWORD* pFunction, IDirect3DPixelShader9** ppShader) { return m_pDevice->CreatePixelShader(pFunction, ppShader); }
HRESULT MyDirect3DDevice9Ex::SetPixelShader(IDirect3DPixelShader9* pShader) { return m_pDevice->SetPixelShader(pShader); }
HRESULT MyDirect3DDevice9Ex::GetPixelShader(IDirect3DPixelShader9** ppShader) { return m_pDevice->GetPixelShader(ppShader); }
HRESULT MyDirect3DDevice9Ex::SetPixelShaderConstantF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount) { return m_pDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
HRESULT MyDirect3DDevice9Ex::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) { return m_pDevice->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount); }
HRESULT MyDirect3DDevice9Ex::SetPixelShaderConstantI(UINT StartRegister, const int* pConstantData, UINT Vector4iCount) { return m_pDevice->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
HRESULT MyDirect3DDevice9Ex::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) { return m_pDevice->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount); }
HRESULT MyDirect3DDevice9Ex::SetPixelShaderConstantB(UINT StartRegister, const BOOL* pConstantData, UINT BoolCount) { return m_pDevice->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount); }
HRESULT MyDirect3DDevice9Ex::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) { return m_pDevice->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount); }
HRESULT MyDirect3DDevice9Ex::DrawRectPatch(UINT Handle, const float* pNumSegs, const D3DRECTPATCH_INFO* pRectPatchInfo) { return m_pDevice->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo); }
HRESULT MyDirect3DDevice9Ex::DrawTriPatch(UINT Handle, const float* pNumSegs, const D3DTRIPATCH_INFO* pTriPatchInfo) { return m_pDevice->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo); }
HRESULT MyDirect3DDevice9Ex::DeletePatch(UINT Handle) { return m_pDevice->DeletePatch(Handle); }
HRESULT MyDirect3DDevice9Ex::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) { return m_pDevice->CreateQuery(Type, ppQuery); }

// IDirect3DDevice9Ex
HRESULT MyDirect3DDevice9Ex::SetConvolutionMonoKernel(UINT width, UINT height, float* rows, float* columns) { return D3DERR_NOTAVAILABLE; }

HRESULT MyDirect3DDevice9Ex::ComposeRects(IDirect3DSurface9* pSrc, IDirect3DSurface9* pDst, IDirect3DVertexBuffer9* pSrcRectDescs, UINT NumRects, IDirect3DVertexBuffer9* pDstRectDescs, D3DCOMPOSERECTSOP Operation, int Xoffset, int Yoffset) {
    if (!pSrc || !pDst || !pSrcRectDescs || !pDstRectDescs || NumRects == 0) return D3DERR_INVALIDCALL;
    if (Operation != D3DCOMPOSERECTS_COPY) return D3DERR_NOTAVAILABLE;

    struct ComposRectDesc { USHORT X, Y, Width, Height; };
    ComposRectDesc* pSrcDescs = nullptr;
    ComposRectDesc* pDstDescs = nullptr;

    DWORD lockFlags = D3DLOCK_READONLY | D3DLOCK_NOSYSLOCK;
    UINT lockSize = NumRects * sizeof(ComposRectDesc);

    if (FAILED(pSrcRectDescs->Lock(0, lockSize, reinterpret_cast<void**>(&pSrcDescs), lockFlags))) return D3DERR_INVALIDCALL;
    
    if (FAILED(pDstRectDescs->Lock(0, lockSize, reinterpret_cast<void**>(&pDstDescs), lockFlags))) {
        pSrcRectDescs->Unlock();
        return D3DERR_INVALIDCALL;
    }

    D3DSURFACE_DESC dstDesc;
    pDst->GetDesc(&dstDesc);
    
    const LONG maxDstW = static_cast<LONG>(dstDesc.Width);
    const LONG maxDstH = static_cast<LONG>(dstDesc.Height);

    HRESULT hr = S_OK;

    for (UINT i = 0; i < NumRects; ++i) {
        const auto& srcDesc = pSrcDescs[i];
        const auto& dstDescItem = pDstDescs[i];

        RECT src = { srcDesc.X, srcDesc.Y, srcDesc.X + srcDesc.Width, srcDesc.Y + srcDesc.Height };
        RECT dst = { dstDescItem.X + Xoffset, dstDescItem.Y + Yoffset, 
                     dstDescItem.X + Xoffset + dstDescItem.Width, 
                     dstDescItem.Y + Yoffset + dstDescItem.Height };

        if (dst.left < 0) dst.left = 0;
        if (dst.top < 0) dst.top = 0;
        if (dst.right > maxDstW) dst.right = maxDstW;
        if (dst.bottom > maxDstH) dst.bottom = maxDstH;

        LONG dstW = dst.right - dst.left;
        LONG dstH = dst.bottom - dst.top;

        if (dstW <= 0 || dstH <= 0) continue;

        src.right = src.left + dstW;
        src.bottom = src.top + dstH;

        hr = m_pDevice->StretchRect(pSrc, &src, pDst, &dst, D3DTEXF_POINT);
        if (FAILED(hr)) break;
    }

    pDstRectDescs->Unlock();
    pSrcRectDescs->Unlock();
    
    return hr;
}

HRESULT MyDirect3DDevice9Ex::PresentEx(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
    if (dwFlags & D3DPRESENT_DONOTFLIP) {
        return (m_pDevice->TestCooperativeLevel() == D3DERR_DEVICELOST) ? D3DERR_DEVICELOST : D3D_OK;
    }

    HRESULT hr = m_pDevice->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    
    if (hr == D3DERR_INVALIDCALL && (pSourceRect || pDestRect)) { 
        hr = m_pDevice->Present(nullptr, nullptr, hDestWindowOverride, pDirtyRegion); 
    }
    return hr;
}

HRESULT MyDirect3DDevice9Ex::SetGPUThreadPriority(INT Priority)
{
    m_gpuThreadPriority = Priority;
    return D3D_OK;
}

HRESULT MyDirect3DDevice9Ex::GetGPUThreadPriority(INT* pPriority)
{
    if (!pPriority)
        return D3DERR_INVALIDCALL;

    *pPriority = m_gpuThreadPriority;
    return D3D_OK;
}

HRESULT MyDirect3DDevice9Ex::WaitForVBlank(UINT iSwapChain) { return D3D_OK; }
HRESULT MyDirect3DDevice9Ex::CheckResourceResidency(IDirect3DResource9** pResourceArray, UINT32 NumResources) { 
    return S_OK;
}

HRESULT MyDirect3DDevice9Ex::SetMaximumFrameLatency(UINT MaxLatency) {
    if (MaxLatency == 0) MaxLatency = 3;
    if (MaxLatency > 16) return D3DERR_INVALIDCALL;
    m_maxFrameLatency = MaxLatency;
    return D3D_OK;
}

HRESULT MyDirect3DDevice9Ex::GetMaximumFrameLatency(UINT* pMaxLatency) {
    if (!pMaxLatency) return D3DERR_INVALIDCALL;
    *pMaxLatency = m_maxFrameLatency;
    return D3D_OK;
}

HRESULT MyDirect3DDevice9Ex::CheckDeviceState(HWND)
{
    HRESULT hr = m_pDevice->TestCooperativeLevel();

    switch (hr)
    {
    case D3D_OK:
        return D3D_OK;

    case D3DERR_DEVICELOST:
        return D3DERR_DEVICELOST;

    case D3DERR_DEVICENOTRESET:
        return D3DERR_DEVICENOTRESET;

    case D3DERR_DEVICEHUNG:
    case D3DERR_DEVICEREMOVED:
        return hr;

    default:
        return D3DERR_DRIVERINTERNALERROR;
    }
}

HRESULT MyDirect3DDevice9Ex::CreateRenderTargetEx(
    UINT Width,
    UINT Height,
    D3DFORMAT Format,
    D3DMULTISAMPLE_TYPE MultiSample,
    DWORD MultisampleQuality,
    BOOL Lockable,
    IDirect3DSurface9** ppSurface,
    HANDLE* pSharedHandle,
    DWORD Usage)
{
    return m_pDevice->CreateRenderTarget(
        Width,
        Height,
        Format,
        MultiSample,
        MultisampleQuality,
        Lockable,
        ppSurface,
        pSharedHandle);
}

HRESULT MyDirect3DDevice9Ex::CreateOffscreenPlainSurfaceEx(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage) {
DWORD safeUsage = Usage & ~D3D9EX_ONLY_USAGE_FLAGS;
HRESULT hr = m_pDevice->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);

    if (FAILED(hr) && Pool == D3DPOOL_DEFAULT) {
        hr = m_pDevice->CreateOffscreenPlainSurface(Width, Height, Format, D3DPOOL_SYSTEMMEM, ppSurface, nullptr);
    }
    return hr;
}

HRESULT MyDirect3DDevice9Ex::CreateDepthStencilSurfaceEx(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage) {
    DWORD safeUsage = Usage & ~D3D9EX_ONLY_USAGE_FLAGS;
    return m_pDevice->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT MyDirect3DDevice9Ex::ResetEx(D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode)
{
    if (!pPresentationParameters) return D3DERR_INVALIDCALL;

    if (pFullscreenDisplayMode) {
        pPresentationParameters->BackBufferWidth  = pFullscreenDisplayMode->Width;
        pPresentationParameters->BackBufferHeight = pFullscreenDisplayMode->Height;
        pPresentationParameters->BackBufferFormat = pFullscreenDisplayMode->Format;
        pPresentationParameters->FullScreen_RefreshRateInHz = pFullscreenDisplayMode->RefreshRate;
    }

    return Reset(pPresentationParameters);
}


HRESULT MyDirect3DDevice9Ex::GetDisplayModeEx(UINT iSwapChain, D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation) {
    if (!pMode) return D3DERR_INVALIDCALL;
    D3DDISPLAYMODE dm;
    HRESULT hr = m_pDevice->GetDisplayMode(iSwapChain, &dm);
    if (SUCCEEDED(hr)) {
        pMode->Size = sizeof(D3DDISPLAYMODEEX);
        pMode->Width = dm.Width;
        pMode->Height = dm.Height;
        pMode->RefreshRate = dm.RefreshRate;
        pMode->Format = dm.Format;
        pMode->ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE;
        if (pRotation) *pRotation = D3DDISPLAYROTATION_IDENTITY;
    }
    return hr;
}



MyDirect3DSwapChain9Ex::MyDirect3DSwapChain9Ex(IDirect3DSwapChain9* pSwapChain,
                                               MyDirect3DDevice9Ex* pDeviceEx,
                                               bool isExDevice)
    : m_pSwapChain(pSwapChain)
    , m_pDeviceEx(pDeviceEx)
    , m_refCount(1)
    , m_isExDevice(isExDevice)
{
    if (m_pSwapChain)
        m_pSwapChain->AddRef();
}

MyDirect3DSwapChain9Ex::~MyDirect3DSwapChain9Ex()
{
    if (m_pSwapChain)
        m_pSwapChain->Release();
}

HRESULT MyDirect3DSwapChain9Ex::QueryInterface(REFIID riid, void **ppvObj) {
    if (!ppvObj) return E_POINTER;

    if (riid == IID_IUnknown || riid == IID_IDirect3DSwapChain9) {
        *ppvObj = static_cast<IDirect3DSwapChain9*>(this);
        AddRef();
        return S_OK;
    }
    if (riid == IID_IDirect3DSwapChain9Ex) {
        if (m_isExDevice) {
            *ppvObj = static_cast<IDirect3DSwapChain9Ex*>(this);
            AddRef();
            return S_OK;
        }
        *ppvObj = nullptr;
        return E_NOINTERFACE;
    }
    *ppvObj = nullptr;
    if (m_pSwapChain) return m_pSwapChain->QueryInterface(riid, ppvObj);
    return E_NOINTERFACE;
}

ULONG MyDirect3DSwapChain9Ex::AddRef()
{
    return (ULONG)InterlockedIncrement(reinterpret_cast<LONG*>(&m_refCount));
}

ULONG MyDirect3DSwapChain9Ex::Release()
{
    ULONG count = (ULONG)InterlockedDecrement(reinterpret_cast<LONG*>(&m_refCount));
    if (count == 0)
        delete this;
    return count;
}

HRESULT MyDirect3DSwapChain9Ex::Present(const RECT* pSourceRect, const RECT* pDestRect,
                                         HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags)
{
    if (dwFlags & D3DPRESENT_DONOTFLIP) {
        return D3D_OK; 
    }

    DWORD safeFlags = dwFlags & ~(D3DPRESENT_DONOTFLIP | D3DPRESENT_UPDATEOVERLAYONLY | D3DPRESENT_HIDEOVERLAY);

    HRESULT hr = m_pSwapChain->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion, safeFlags);
    
    if (hr == D3DERR_INVALIDCALL && (dwFlags & D3DPRESENT_UPDATEOVERLAYONLY)) {
        hr = m_pSwapChain->Present(nullptr, nullptr, nullptr, nullptr, 0);
    }

    return hr;
}

HRESULT MyDirect3DSwapChain9Ex::GetFrontBufferData(IDirect3DSurface9* pDestSurface)
{
    return m_pSwapChain->GetFrontBufferData(pDestSurface);
}

HRESULT MyDirect3DSwapChain9Ex::GetBackBuffer(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type,
                                               IDirect3DSurface9** ppBackBuffer)
{
    return m_pSwapChain->GetBackBuffer(iBackBuffer, Type, ppBackBuffer);
}

HRESULT MyDirect3DSwapChain9Ex::GetRasterStatus(D3DRASTER_STATUS* pRasterStatus)
{
    return m_pSwapChain->GetRasterStatus(pRasterStatus);
}

HRESULT MyDirect3DSwapChain9Ex::GetDisplayMode(D3DDISPLAYMODE* pMode)
{
    return m_pSwapChain->GetDisplayMode(pMode);
}

HRESULT MyDirect3DSwapChain9Ex::GetDevice(IDirect3DDevice9** ppDevice)
{
    if (!ppDevice) return D3DERR_INVALIDCALL;
    *ppDevice = m_pDeviceEx;
    if (m_pDeviceEx) m_pDeviceEx->AddRef();
    return D3D_OK;
}

HRESULT MyDirect3DSwapChain9Ex::GetPresentParameters(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    return m_pSwapChain->GetPresentParameters(pPresentationParameters);
}

HRESULT MyDirect3DSwapChain9Ex::GetLastPresentCount(UINT* pLastPresentCount)
{
    if (!pLastPresentCount) return E_POINTER;
    *pLastPresentCount = 0;
    return S_OK;
}

HRESULT MyDirect3DSwapChain9Ex::GetPresentStats(D3DPRESENTSTATS* pPresentationStatistics)
{
    if (!pPresentationStatistics) return E_POINTER;
    ZeroMemory(pPresentationStatistics, sizeof(D3DPRESENTSTATS));
    pPresentationStatistics->PresentRefreshCount = 0;
    pPresentationStatistics->SyncRefreshCount = 0;
    pPresentationStatistics->SyncQPCTime.QuadPart = 0;
    pPresentationStatistics->SyncGPUTime.QuadPart = 0;
    return S_OK;
}

HRESULT MyDirect3DSwapChain9Ex::GetDisplayModeEx(D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation)
{
    if (!pMode || pMode->Size != sizeof(D3DDISPLAYMODEEX))
        return D3DERR_INVALIDCALL;

    if (!m_pSwapChain)
        return D3DERR_INVALIDCALL;

    D3DDISPLAYMODE dm;
    ZeroMemory(&dm, sizeof(dm));

    HRESULT hr = m_pSwapChain->GetDisplayMode(&dm);
    if (FAILED(hr))
        return hr;

    pMode->Width = dm.Width;
    pMode->Height = dm.Height;
    pMode->RefreshRate = dm.RefreshRate;
    pMode->Format = dm.Format;
    pMode->ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE;

    if (pRotation)
        *pRotation = D3DDISPLAYROTATION_IDENTITY;

    return D3D_OK;
}
