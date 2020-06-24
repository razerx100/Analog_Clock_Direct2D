#pragma once
#include<Windows.h>
template<class Derived>
class BaseWindow{
protected:
	virtual LRESULT handle_messages(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	virtual PCWSTR class_name() const = 0;

	HWND m_hwnd;

public:
	BaseWindow() : m_hwnd(NULL) { }

	static LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		Derived* pThis = NULL;
		if (uMsg == WM_NCCREATE) {
			CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			pThis = reinterpret_cast<Derived*>(pCreate->lpCreateParams);
			SetWindowLongW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

			pThis->m_hwnd = hwnd;
		}
		else {
			pThis = reinterpret_cast<Derived*>(GetWindowLongW(hwnd, GWLP_USERDATA));
		}
		if (pThis) {
			return pThis->handle_messages(uMsg, wParam, lParam);
		}
		else {
			return DefWindowProcW(hwnd, uMsg, wParam, lParam);
		}
	}

	HWND window() const { return m_hwnd; }

	BOOL create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		INT nWidth = CW_USEDEFAULT, INT nHeight = CW_USEDEFAULT,
		INT x = CW_USEDEFAULT, INT y = CW_USEDEFAULT,
		DWORD dwExStyle = 0, HWND hwndParent = 0,
		HMENU hMenu = 0
	) {
		WNDCLASSW wc = { };
		wc.lpfnWndProc = Derived::window_proc;
		wc.hInstance = GetModuleHandleW(0);
		wc.lpszClassName = class_name();

		RegisterClassW(&wc);

		m_hwnd = CreateWindowExW(
			dwExStyle, class_name(), lpWindowName, dwStyle,
			x, y, nWidth, nHeight, hwndParent, hMenu,
			GetModuleHandleW(0), this);
		return (m_hwnd ? TRUE : FALSE);
	}
};

