#include "render.h"

#include "d3d9.h"
#include "d3d11.h"

#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_win32.h"
#include "stdio.h"

#include "dwmapi.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace render {

    static HWND window = NULL;
    static IDXGISwapChain* swap_chain = NULL;
    static ID3D11RenderTargetView* render_target_view = NULL;
    static ID3D11Device* device = NULL;

 
    wchar_t* A_to_W(const char* text) {
        if (text == NULL) return NULL;

        const int w_size = MultiByteToWideChar(CP_ACP, 0, text, -1, NULL, 0);
        if (w_size == 0)  return NULL;  // Failed conversion

        wchar_t* wString = (wchar_t*)malloc(w_size * sizeof(wchar_t));
        if (wString == NULL) return NULL;
        MultiByteToWideChar(CP_ACP, 0, text, -1, wString, w_size);
        return wString;
    }
    //https://stackoverflow.com/questions/3019977/convert-wchar-t-to-char
    char* W_to_A(const wchar_t* pwchar)
    {
        // get the number of characters in the string.
        int currentCharIndex = 0;
        char currentChar = pwchar[currentCharIndex];

        while (currentChar != '\0')
        {
            currentCharIndex++;
            currentChar = pwchar[currentCharIndex];
        }

        const int charCount = currentCharIndex + 1;

        // allocate a new block of memory size char (1 byte) instead of wide char (2 bytes)
        char* aString = (char*)malloc(sizeof(char) * charCount);

        for (int i = 0; i < charCount; i++)
        {
            // convert to char (1 byte)
            char character = pwchar[i];

            *aString = character;

            aString += sizeof(char);

        }
        aString += '\0';

        aString -= (sizeof(char) * charCount);

        return aString;
    }

    static void InternalIcon(const int size,const int iconType, const wchar_t* pathIcon) {
        // Load the small icon (16x16)
        HICON hIcon = (HICON)LoadImageW(
            nullptr,                 // No module, because we're loading from a file
            pathIcon,                // Path to your icon file
            IMAGE_ICON,              // Loading an icon
            size, size,                  // Desired width and height
            LR_LOADFROMFILE          // Load the icon from file
        );
        // Set the small icon
        if (hIcon) {
            SendMessage(window, WM_SETICON, iconType, (LPARAM)hIcon);
        }
    }

    extern char setSmallWindowIconW(const wchar_t* pathIcon) {
        if (window == NULL) return FALSE;
        InternalIcon(16, ICON_SMALL, pathIcon);
        return TRUE;
    }

    extern char setSmallWindowIconA(const char* pathIcon) {
        wchar_t* wString = NULL;
        if (wString = A_to_W(pathIcon)) {
            char res = setSmallWindowIconW(wString);
            free(wString); wString = NULL;
            return TRUE;
        }
        return FALSE;
    }

    char setWindowIconW(const wchar_t* pathIcon) {
        if (window == NULL) return FALSE;
        InternalIcon(32, ICON_BIG, pathIcon);
        return TRUE;
    }

    char setWindowIconA(const char* pathIcon) {
        wchar_t* wString = A_to_W(pathIcon);
        if (wString == NULL) return FALSE;

        char res = setWindowIconW(wString);

        free(wString); wString = NULL;
        return res;
    }

    char checkKeyToggle(int vKey) {
        static bool previousState = false;
        // Check if the specified key is currently pressed
        bool currentState = (GetAsyncKeyState(vKey) & 0x8000) != 0;

        // Detect key press (transition from not pressed to pressed)
        bool keyPressed = currentState && !previousState;

        previousState = currentState;
        return keyPressed;
    }

    static char ExistingSwapchain(void* externalSwapChain, uintptr_t device_context_address) {
        swap_chain = (IDXGISwapChain*)externalSwapChain;

        void* d3d_device{};
        HRESULT hr = swap_chain->GetDevice(__uuidof(IDXGIDevice), (void**)&d3d_device);
        if (FAILED(hr))
            return FALSE;
        
        device = (ID3D11Device*)d3d_device;
        device->GetImmediateContext((ID3D11DeviceContext**)device_context_address);

        return TRUE;
    }

    static char createRenderTargetView() {
        ID3D11Texture2D* back_buffer = NULL;
        ((IDXGISwapChain*)swap_chain)->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

        if (back_buffer) {
            device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
            back_buffer->Release();
            return TRUE;
        }
        else {
            return TRUE;
        }
    }

    static void releaseTargetView() {
        if (render_target_view)
        {
            render_target_view->Release();
            render_target_view = NULL;
        }
    }

    LRESULT CALLBACK windowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(window, message, wParam, lParam))
            return true;

        switch (message)
        {
        case WM_SIZE:
        {
            if (swap_chain != NULL) {
                releaseTargetView();
                HRESULT hr = ((IDXGISwapChain*)swap_chain)->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                if (FAILED(hr)) {
                    // Handle error (log, cleanup, etc.)
                }
                createRenderTargetView();
            }
            return 0;
        }
        case WM_DPICHANGED:
        {
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(window, nullptr, suggested_rect->left, suggested_rect->top,
                suggested_rect->right - suggested_rect->left,
                suggested_rect->bottom - suggested_rect->top,
                SWP_NOZORDER | SWP_NOACTIVATE);
            return 0; // Add return here for clarity
        }
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(window, message, wParam, lParam);
        }
    }


    void draw_callback() {
        if (ImGui::BeginTabBar("rabbar")) {

            if (ImGui::BeginTabItem("TAB 1")) {
                ImGui::Text("Tab item");
                ImGui::EndTabItem();
            }


            if (ImGui::BeginTabItem("TAB 2")) {
                ImGui::Text("Tab item");
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }

    static char createWindow(HINSTANCE instance,HWND* window, PWNDCLASSEXW wc, LPCWSTR windowsName, char isOverlay, HICON hIcon, HICON hIconSm) {
        SetProcessDPIAware();
        wc->cbSize = sizeof(WNDCLASSEXW);
        if (hIcon != NULL) wc->hIcon = hIcon;
        if (hIcon != hIconSm) wc->hIconSm = hIconSm;

        wc->style = CS_HREDRAW | CS_VREDRAW;
        wc->lpfnWndProc = windowProcedure;
        wc->cbClsExtra = 0;
        wc->cbWndExtra = 0;
        wc->hInstance = instance;
        wc->lpszClassName = L" ";
        wc->hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
        // Register the window class
        RegisterClassExW(wc);
        const DWORD dwStyle = isOverlay ? WS_POPUP : WS_OVERLAPPEDWINDOW;
        const DWORD dwExStyle = isOverlay ? WS_EX_TOPMOST | WS_EX_TRANSPARENT : 0;
        // Create the window with WS_OVERLAPPEDWINDOW style for standard window features (close, minimize, maximize, etc.)
        *window = CreateWindowExW(
            dwExStyle,                                  // Optional extended window style (can use WS_EX_APPWINDOW or others if needed)
            wc->lpszClassName,                 // Window class name
            windowsName,                   // Window title
            dwStyle,               // Window style providing standard features (close, minimize, maximize buttons, system menu)
            0, 0,          // Default window position (x, y)
            1920, 1080,                // Window size (width, height)
            nullptr,                      // No parent window
            nullptr,                         // No menu
            wc->hInstance,                         // Handle to the application instance
            nullptr                       // No additional application data
        );

        if (isOverlay) {
            SetLayeredWindowAttributes(*window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);
            RECT window_area{};

            {
                GetWindowRect(*window, &window_area);
                POINT diff{};
                ClientToScreen(*window, &diff);

                const MARGINS margins{
                    window_area.left + (diff.x - window_area.left),
                    window_area.top + (diff.x - window_area.top),
                    window_area.right,
                    window_area.bottom
                };

                DwmExtendFrameIntoClientArea(*window, &margins);
            }
        }
        return TRUE;
    }

    static char createSwapChain(uintptr_t device_context_address) {

        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = window;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        constexpr D3D_FEATURE_LEVEL levels[2]{
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_0
        };

        D3D_FEATURE_LEVEL level{};

        D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0U,
            levels,
            2U,
            D3D11_SDK_VERSION,
            &sd,
            &swap_chain,
            &device,
            &level,
            (ID3D11DeviceContext**)device_context_address
        );

        if (swap_chain == NULL)  return FALSE;
        if (device == NULL)      return FALSE;

        return TRUE;
    }

    static void scaleFontForMonitor() {
        const HMONITOR monitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
        MONITORINFO info = {};
        info.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(monitor, &info);
        const int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;

        if (monitor_height > 1080)
        {
            const float fScale = 2.0f;
            ImFontConfig cfg;
            cfg.SizePixels = 13 * fScale;
            ImGui::GetIO().Fonts->AddFontDefault(&cfg);
        }
    }

    static ImGuiWindowFlags currentFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize;

    void setWindowFlags(int flags) { currentFlags = flags; }
    void addWindowFlags(int flags) { currentFlags |= flags; }
    void removeWindowFlags(int flags) { currentFlags &= ~flags; }

    void ChangeWindowTitleA(const char* newTitle) {
        SetWindowTextA(window, newTitle);
    }
    void ChangeWindowTitleW(const wchar_t* newTitle) {
        SetWindowTextW(window, newTitle);
    }
    static void mainWindow(char isOverlay, void(*renderFunction)()) {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;

        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f)); // Position at the top-left corner
        ImGui::SetNextWindowSize(displaySize); // Size equal to the display size

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, isOverlay ? ImVec4(0.f, 0.f, 0.f, 0.f) : ImVec4(1.f, 1.f, 1.f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_Text, { 0,0,0,255 });

        // Use the flags passed or the currentFlags
        if (ImGui::Begin("main window", nullptr, currentFlags)) {
            renderFunction();
            ImGui::End();
        }

        ImGui::PopStyleVar(1);
        ImGui::PopStyleColor(2);
    }   

    static void setupImGuiFonts() {
        ImGuiIO& io = ImGui::GetIO();

        // Load Segoe UI font from Windows font directory default windows font
        io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);  // Set the size as needed (e.g., 18.0f)
        ImGui::GetIO().Fonts->Build();
    }

    static char drawInternal(void* instance, const wchar_t* windowsName, int cmd_show, void(*renderFunction)(),char isOverlay,void* hIcon,void* hIconSm) {
        WNDCLASSEXW wc = { 0 };
        createWindow((HINSTANCE)instance, &window, &wc, windowsName, isOverlay,(HICON)hIcon, (HICON)hIconSm);

        ShowWindow(window, SW_SHOW);
        UpdateWindow(window);

        ID3D11DeviceContext* device_context = NULL;
        createSwapChain((uintptr_t)&device_context);
        createRenderTargetView();

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        scaleFontForMonitor();
        ImGui::GetIO().IniFilename = nullptr;

        ImGui_ImplWin32_Init(window);
        ImGui_ImplDX11_Init(device, device_context);

        setupImGuiFonts();

        
        bool running = true;
        while (running) {
            MSG msg;
            
            
            while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {            
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (msg.message == WM_QUIT) {
                    running = false;
                }
            }
            
            
            

            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();

            ImGui::NewFrame();
            mainWindow(isOverlay, renderFunction);
            ImGui::Render();

            constexpr float color[4]{ 0.f,0.f,0.f,0.f };
            device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
            device_context->ClearRenderTargetView(render_target_view, color);

            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            ((IDXGISwapChain*)swap_chain)->Present(1U, 0U);
        }

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        if (swap_chain) {
            ((IDXGISwapChain*)swap_chain)->Release();
        }

        if (device_context) {
            device_context->Release();
        }

        if (device) {
            device->Release();
        }

            releaseTargetView();
            DestroyWindow(window);
            UnregisterClassW(wc.lpszClassName, wc.hInstance);
        
        return TRUE;
    }

    char createWindowA(void* instance, const char* windowsName, int cmd_show, void(*renderFunction)(), void* hIcon, void* hIconSm) {

        wchar_t* wString = A_to_W(windowsName);
        if (wString == NULL) return FALSE;
       const char res = createWindowW(instance, wString, cmd_show, renderFunction,(HICON)hIcon, (HICON)hIconSm);

       free(wString); wString = NULL;
       return res;
    }

    char createWindowW(void* instance, const wchar_t* windowsName, int cmd_show, void(*renderFunction)(),void* hIcon, void* hIconSm) {
        return drawInternal(instance, windowsName, cmd_show, renderFunction, FALSE, (HICON)hIcon, (HICON)hIconSm);
    }

    char createOverlayA(void* instance, const char* windowsName, int cmd_show, void(*renderFunction)(), void* hIcon, void* hIconSm) {
       wchar_t* wString = A_to_W(windowsName);
       if (wString == NULL) return FALSE;

       const char res = createOverlayW(instance, wString, cmd_show, renderFunction,(HICON)hIcon, (HICON)hIconSm);

       free(wString); wString = NULL;
       return res;
    }

    char createOverlayW(void* instance, const wchar_t* windowsName, int cmd_show, void(*renderFunction)(), void* hIcon, void* hIconSm) {
       return drawInternal(instance, L"", cmd_show, renderFunction, TRUE, (HICON)hIcon, (HICON)hIconSm);
    }

    void toggleOverlayVisible() {
        IsIconic(window) ? 
            ShowWindow(window, SW_RESTORE) : ShowWindow(window, SW_MINIMIZE);
    }

    void makeOverlayClickable() {
        // Remove the WS_EX_TRANSPARENT flag to make the window clickable
        SetWindowLongPtr(window, GWL_EXSTYLE,
            GetWindowLongPtr(window, GWL_EXSTYLE) & ~WS_EX_TRANSPARENT);
    }

    void makeOverlayNonClickable() {
        // Add the WS_EX_TRANSPARENT flag to make the window click-through
        SetWindowLongPtr(window, GWL_EXSTYLE,
            GetWindowLongPtr(window, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
    }

}