#ifndef D3D9EX_WRAPPER_H
#define D3D9EX_WRAPPER_H

#include <d3d9.h>
#include <windows.h>
#include <map>

typedef IDirect3D9* (WINAPI* Direct3DCreate9_t)(UINT SDKVersion);

typedef HRESULT (WINAPI* CheckFullscreen_t)(UINT Adapter, HWND hWnd, DWORD dwFlags);
typedef void* (WINAPI* Direct3DShaderValidatorCreate9_t)();
typedef HRESULT (WINAPI* PSGPError_t)(void* pDevice, DWORD Error, DWORD Value);
typedef HRESULT (WINAPI* PSGPSampleTexture_t)(void* pDevice, DWORD Stage, float* pCoord, DWORD Flags, float* pColor);

typedef int     (WINAPI* D3DPERF_BeginEvent_t)(D3DCOLOR color, LPCWSTR name);
typedef int     (WINAPI* D3DPERF_EndEvent_t)();
typedef DWORD   (WINAPI* D3DPERF_GetStatus_t)();
typedef BOOL    (WINAPI* D3DPERF_QueryRepeatFrame_t)();
typedef void    (WINAPI* D3DPERF_SetMarker_t)(D3DCOLOR color, LPCWSTR name);
typedef void    (WINAPI* D3DPERF_SetOptions_t)(DWORD options);
typedef void    (WINAPI* D3DPERF_SetRegion_t)(D3DCOLOR color, LPCWSTR name);

typedef void    (WINAPI* DebugSetLevel_t)(int level, int mute);
typedef void    (WINAPI* DebugSetMute_t)();

extern Direct3DCreate9_t                Direct3DCreate9_original;
extern CheckFullscreen_t                CheckFullscreen_original;
extern Direct3DShaderValidatorCreate9_t Direct3DShaderValidatorCreate9_original;
extern PSGPError_t                      PSGPError_original;
extern PSGPSampleTexture_t              PSGPSampleTexture_original;
extern D3DPERF_BeginEvent_t             D3DPERF_BeginEvent_original;
extern D3DPERF_EndEvent_t               D3DPERF_EndEvent_original;
extern D3DPERF_GetStatus_t              D3DPERF_GetStatus_original;
extern D3DPERF_QueryRepeatFrame_t       D3DPERF_QueryRepeatFrame_original;
extern D3DPERF_SetMarker_t              D3DPERF_SetMarker_original;
extern D3DPERF_SetOptions_t             D3DPERF_SetOptions_original;
extern D3DPERF_SetRegion_t              D3DPERF_SetRegion_original;
extern DebugSetLevel_t                  DebugSetLevel_original;
extern DebugSetMute_t                   DebugSetMute_original;

class MyDirect3D9Ex : public IDirect3D9Ex {
private:
    IDirect3D9* m_pD3D9;
    ULONG m_refCount;
    bool m_isExD3D9;

public:
    MyDirect3D9Ex(IDirect3D9* pD3D9, bool isEx);
    virtual ~MyDirect3D9Ex();
    bool IsExD3D9() const { return m_isExD3D9; }

    // IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // IDirect3D9
    STDMETHOD(RegisterSoftwareDevice)(void* pInitializeFunction);
    STDMETHOD_(UINT, GetAdapterCount)();
    STDMETHOD(GetAdapterIdentifier)(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier);
    STDMETHOD_(UINT, GetAdapterModeCount)(UINT Adapter, D3DFORMAT Format);
    STDMETHOD(EnumAdapterModes)(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode);
    STDMETHOD(GetAdapterDisplayMode)(UINT Adapter, D3DDISPLAYMODE* pMode);
    STDMETHOD(CheckDeviceType)(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed);
    STDMETHOD(CheckDeviceFormat)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat);
    STDMETHOD(CheckDeviceMultiSampleType)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels);
    STDMETHOD(CheckDepthStencilMatch)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat);
    STDMETHOD(CheckDeviceFormatConversion)(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);
    STDMETHOD(GetDeviceCaps)(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps);
    STDMETHOD_(HMONITOR, GetAdapterMonitor)(UINT Adapter);
    STDMETHOD(CreateDevice)(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface);

    // IDirect3D9Ex
    STDMETHOD_(UINT, GetAdapterModeCountEx)(UINT Adapter, const D3DDISPLAYMODEFILTER* pFilter);
    STDMETHOD(EnumAdapterModesEx)(UINT Adapter, const D3DDISPLAYMODEFILTER* pFilter, UINT Mode, D3DDISPLAYMODEEX* pMode);
    STDMETHOD(GetAdapterDisplayModeEx)(UINT Adapter, D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation);
    STDMETHOD(CreateDeviceEx)(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode, IDirect3DDevice9Ex** ppReturnedDeviceInterface);
    STDMETHOD(GetAdapterLUID)(UINT Adapter, LUID* pLUID);
};

class MyDirect3DSwapChain9Ex;

class MyDirect3DDevice9Ex : public IDirect3DDevice9Ex {
private:
    IDirect3DDevice9* m_pDevice;
    MyDirect3D9Ex* m_pD3D9Ex;
    ULONG m_refCount;
    INT m_gpuThreadPriority;
    UINT m_maxFrameLatency;
    DWORD m_OriginalBehaviorFlags;
    bool m_isExD3D9;
    std::map<IDirect3DSwapChain9*, MyDirect3DSwapChain9Ex*> m_swapChainWrappers;

public:
    MyDirect3DDevice9Ex(IDirect3DDevice9* pDevice, MyDirect3D9Ex* pD3D9Ex, DWORD originalBehaviorFlags, bool isEx);
    virtual ~MyDirect3DDevice9Ex();

    bool IsExDevice() const { return m_isExD3D9; }
    bool m_lastWindowed;

    void RemoveSwapChainWrapper(IDirect3DSwapChain9* pOrig);

    // IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // IDirect3DDevice9
    STDMETHOD(TestCooperativeLevel)();
    STDMETHOD_(UINT, GetAvailableTextureMem)();
    STDMETHOD(EvictManagedResources)();
    STDMETHOD(GetDirect3D)(IDirect3D9** ppD3D9);
    STDMETHOD(GetDeviceCaps)(D3DCAPS9* pCaps);
    STDMETHOD(GetDisplayMode)(UINT iSwapChain, D3DDISPLAYMODE* pMode);
    STDMETHOD(GetCreationParameters)(D3DDEVICE_CREATION_PARAMETERS* pParameters);
    STDMETHOD(SetCursorProperties)(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap);
    STDMETHOD_(void, SetCursorPosition)(int X, int Y, DWORD Flags);
    STDMETHOD_(BOOL, ShowCursor)(BOOL bShow);
    STDMETHOD(CreateAdditionalSwapChain)(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
    STDMETHOD(GetSwapChain)(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
    STDMETHOD_(UINT, GetNumberOfSwapChains)();
    STDMETHOD(Reset)(D3DPRESENT_PARAMETERS* pPresentationParameters);
    STDMETHOD(Present)(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
    STDMETHOD(GetBackBuffer)(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
    STDMETHOD(GetRasterStatus)(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus);
    STDMETHOD(SetDialogBoxMode)(BOOL bEnableDialogs);
    STDMETHOD_(void, SetGammaRamp)(UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp);
    STDMETHOD_(void, GetGammaRamp)(UINT iSwapChain, D3DGAMMARAMP* pRamp);
    STDMETHOD(CreateTexture)(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle);
    STDMETHOD(CreateVolumeTexture)(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle);
    STDMETHOD(CreateCubeTexture)(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle);
    STDMETHOD(CreateVertexBuffer)(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle);
    STDMETHOD(CreateIndexBuffer)(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle);
    STDMETHOD(CreateRenderTarget)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    STDMETHOD(CreateDepthStencilSurface)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    STDMETHOD(UpdateSurface)(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, const POINT* pDestPoint);
    STDMETHOD(UpdateTexture)(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture);
    STDMETHOD(GetRenderTargetData)(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface);
    STDMETHOD(GetFrontBufferData)(UINT iSwapChain, IDirect3DSurface9* pDestSurface);
    STDMETHOD(StretchRect)(IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter);
    STDMETHOD(ColorFill)(IDirect3DSurface9* pSurface, const RECT* pRect, D3DCOLOR color);
    STDMETHOD(CreateOffscreenPlainSurface)(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);
    STDMETHOD(SetRenderTarget)(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget);
    STDMETHOD(GetRenderTarget)(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget);
    STDMETHOD(SetDepthStencilSurface)(IDirect3DSurface9* pNewZStencil);
    STDMETHOD(GetDepthStencilSurface)(IDirect3DSurface9** ppZStencilSurface);
    STDMETHOD(BeginScene)();
    STDMETHOD(EndScene)();
    STDMETHOD(Clear)(DWORD Count, const D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);
    STDMETHOD(SetTransform)(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix);
    STDMETHOD(GetTransform)(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
    STDMETHOD(MultiplyTransform)(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix);
    STDMETHOD(SetViewport)(const D3DVIEWPORT9* pViewport);
    STDMETHOD(GetViewport)(D3DVIEWPORT9* pViewport);
    STDMETHOD(SetMaterial)(const D3DMATERIAL9* pMaterial);
    STDMETHOD(GetMaterial)(D3DMATERIAL9* pMaterial);
    STDMETHOD(SetLight)(DWORD Index, const D3DLIGHT9* pLight);
    STDMETHOD(GetLight)(DWORD Index, D3DLIGHT9* pLight);
    STDMETHOD(LightEnable)(DWORD Index, BOOL Enable);
    STDMETHOD(GetLightEnable)(DWORD Index, BOOL* pEnable);
    STDMETHOD(SetClipPlane)(DWORD Index, const float* pPlane);
    STDMETHOD(GetClipPlane)(DWORD Index, float* pPlane);
    STDMETHOD(SetRenderState)(D3DRENDERSTATETYPE State, DWORD Value);
    STDMETHOD(GetRenderState)(D3DRENDERSTATETYPE State, DWORD* pValue);
    STDMETHOD(CreateStateBlock)(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB);
    STDMETHOD(BeginStateBlock)();
    STDMETHOD(EndStateBlock)(IDirect3DStateBlock9** ppSB);
    STDMETHOD(SetClipStatus)(const D3DCLIPSTATUS9* pClipStatus);
    STDMETHOD(GetClipStatus)(D3DCLIPSTATUS9* pClipStatus);
    STDMETHOD(GetTexture)(DWORD Stage, IDirect3DBaseTexture9** ppTexture);
    STDMETHOD(SetTexture)(DWORD Stage, IDirect3DBaseTexture9* pTexture);
    STDMETHOD(GetTextureStageState)(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
    STDMETHOD(SetTextureStageState)(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    STDMETHOD(GetSamplerState)(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue);
    STDMETHOD(SetSamplerState)(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
    STDMETHOD(ValidateDevice)(DWORD* pNumPasses);
    STDMETHOD(SetPaletteEntries)(UINT PaletteNumber, const PALETTEENTRY* pEntries);
    STDMETHOD(GetPaletteEntries)(UINT PaletteNumber, PALETTEENTRY* pEntries);
    STDMETHOD(SetCurrentTexturePalette)(UINT PaletteNumber);
    STDMETHOD(GetCurrentTexturePalette)(UINT* PaletteNumber);
    STDMETHOD(SetScissorRect)(const RECT* pRect);
    STDMETHOD(GetScissorRect)(RECT* pRect);
    STDMETHOD(SetSoftwareVertexProcessing)(BOOL bSoftware);
    STDMETHOD_(BOOL, GetSoftwareVertexProcessing)();
    STDMETHOD(SetNPatchMode)(float nSegments);
    STDMETHOD_(float, GetNPatchMode)();
    STDMETHOD(DrawPrimitive)(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
    STDMETHOD(DrawIndexedPrimitive)(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
    STDMETHOD(DrawPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    STDMETHOD(DrawIndexedPrimitiveUP)(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, UINT VertexStreamZeroStride);
    STDMETHOD(ProcessVertices)(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags);
    STDMETHOD(CreateVertexDeclaration)(const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);
    STDMETHOD(SetVertexDeclaration)(IDirect3DVertexDeclaration9* pDecl);
    STDMETHOD(GetVertexDeclaration)(IDirect3DVertexDeclaration9** ppDecl);
    STDMETHOD(SetFVF)(DWORD FVF);
    STDMETHOD(GetFVF)(DWORD* pFVF);
    STDMETHOD(CreateVertexShader)(const DWORD* pFunction, IDirect3DVertexShader9** ppShader);
    STDMETHOD(SetVertexShader)(IDirect3DVertexShader9* pShader);
    STDMETHOD(GetVertexShader)(IDirect3DVertexShader9** ppShader);
    STDMETHOD(SetVertexShaderConstantF)(UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
    STDMETHOD(GetVertexShaderConstantF)(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    STDMETHOD(SetVertexShaderConstantI)(UINT StartRegister, const int* pConstantData, UINT Vector4iCount);
    STDMETHOD(GetVertexShaderConstantI)(UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    STDMETHOD(SetVertexShaderConstantB)(UINT StartRegister, const BOOL* pConstantData, UINT BoolCount);
    STDMETHOD(GetVertexShaderConstantB)(UINT StartRegister, BOOL* pConstantData, UINT BoolCount);
    STDMETHOD(SetStreamSource)(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride);
    STDMETHOD(GetStreamSource)(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride);
    STDMETHOD(SetStreamSourceFreq)(UINT StreamNumber, UINT Setting);
    STDMETHOD(GetStreamSourceFreq)(UINT StreamNumber, UINT* pSetting);
    STDMETHOD(SetIndices)(IDirect3DIndexBuffer9* pIndexData);
    STDMETHOD(GetIndices)(IDirect3DIndexBuffer9** ppIndexData);
    STDMETHOD(CreatePixelShader)(const DWORD* pFunction, IDirect3DPixelShader9** ppShader);
    STDMETHOD(SetPixelShader)(IDirect3DPixelShader9* pShader);
    STDMETHOD(GetPixelShader)(IDirect3DPixelShader9** ppShader);
    STDMETHOD(SetPixelShaderConstantF)(UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
    STDMETHOD(GetPixelShaderConstantF)(UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    STDMETHOD(SetPixelShaderConstantI)(UINT StartRegister, const int* pConstantData, UINT Vector4iCount);
    STDMETHOD(GetPixelShaderConstantI)(UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    STDMETHOD(SetPixelShaderConstantB)(UINT StartRegister, const BOOL* pConstantData, UINT BoolCount);
    STDMETHOD(GetPixelShaderConstantB)(UINT StartRegister, BOOL* pConstantData, UINT BoolCount);
    STDMETHOD(DrawRectPatch)(UINT Handle, const float* pNumSegs, const D3DRECTPATCH_INFO* pRectPatchInfo);
    STDMETHOD(DrawTriPatch)(UINT Handle, const float* pNumSegs, const D3DTRIPATCH_INFO* pTriPatchInfo);
    STDMETHOD(DeletePatch)(UINT Handle);
    STDMETHOD(CreateQuery)(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);

    // IDirect3DDevice9Ex
    STDMETHOD(SetConvolutionMonoKernel)(UINT width, UINT height, float* rows, float* columns);
    STDMETHOD(ComposeRects)(IDirect3DSurface9* pSrc, IDirect3DSurface9* pDst, IDirect3DVertexBuffer9* pSrcRectDescs, UINT NumRects, IDirect3DVertexBuffer9* pDstRectDescs, D3DCOMPOSERECTSOP Operation, int Xoffset, int Yoffset);
    STDMETHOD(PresentEx)(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags);
    STDMETHOD(GetGPUThreadPriority)(INT* pPriority);
    STDMETHOD(SetGPUThreadPriority)(INT Priority);
    STDMETHOD(WaitForVBlank)(UINT iSwapChain);
    STDMETHOD(CheckResourceResidency)(IDirect3DResource9** pResourceArray, UINT32 NumResources);
    STDMETHOD(SetMaximumFrameLatency)(UINT MaxLatency);
    STDMETHOD(GetMaximumFrameLatency)(UINT* pMaxLatency);
    STDMETHOD(CheckDeviceState)(HWND hDestinationWindow);
    STDMETHOD(CreateRenderTargetEx)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage);
    STDMETHOD(CreateOffscreenPlainSurfaceEx)(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage);
    STDMETHOD(CreateDepthStencilSurfaceEx)(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle, DWORD Usage);
    STDMETHOD(ResetEx)(D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* pFullscreenDisplayMode);
    STDMETHOD(GetDisplayModeEx)(UINT iSwapChain, D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation);
};

class MyDirect3DSwapChain9Ex : public IDirect3DSwapChain9Ex {
private:
    IDirect3DSwapChain9* m_pSwapChain;
    MyDirect3DDevice9Ex* m_pDeviceEx;
    ULONG m_refCount;
    bool m_isExDevice;
public:
    MyDirect3DSwapChain9Ex(IDirect3DSwapChain9* pSwapChain, MyDirect3DDevice9Ex* pDeviceEx, bool isExDevice);
    virtual ~MyDirect3DSwapChain9Ex();

    // IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // IDirect3DSwapChain9
    STDMETHOD(Present)(const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion, DWORD dwFlags);
    STDMETHOD(GetFrontBufferData)(IDirect3DSurface9* pDestSurface);
    STDMETHOD(GetBackBuffer)(UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
    STDMETHOD(GetRasterStatus)(D3DRASTER_STATUS* pRasterStatus);
    STDMETHOD(GetDisplayMode)(D3DDISPLAYMODE* pMode);
    STDMETHOD(GetDevice)(IDirect3DDevice9** ppDevice);
    STDMETHOD(GetPresentParameters)(D3DPRESENT_PARAMETERS* pPresentationParameters);

    // IDirect3DSwapChain9Ex
    STDMETHOD(GetLastPresentCount)(UINT* pLastPresentCount);
    STDMETHOD(GetPresentStats)(D3DPRESENTSTATS* pPresentationStatistics);
    STDMETHOD(GetDisplayModeEx)(D3DDISPLAYMODEEX* pMode, D3DDISPLAYROTATION* pRotation);
};

#endif // D3D9EX_WRAPPER_H
