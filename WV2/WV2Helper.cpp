#include "WV2Helper.h"
#include <windows.h>
#include <shlobj.h>           // for SHGetKnownFolderPath
#include <string>

// Retrieves the directory where the executable resides
std::wstring GetExecutableDirectory()
{
    wchar_t fullPath[MAX_PATH] = { 0 };
    // Get the full path of the current module (EXE)
    if (GetModuleFileNameW(nullptr, fullPath, MAX_PATH) == 0) {
        // Failed to get module filename
        return L"";
    }

    std::wstring path(fullPath);
    // Find last directory separator
    auto pos = path.find_last_of(L"\\/");
    if (pos == std::wstring::npos) {
        return L"";
    }
    // Return directory part only
    return path.substr(0, pos);
}

// Returns a folder path for WebView2 user data, based on the executable directory
std::wstring GetWebView2UserDataFolder()
{
    std::wstring folder = GetExecutableDirectory();
    if (folder.empty()) {
        // Fallback or handle error
        return L"";
    }
    folder += L"\\WV2UserData";

    // Create directory if it doesn't exist
    if (CreateDirectoryW(folder.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS) {
        // Directory exists or was created successfully
        return folder;
    }
    else {
        // Failed to create directory
        // Optionally handle error
        return L"";
    }
}