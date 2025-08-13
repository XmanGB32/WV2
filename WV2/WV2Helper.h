#pragma once
#include <string>

namespace WV2Helper
{
    // Returns the directory where the current executable resides
    std::wstring GetExecutableDirectory();

    // Returns the path for WebView2 user data folder, based on executable directory
    std::wstring GetWebView2UserDataFolder();
}