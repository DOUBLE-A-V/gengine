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

	operator string();
};