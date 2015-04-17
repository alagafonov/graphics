#include "pch.h"
#include "Particle.h"


Particle::Particle()
{
}


Particle::~Particle()
{
}

void Particle::Update()
{
	TTL--;
	position.x += velocity.x;
	position.y += velocity.y;
}

void Particle::Draw()
{
	/*ComPtr<ID2D1SolidColorBrush> pBlackBrush;
	m_d2dContext->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Aqua),
		&pBlackBrush
		);

	m_d2dContext->DrawRectangle(
		D2D1::RectF(
		rc.left + 100.0f,
		rc.top + 100.0f,
		rc.right - 100.0f,
		rc.bottom - 100.0f),
		pBlackBrush);*/
}