# RenderLib

RenderLib is a rendering library that utilizes ImGui for GUI rendering. This library is intended for use in projects that require high-performance graphical user interfaces with DirectX.

## Getting Started

### Clone the Repository

To clone the repository, including the ImGui submodule, use the following command:

In PowerShell
```bash
 git clone --recurse-submodules https://github.com/Bunny-B/RenderLib.git --depth 1
```
If issue occure 

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
