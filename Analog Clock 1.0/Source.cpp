#include "MainWindow.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PWSTR pCmdLine, int nCmdShow) {
	MainWindow win;
	win.create(L"Analog clock 1.0", WS_OVERLAPPEDWINDOW, 480, 480);
	ShowWindow(win.window(), nCmdShow);

	MSG msg = { };
	while (msg.message != WM_QUIT) {
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else {
			win.wait_timer();
		}
	}
	return 0;
}