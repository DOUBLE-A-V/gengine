#pragma once
#include <string>
#include <iostream>
using namespace std;
class Vector2 {
public:
	float x;
	float y;
	Vector2();
	Vector2(int x, int y);
	Vector2(float x, float y);
	Vector2(double x, double y);

	Vector2 operator/(float divideBy);
	Vector2 operator*(float multiplier);

	Vector2 operator+(Vector2 vec2);
	Vector2 operator-(Vector2 vec2);

	Vector2 operator+=(Vector2 vec2);

	float distance(Vector2 pos);

	operator string();
};