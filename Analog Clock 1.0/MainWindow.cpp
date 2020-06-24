#include"MainWindow.h"

PCWSTR MainWindow::class_name() const {
	return L"Analog Clock Class";
}

LRESULT MainWindow::handle_messages(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_CREATE) {
		if (FAILED(m_scene.Initialize()) or !initialize_timer()) {
			return -1;
		}
	}
	else if (uMsg == WM_PAINT or uMsg == WM_DISPLAYCHANGE) {
		PAINTSTRUCT	ps = { };
		BeginPaint(m_hwnd, &ps);
		m_scene.Render(m_hwnd);
		EndPaint(m_hwnd, &ps);
	}
	else if (uMsg == WM_SIZE) {
		m_scene.Resize(m_hwnd);
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
	else if (uMsg == WM_DESTROY) {
		CloseHandle(m_hTimer);
		m_scene.CleanUp();
		PostQuitMessage(0);
	}
	else if (uMsg == WM_ERASEBKGND) {
		return 1;
	}
	else {
		return DefWindowProcW(m_hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

BOOL MainWindow::initialize_timer() {
	m_hTimer = CreateWaitableTimerW(NULL, FALSE, NULL);
	if (!m_hTimer) {
		return FALSE;
	}

	LARGE_INTEGER li = { 0 };

	if (!SetWaitableTimer(m_hTimer, &li, (1000 / 60), NULL, NULL, FALSE)) {
		CloseHandle(m_hTimer);
		m_hTimer = NULL;
		return FALSE;
	}

	return TRUE;
}

void MainWindow::wait_timer() {
	if (MsgWaitForMultipleObjects(1, &m_hTimer, FALSE, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0) {
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}