#pragma once

#ifndef WINAPI_WINDOW_H
#define WINAPI_WINDOW_H

#include "wapi_control.h"

class Window : public Control
{
public:
	Window(HINSTANCE hInstance, Control* parent = nullptr);

	inline void Show()
	{
		ShowWindow(hwnd, SW_SHOW);
	}

	void ProcessMessages();

protected:
	DWORD DefaultStyle() const override;

private:
	WORD GetId() const final;
};

#endif // WINAPI_WINDOW_H
