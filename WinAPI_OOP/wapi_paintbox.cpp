#include "wapi_paintbox.h"

constexpr const LPARAM mycall = 1337;

PaintBox::PaintBox(HINSTANCE hInstance, WORD id, Control* parent) : NonTopControl(hInstance, id, parent), pen(nullptr)
{
	if (!Control::IsRegistered(ClassName())) throw std::logic_error("PaintBox is not registered");

	InitComponent(parent);
}

PaintBox::~PaintBox()
{
	if (pen != nullptr) DeleteObject(pen);
}

void PaintBox::SetPen(PenStyle style, int width, Color color)
{
	if (pen != nullptr) DeleteObject(pen);
	pen = CreatePen(style, width, RGB(color.r, color.g, color.b));
}

LRESULT PaintBox::OnPaint(HDC& hdc, PAINTSTRUCT& ps, WPARAM wp, LPARAM lp)
{
	if (lp == mycall)
	{
		SelectObject(hdc, pen);
		for (auto const& pp : paintEvents)
		{
			switch (pp.figure)
			{
			case Figure::LINE:
				MoveToEx(hdc, pp.x1, pp.y1, nullptr);
				LineTo(hdc, pp.x2, pp.y2);
				break;
			case Figure::RECTANGLE:
				Rectangle(hdc, pp.x1, pp.y1, pp.x2, pp.y2);
				break;
			case Figure::ELLIPSE:
				Ellipse(hdc, pp.x1, pp.y1, pp.x2, pp.y2);
				break;
			case -1:
				TextOutW(hdc, pp.x1, pp.y1, pp.text.c_str(), pp.text.size());
				break;
			}
		}
		ValidateRect(hwnd, nullptr);
		paintEvents.clear();
	}
	return 0;
}

DWORD PaintBox::DefaultStyle() const
{
	return NonTopControl::DefaultStyle() | WS_BORDER;
}