#include "wapi_nontopcontrol.h"

NonTopControl::NonTopControl(HINSTANCE hInstance, WORD id, Control* parent)
	: Control(hInstance, parent)
	, parent(parent)
	, id(id)
{ }

void NonTopControl::SetFont(HFONT font)
{
	SendMessageW(hwnd, WM_SETFONT, (WPARAM)font, (LPARAM)MAKELONG(TRUE, 0));
}

void NonTopControl::SetEnabled(bool value)
{
	EnableWindow(hwnd, value);
}

WORD NonTopControl::GetId() const
{
	return id;
}

DWORD NonTopControl::DefaultStyle() const
{
	return WS_VISIBLE | WS_CHILD;
}