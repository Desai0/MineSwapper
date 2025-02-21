#pragma once

#ifndef WINAPI_INTERACTIVE_H
#define WINAPI_INTERACTIVE_H

#include "wapi_nontopcontrol.h"

class Button : public NonTopControl
{
public:
	Button(HINSTANCE hInstance, WORD id, Control* parent = nullptr) : NonTopControl(hInstance, id, parent)
	{
		InitComponent(parent);
	}

protected:
	DECLARE_CLASS_NAME(BUTTON)

protected:
	DWORD DefaultStyle() const override
	{
		return NonTopControl::DefaultStyle() | WS_TABSTOP | BS_PUSHBUTTON;
	}
};

class LineEdit final : public NonTopControl
{
public:
	LineEdit(HINSTANCE hInstance, WORD id, Control* parent = nullptr) : NonTopControl(hInstance, id, parent)
	{
		InitComponent(parent);
	}

protected:
	DECLARE_CLASS_NAME(EDIT)

	DWORD DefaultStyle() const override
	{
		return NonTopControl::DefaultStyle() | WS_BORDER | ES_LEFT;
	}
};

#endif // WINAPI_INTERACTIVE_H