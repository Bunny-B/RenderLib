#ifndef RENDER_H
#define RENDER_H

namespace render
{
	extern char checkKeyToggle(int vKey);

	extern char createWindowW(void* instance,const wchar_t* windowsName, int cmd_show, void(*renderFunction)());

	extern char createWindowA(void* instance, const char* windowsName, int cmd_show, void(*renderFunction)());

	extern char createOverlayW(void* instance, const wchar_t* windowsName, int cmd_show, void(*renderFunction)());

	extern char createOverlayA(void* instance, const char* windowsName, int cmd_show, void(*renderFunction)());

	extern void toggleOverlayVisible();

	extern void makeOverlayClickable();

	extern void makeOverlayNonClickable();

	extern void draw_callback();
};

#endif // !RENDER_H
