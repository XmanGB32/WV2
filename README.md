![WV2 Banner](https://github.com/XmanGB32/WV2/banner.jpg?raw=true)

# Let's Connect

We're building something cool with GFA-BASIC 32 and WebView2—and we'd love to hear from you.

Whether you're curious, tinkering, or just passing through, feel free to:

🗨️ [Visit our developer forum](https://gb32.proboards.com)

Ask questions, share ideas, or just see what others are up to. No pressure—just good conversation.

# WebView2 Bridge for GFA-BASIC 32

## Keywords

WebView2, VB6, GFA-BASIC 32, ActiveX, COM, 32-bit, browser control, embedded web, legacy Windows, WebView2 bridge, VB6 alternative

## Overview

This project provides a native C++ DLL that acts as a bridge between GFA-BASIC 32 and the Microsoft Edge WebView2 control. It allows GFA-BASIC developers to embed modern web content directly into their Windows desktop applications.

The DLL exposes a simple API for creating, resizing, navigating, and destroying WebView2 instances hosted inside standard Windows child windows.

## Features

- Embed WebView2 browser instances inside GFA-BASIC forms
- Navigate to any URL programmatically
- Resize and reposition the embedded browser
- Cleanly destroy and release resources
- Thread-safe management of multiple WebView2 instances

## Requirements

- Windows 10 or later
- Microsoft Edge (Chromium-based)
- WebView2 Runtime installed
- Visual Studio 2022 (for building the DLL)
- GFA-BASIC 32 (for integration)

## Project Structure

| File                  | Description                                 |
|-----------------------|---------------------------------------------|
| `WV2.sln`             | Visual Studio solution file                 |
| `WV2.vcxproj`         | Visual Studio project file                  |
| `WebView2Bridge.cpp`  | Main C++ source file                        |
| `WebView2Bridge.h`    | Header for WebView2 wrapper class           |
| `WV2Helper.cpp`       | Supporting C++ source file                  |
| `WV2Helper.h`         | Header for helper functions                 |
| `WV2.def`             | Module definition file for DLL exports      |
| `packages.config`     | NuGet package references                    |
| `Welcome.txt`         | Optional documentation or greeting          |
| `LICENSE.md`          | MIT License for open-source usage           |
| `README.md`           | This file                                   |

## Usage

1. Build the DLL using Visual Studio 2022.
2. In your GFA-BASIC 32 project, declare the DLL functions:
   - `CreateWebView2HostExW`
   - `NavigateWebView2W`
   - `ResizeWebView2`
   - `DestroyWebView2Host`
3. Use these functions to embed and control WebView2 instances.

## License

This project is licensed under the MIT License. You are free to use, modify, and distribute it for any purpose, including commercial applications. See `LICENSE.txt` for details.

## Contributing

Pull requests and suggestions are welcome. If you find bugs or want to extend functionality, feel free to fork the project and submit improvements.

## Contact

For questions or feedback, please open an issue on the GitHub repository.

