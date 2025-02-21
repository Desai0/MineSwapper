#pragma once

#ifndef WINAPI_OOP_H
#define WINAPI_OOP_H

#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "wapi_control.h"
#include "wapi_nontopcontrol.h"
#include "wapi_interactive.h"
#include "wapi_paintbox.h"
#include "wapi_window.h"

#define RegisterControl(class, hInstance) Control::Register(CONTROL_NAME(class), hInstance)

#define REGISTER_DEFAULT(hInstance) \
RegisterControl(PaintBox, hInstance);

#endif // WINAPI_OOP_H