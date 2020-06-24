#pragma once
#include "GraphicsScene.h"
#pragma comment(lib, "d2d1")
class Scene : public GraphicsScene{
	ID2D1SolidColorBrush* pFill, * pStroke;

	D2D1_ELLIPSE ellipse;
	D2D_POINT_2F ticks[24];

	HRESULT CreateDeviceIndependentResources() { return S_OK; }
	void DiscardDeviceIndependentResources() {}
	HRESULT CreateDeviceDependentResources();
	void DiscardDeviceDependentResources();
	void CalculateLayout();
	void RenderScene();

	void DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth);
public:
	Scene() : pFill(NULL), pStroke(NULL) {}
};

