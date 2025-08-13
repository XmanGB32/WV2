#pragma once

#include <windows.h>
#include <wrl.h>
#include "WebView2.h"        // From the WebView2 SDK

using Microsoft::WRL::ComPtr;

//-----------------------------------------------------------------------------
// Holds the pair of controller + WebView for each host HWND
//-----------------------------------------------------------------------------
struct WebViewEntry
{
    ComPtr<ICoreWebView2Controller> controller;
    ComPtr<ICoreWebView2> webview;
};

//-----------------------------------------------------------------------------
// C-style exports (wide-character strings); convert inside the .cpp
//-----------------------------------------------------------------------------
extern "C" {

    // Creates a WebView2 host window, initializes WebView2, and navigates to URL
    __declspec(dllexport) HWND __stdcall CreateWebView2HostExW(
        HWND parent,
        int x,
        int y,
        int width,
        int height,
        const wchar_t* userDataFolderW,
        const wchar_t* initialUrlW);

    // Navigates the WebView2 to the specified URL
    __declspec(dllexport) void __stdcall NavigateWebView2W(
        HWND hostHwnd,
        const wchar_t* urlW);

    // Resizes the WebView2 container
    __declspec(dllexport) void __stdcall ResizeWebView2(
        HWND hostHwnd,
        int x,
        int y,
        int width,
        int height);

    // Destroys the WebView2 host window and cleans up resources
    __declspec(dllexport) void __stdcall DestroyWebView2Host(
        HWND hostHwnd);
}
