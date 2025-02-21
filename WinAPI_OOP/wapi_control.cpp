#include "wapi_control.h"

Control::Control(HINSTANCE hInstance, Control* parent) : hInstance(hInstance), hwnd(nullptr)
{
	x = y = cx = cy = 0;
}

Control::~Control()
{
	controls.erase(hwnd);
	DestroyWindow(hwnd);
}

void Control::InitComponent(Control* parent)
{
	hwnd = CreateWindowExW(0,
		ClassName().c_str(),
		L"",
		DefaultStyle(),
		CW_USEDEFAULT, CW_USEDEFAULT,
		100, 100,
		parent == nullptr ? nullptr : parent->hwnd,
		parent == nullptr ? 0 : reinterpret_cast<HMENU>(GetId()),
		parent == nullptr ? hInstance : reinterpret_cast<HINSTANCE>(GetWindowLongPtr(parent->hwnd, GWLP_HINSTANCE)),
		nullptr
	);

	if (hwnd == nullptr) throw std::logic_error("Could not create class: " + std::to_string(GetLastError()));

	cx = cy = 100;
	RECT r{}, rp{};
	GetWindowRect(hwnd, &r);
	if (parent != nullptr)
	{
		GetWindowRect(parent->hwnd, &rp);
		x = r.left - rp.left;
		y = r.top - rp.top;
	}
	else
	{
		x = r.left;
		y = r.top;
	}

	Control::controls.insert({ hwnd, this });
}

void Control::Register(std::wstring name, HINSTANCE hInstance)
{
	std::shared_ptr<WNDCLASS> wnd(new WNDCLASS{});
	wnd->lpfnWndProc = WinProc;
	wnd->lpszClassName = name.c_str();
	wnd->hInstance = hInstance;

	classes.insert({ name, wnd });

	RegisterClassW(wnd.get());
}

#define START_MSG(msg) case msg: try {
#define END_MSG } catch (std::out_of_range const & e) { return DefWindowProcW(hwnd, msg, wp, lp); } break;

LRESULT Control::WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	LRESULT res;
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		START_MSG(WM_CLOSE)
			Control* ctrl = controls.at(hwnd);
			res = ctrl->OnClose(wp, lp);
		END_MSG

		START_MSG(WM_COMMAND)
			Control* ctrl = controls.at(hwnd);
			res = ctrl->OnCommand(wp, lp);
		END_MSG

		START_MSG(WM_PAINT)
			Control* ctrl = controls.at(hwnd);
			PAINTSTRUCT ps{};
			HDC hdc = BeginPaint(hwnd, &ps);
			res = ctrl->OnPaint(hdc, ps, wp, lp);
			EndPaint(hwnd, &ps);
		END_MSG

	default:
		res = DefWindowProcW(hwnd, msg, wp, lp);
	}
	return res;
}

#undef START_MSG
#undef END_MSG

void Control::SetText(std::wstring text)
{
	this->text = text;
	SetWindowTextW(hwnd, this->text.c_str());
}

std::wstring Control::GetText() const
{
	return text;
}

void Control::SetStyle(DWORD style)
{
	SetWindowLongPtrW(hwnd, GWL_STYLE, style);
	ShowWindow(hwnd, SW_SHOW);
}

void Control::SetSize(int cx, int cy)
{
	this->cx = cx;
	this->cy = cy;
	SetWindowPos(hwnd, nullptr, x, y, cx, cy, 0);
}

void Control::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
	SetWindowPos(hwnd, nullptr, x, y, cx, cy, 0);
}

LRESULT Control::OnPaint(HDC& hdc, PAINTSTRUCT& ps, WPARAM wp, LPARAM lp) { return DefWindowProcW(hwnd, WM_PAINT, wp, lp); };
LRESULT Control::OnCommand(WPARAM wp, LPARAM lp) { return DefWindowProcW(hwnd, WM_COMMAND, wp, lp); };
LRESULT Control::OnClose(WPARAM wp, LPARAM lp) { return DefWindowProcW(hwnd, WM_CLOSE, wp, lp); };

std::map<HWND, Control*> Control::controls = {};
std::map<std::wstring, std::shared_ptr<WNDCLASS>> Control::classes = {};