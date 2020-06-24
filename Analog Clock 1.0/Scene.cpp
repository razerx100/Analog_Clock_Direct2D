#include "Scene.h"

HRESULT Scene::CreateDeviceDependentResources() {
	HRESULT hr = pRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::WhiteSmoke),
		D2D1::BrushProperties(),
		&pFill
	);
	if (SUCCEEDED(hr)) {
		hr = pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black),
			D2D1::BrushProperties(),
			&pStroke
		);
	}
	return hr;
}

void Scene::DrawClockHand(float fHandLength, float fAngle, float fStrokeWidth) {
	pRenderTarget->SetTransform(
		D2D1::Matrix3x2F::Rotation(fAngle, ellipse.point)
	);

	D2D_POINT_2F endPoint = D2D1::Point2F(
		ellipse.point.x,
		ellipse.point.y - (ellipse.radiusY * fHandLength)
	);

	pRenderTarget->DrawLine(
		ellipse.point, endPoint, pStroke, fStrokeWidth
	);
}

void Scene::RenderScene() {
	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	pRenderTarget->FillEllipse(ellipse, pFill);
	pRenderTarget->DrawEllipse(ellipse, pStroke);

	for (DWORD i = 0; i < 12; i++) {
		pRenderTarget->DrawLine(ticks[i * 2], ticks[i * 2 + 1], pStroke, 2.0f);
	}
	SYSTEMTIME time;
	GetLocalTime(&time);

	const float fHourAngle = (360.0f / 12) * (time.wHour) + (time.wMinute * 0.5f);
	const float fMinuteAngle = (360.f / 60) * (time.wMinute);
	const float fSecondAngle = (360.0f / 60) * (time.wSecond) + (360.0f / 60'000) * (time.wMilliseconds);

	DrawClockHand(0.6f, fHourAngle, 6);
	DrawClockHand(0.85f, fMinuteAngle, 4);
	DrawClockHand(0.85f, fSecondAngle, 1);

	pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void Scene::CalculateLayout() {
	D2D1_SIZE_F fSize = pRenderTarget->GetSize();

	const float x = fSize.height / 2.0f;
	const float y = fSize.width / 2.0f;

	const float radius = min(x, y);
	ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);

	D2D_POINT_2F pt1 = D2D1::Point2F(
		ellipse.point.x,
		ellipse.point.y - (ellipse.radiusY * 0.9f)
	);

	D2D_POINT_2F pt2 = D2D1::Point2F(
		ellipse.point.x,
		ellipse.point.y - ellipse.radiusY
	);

	for (DWORD i = 0; i < 12; i++) {
		D2D1::Matrix3x2F mat = D2D1::Matrix3x2F::Rotation(
			(360.0f / 12) * i, ellipse.point
		);

		ticks[i * 2] = mat.TransformPoint(pt1);
		ticks[i * 2 + 1] = mat.TransformPoint(pt2);
	}
}

void Scene::DiscardDeviceDependentResources() {
	safe_release(&pFill);
	safe_release(&pStroke);
}
