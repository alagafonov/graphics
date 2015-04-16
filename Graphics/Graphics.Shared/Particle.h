#pragma once
class Particle
{
public:

	D2D1_VECTOR_2F position;
	D2D1_VECTOR_2F velocity;
	float angle;
	int TTL;

	Particle();
	~Particle();
	void Update();
	void Draw();
};

