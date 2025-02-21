#pragma once

#ifndef WINAPI_PAINTBOX_H
#define WINAPI_PAINTBOX_H

#include "wapi_nontopcontrol.h"
#include <list>

extern const LPARAM mycall;

struct Color
{
	int r, g, b;
};

enum Figure : long
{
	LINE = 0,
	RECTANGLE = 1,
	ELLIPSE = 2
};

class PaintBox : public NonTopControl
{
	struct PaintParams
	{
		long figure;
		int x1, y1, x2, y2;
		std::wstring text;
	};

	HPEN pen;
	std::list<PaintParams> paintEvents;

public:

	enum PenStyle
	{
		Solid = PS_SOLID,
		Dashed = PS_DASH,
		Dotted = PS_DOT,
		DashDot = PS_DASHDOT,
		DashDotDot = PS_DASHDOTDOT,
		None = PS_NULL
	};

	PaintBox(HINSTANCE hInstance, WORD id, Control* parent);
	~PaintBox();

	void SetPen(PenStyle style, int width, Color color);

	template <Figure f>
	void Draw(int x1, int y1, int x2, int y2)
	{
		if (pen == nullptr) throw std::logic_error("Pen is not set");

		PaintParams pp{ f, x1, y1, x2, y2 };
		paintEvents.push_back(pp);
	}

	void DrawString(int x, int y, std::wstring text)
	{
		PaintParams pp{ -1, x, y };
		pp.text = text;
		paintEvents.push_back(pp);
	}

	inline void Update()
	{
		PostMessageW(hwnd, WM_PAINT, 0, mycall);
	}

protected:
	LRESULT OnPaint(HDC& hdc, PAINTSTRUCT& ps, WPARAM wp, LPARAM) override;

	DWORD DefaultStyle() const override;

	DECLARE_CLASS_NAME(PaintBox)
};

#endif // WINAPI_PAINTBOX_H