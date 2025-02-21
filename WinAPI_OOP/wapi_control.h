#pragma once

#ifndef WINAPI_CONTROL_H
#define WINAPI_CONTROL_H

#include <windows.h>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>
#include <set>

#define CONTROL_NAME(class) L ## #class

#define DECLARE_CLASS_NAME(class) \
std::wstring ClassName() const override \
{ return CONTROL_NAME(class); }

class Control
{
protected:
	HWND hwnd;
	HINSTANCE hInstance;
	std::wstring text;

	int x, y, cx, cy;

	void InitComponent(Control* parent);

private:
	static std::map<HWND, Control*> controls;
	static std::map<std::wstring, std::shared_ptr<WNDCLASS>> classes;

public:
	static void Register(std::wstring name, HINSTANCE hInstance);

	static inline bool IsRegistered(std::wstring name)
	{
		std::set<std::wstring> defaultClasses = {
			L"BUTTON", L"EDIT", L"COMBOBOX", L"LISTBOX", L"ScrollBar"
		};

		return defaultClasses.find(name) != defaultClasses.end() ||
			classes.find(name) != classes.end();
	}

	static LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	void SendMessageTo(Control* other, UINT msg, WPARAM wp, LPARAM lp)
	{
		SendMessageW(other->hwnd, msg, wp, lp);
	}

	Control(HINSTANCE hInstance, Control* parent);

	virtual ~Control();

	void SetText(std::wstring text);
	std::wstring GetText() const;
	void SetStyle(DWORD style);
	void SetSize(int cx, int cy);
	void SetPosition(int x, int y);

protected:
	virtual LRESULT OnPaint(HDC& hdc, PAINTSTRUCT& ps, WPARAM wp, LPARAM lp);
	virtual LRESULT OnCommand(WPARAM wp, LPARAM lp);
	virtual LRESULT OnClose(WPARAM wp, LPARAM lp);
	virtual std::wstring ClassName() const = 0;
	virtual DWORD DefaultStyle() const = 0;

public:
	virtual WORD GetId() const = 0;
};

#endif // WINAPI_CONTROL_H