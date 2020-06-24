#pragma once
#include<Windows.h>
#include<d2d1.h>
class GraphicsScene {
protected:
	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRenderTarget;

	float fScaleX, fScaleY;

	virtual HRESULT CreateDeviceIndependentResources() = 0;
	virtual void DiscardDeviceIndependentResources() = 0;
	virtual HRESULT CreateDeviceDependentResources() = 0;
	virtual void DiscardDeviceDependentResources() = 0;
	virtual void CalculateLayout() = 0;
	virtual void RenderScene() = 0;

	HRESULT CreateGraphicsResources(HWND hwnd) {
		HRESULT hr = S_OK;
		if (!pRenderTarget) {
			RECT rc;
			GetClientRect(hwnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
			hr = pFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(hwnd, size),
				&pRenderTarget
			);
		}
		if (SUCCEEDED(hr)) {
			hr = CreateDeviceDependentResources();
		}
		if (SUCCEEDED(hr)) {
			CalculateLayout();
		}
		return hr;
	}

	template<class T>
	T PixelToDipX(T pixels) const {
		return static_cast<T>(pixels / fScaleX);
	}

	template<class T>
	T PixelToDipY(T pixels) const {
		return static_cast<T>(pixels / fScaleY);
	}

	template<class T>
	void safe_release(T** ptr) {
		if (*ptr) {
			(*ptr)->Release();
			*ptr = NULL;
		}
	}

public:
	GraphicsScene() : pFactory(NULL), pRenderTarget(NULL), fScaleX(1.0f), fScaleY(1.0f) { }

	HRESULT Initialize() {
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
		if (SUCCEEDED(hr)) {
			CreateDeviceIndependentResources();
		}
		return hr;
	}

	void Render(HWND hwnd) {
		HRESULT hr = CreateGraphicsResources(hwnd);
		if (SUCCEEDED(hr)) {
			pRenderTarget->BeginDraw();
			RenderScene();
			hr = pRenderTarget->EndDraw();
		}
		if (hr == D2DERR_RECREATE_TARGET) {
			DiscardDeviceDependentResources();
			safe_release(&pRenderTarget);
		}
	}

	HRESULT Resize(HWND hwnd) {
		HRESULT hr = S_OK;
		if (pRenderTarget) {
			RECT rc;
			GetClientRect(hwnd, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
			hr = pRenderTarget->Resize(size);
			if (SUCCEEDED(hr)) {
				CalculateLayout();
			}
		}
		return hr;
	}

	void CleanUp() {
		DiscardDeviceDependentResources();
		DiscardDeviceIndependentResources();
		safe_release(&pRenderTarget);
		safe_release(&pFactory);
	}
};

