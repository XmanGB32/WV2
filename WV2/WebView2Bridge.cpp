// WebView2Embed.cpp
#include <windows.h>
#include <map>
#include <memory>
#include <string>
#include <mutex>
#include "WebView2.h"
#include <wrl.h>
#include <wil/com.h>

using namespace Microsoft::WRL;

#pragma comment(lib, "Shlwapi.lib")
// Commented out to get Loader to be static instad of dynamic 
// (ie: required as a seperate file.
//#pragma comment(lib, "WebView2Loader.dll.lib")

// COM lifecycle
static bool g_comInitialized = false;
void InitializeCOM() {
    if (!g_comInitialized) {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (SUCCEEDED(hr)) g_comInitialized = true;
        else OutputDebugString(L"[COM] Initialization failed\n");
    }
}
void UninitializeCOM() {
    if (g_comInitialized) {
        CoUninitialize();
        g_comInitialized = false;
    }
}

// WebView2 wrapper
class WebView2Wrapper {
public:
    WebView2Wrapper(HWND host, RECT bounds, std::wstring userDataFolder, std::wstring url)
        : m_host(host), m_bounds(bounds), m_userDataFolder(std::move(userDataFolder)), m_url(std::move(url)) {
    }

    bool Initialize() {
        return SUCCEEDED(CreateCoreWebView2EnvironmentWithOptions(
            nullptr,
            m_userDataFolder.empty() ? nullptr : m_userDataFolder.c_str(),
            nullptr,
            Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [this](HRESULT hrEnv, ICoreWebView2Environment* env) -> HRESULT {
                    if (FAILED(hrEnv)) {
                        OutputDebugString(L"[WebView2] Environment creation failed\n");
                        return hrEnv;
                    }
                    env->CreateCoreWebView2Controller(
                        m_host,
                        Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                            [this](HRESULT hrCtrl, ICoreWebView2Controller* controller) -> HRESULT {
                                if (FAILED(hrCtrl)) {
                                    OutputDebugString(L"[WebView2] Controller creation failed\n");
                                    return hrCtrl;
                                }
                                m_controller = controller;
                                m_controller->get_CoreWebView2(&m_webview);
                                m_controller->put_Bounds(m_bounds);
                                m_controller->put_IsVisible(TRUE);
                                if (!m_url.empty()) m_webview->Navigate(m_url.c_str());
                                return S_OK;
                            }).Get());
                    return S_OK;
                }).Get()));
    }

    void Resize(int x, int y, int w, int h) {
        m_bounds = { x, y, x + w, y + h };
        if (m_controller) {
            SetWindowPos(m_host, nullptr, x, y, w, h, SWP_NOZORDER);
            m_controller->put_Bounds(m_bounds);
        }
    }

    void Navigate(const std::wstring& url) {
        if (m_webview) m_webview->Navigate(url.c_str());
    }

    void Close() {
        if (m_controller) m_controller->Close();
        m_controller.reset();
        m_webview.reset();
        if (IsWindow(m_host)) DestroyWindow(m_host);
    }

private:
    HWND m_host;
    RECT m_bounds;
    std::wstring m_userDataFolder;
    std::wstring m_url;
    wil::com_ptr<ICoreWebView2> m_webview;
    wil::com_ptr<ICoreWebView2Controller> m_controller;
};

// Global map with thread safety
static std::map<HWND, std::unique_ptr<WebView2Wrapper>> g_webviews;
static std::mutex g_mapMutex;

// Host window class
static const wchar_t* kWebViewHostClass = L"WebViewHost";
void RegisterWebViewHostClass() {
    static bool registered = false;
    if (!registered) {
        WNDCLASS wc = { 0 };
        wc.lpfnWndProc = DefWindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = kWebViewHostClass;
        RegisterClass(&wc);
        registered = true;
    }
}

// DLL API
extern "C" __declspec(dllexport)
HWND __stdcall CreateWebView2HostExW(HWND parent, int x, int y, int w, int h,
    const wchar_t* userDataFolderW, const wchar_t* initialUrlW)
{
    InitializeCOM();
    RegisterWebViewHostClass();

    HWND host = CreateWindowEx(0, kWebViewHostClass, nullptr,
        WS_CHILD | WS_VISIBLE,
        x, y, w, h,
        parent, nullptr, GetModuleHandle(nullptr), nullptr);

    if (!host) {
        OutputDebugString(L"[WebView2] Host window creation failed\n");
        return nullptr;
    }

    RECT bounds = { x, y, x + w, y + h };
    auto wrapper = std::make_unique<WebView2Wrapper>(
        host, bounds,
        userDataFolderW ? std::wstring(userDataFolderW) : L"",
        initialUrlW ? std::wstring(initialUrlW) : L"");

    if (!wrapper->Initialize()) {
        DestroyWindow(host);
        return nullptr;
    }

    {
        std::lock_guard<std::mutex> lock(g_mapMutex);
        g_webviews[host] = std::move(wrapper);
    }

    PostMessage(parent, WM_APP + 100, (WPARAM)host, 0);
    return host;
}

extern "C" __declspec(dllexport)
void __stdcall NavigateWebView2W(HWND hostHwnd, const wchar_t* urlWide) {
    std::lock_guard<std::mutex> lock(g_mapMutex);
    auto it = g_webviews.find(hostHwnd);
    if (it != g_webviews.end()) it->second->Navigate(std::wstring(urlWide));
}

extern "C" __declspec(dllexport)
void __stdcall ResizeWebView2(HWND hostHwnd, int x, int y, int w, int h) {
    std::lock_guard<std::mutex> lock(g_mapMutex);
    auto it = g_webviews.find(hostHwnd);
    if (it != g_webviews.end()) it->second->Resize(x, y, w, h);
}

extern "C" __declspec(dllexport)
void __stdcall DestroyWebView2Host(HWND hostHwnd) {
    std::lock_guard<std::mutex> lock(g_mapMutex);
    auto it = g_webviews.find(hostHwnd);
    if (it != g_webviews.end()) {
        it->second->Close();
        g_webviews.erase(it);
    }
}
