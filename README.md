# RenderLib

RenderLib is a rendering library that utilizes ImGui for GUI rendering. This library is intended for use in projects that require high-performance graphical user interfaces with DirectX.

## Getting Started

### Clone the Repository

To clone the repository, including the ImGui submodule, use the following command:

In PowerShell
```bash
 git clone --recurse-submodules https://github.com/Bunny-B/RenderLib.git --depth 1
```
If an issue occurs during cloning (e.g., connection problems), you can adjust Git settings and try again:
```bash
git config --global http.postBuffer 524288000
git config --global http.version HTTP/1.1
git config --global http.lowSpeedLimit 0      # Disable low speed limit
git config --global http.lowSpeedTime 999999  # Set low speed time 
git clone --recurse-submodules https://github.com/Bunny-B/RenderLib.git --depth 1
git config --global --unset http.postBuffer
git config --global --unset http.version
git config --global --unset http.lowSpeedLimit
git config --global --unset http.lowSpeedTime
```
### Linking and Including RenderLib in Your Project

1. **Link the RenderLib Library**:  
   Ensure you link the `RenderLib.lib` and `d3d11.lib` in your project settings. This will allow you to use the rendering functionality provided by RenderLib.
   `$(DXSDK_DIR)Lib\x64\d3d11.lib`
2. **Include Headers**:  
   Add the following headers to your project:

   ```cpp
   #include "vendor/RenderLib/render.h"
   #include "vendor/imgui/imgui.h"
   ```
3. **Example**:  
```cpp
#include <Windows.h>
//Dont forget to link RenderLib.lib d3d11.lib
#include "vendor/RenderLib/render.h"
#include "vendor/imgui/imgui.h"

#define VK_INSERT         0x2D
void draw_callback();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    HICON hIcon   = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_BIG));
    HICON hIconSm = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    if (hIcon == NULL || hIconSm == NULL) {
        MessageBoxW(NULL, L"Failed to load icon.", L"Error", MB_OK | MB_ICONERROR);
    }

    render::createWindowW(hInstance, L"PEye", nCmdShow, draw_callback, hIcon, hIconSm);
    //render::createWindowW(hInstance, L"PEye", nCmdShow, draw_callback, NULL, NULL);
    //render::createOverlayW(hInstance, L"PEye overlay", nShowCmd, renderFunction,NULL,NULL);


    return 5;
}

void draw_callback() {
    if (render::checkKeyToggle(VK_INSERT))
        render::toggleOverlayVisible();
}

```
