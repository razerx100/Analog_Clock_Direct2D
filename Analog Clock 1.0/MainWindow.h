#pragma once
#include "BaseWindow.h"
#include "Scene.h"
class MainWindow : public BaseWindow<MainWindow>{
	HANDLE m_hTimer;
	Scene m_scene;

	BOOL initialize_timer();

public:
	MainWindow() : m_hTimer(NULL) {}

	void wait_timer();

	PCWSTR class_name() const;
	LRESULT handle_messages(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

