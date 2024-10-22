#ifndef RENDER_H
#define RENDER_H

namespace render
{

	extern void* getWindow();

	extern char checkKeyToggle(int vKey);

	extern char createWindowW(void* instance,const wchar_t* windowsName, int cmd_show, void(*renderFunction)(), void* hIcon, void* hIconSm);

	extern char createWindowA(void* instance, const char* windowsName, int cmd_show, void(*renderFunction)(), void* hIcon, void* hIconSm);

	extern char createOverlayW(void* instance, const wchar_t* windowsName, int cmd_show, void(*renderFunction)(), void* hIcon, void* hIconSm);

	extern char createOverlayA(void* instance, const char* windowsName, int cmd_show, void(*renderFunction)(), void* hIcon, void* hIconSm);

	extern void toggleOverlayVisible();

	extern void makeOverlayClickable();

	extern void makeOverlayNonClickable();

	extern void draw_callback();

	extern char setWindowIconW(const wchar_t* pathIcon);  

	extern char setWindowIconA(const char* pathIcon);  

	extern char setSmallWindowIconW(const wchar_t* pathIcon);

	extern char setSmallWindowIconA(const char* pathIcon);
};

#endif // !RENDER_H
