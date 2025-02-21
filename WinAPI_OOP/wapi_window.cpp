#include "wapi_window.h"

Window::Window(HINSTANCE hInstance, Control* parent)
	: Control(hInstance, parent)
{ }

void Window::ProcessMessages()
{
	MSG message;
	while (GetMessageA(&message, hwnd, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
}

DWORD Window::DefaultStyle() const
{
	return WS_OVERLAPPEDWINDOW;
}

WORD Window::GetId() const
{
	return NULL;
}