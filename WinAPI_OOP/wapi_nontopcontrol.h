#pragma once

#include "wapi_control.h"

#ifndef WINAPI_NONTOPCONTROL_H
#define WINAPI_NONTOPCONTROL_H

class NonTopControl : public Control
{
	WORD id;

protected:
	Control* parent;

public:
	NonTopControl(HINSTANCE hInstance, WORD id, Control* parent);

	void SetFont(HFONT font);

	void SetEnabled(bool value);

	WORD GetId() const final;

protected:
	DWORD DefaultStyle() const override;
};

#endif