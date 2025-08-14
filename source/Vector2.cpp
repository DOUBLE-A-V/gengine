#include "Vector2.h"


Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}
Vector2::Vector2(double x, double y) {
	this->x = x;
	this->y = y;
}
Vector2::Vector2() {
	this->x = 0;
	this->y = 0;
}
Vector2::operator string() {
	return "Vector2(" + to_string(x) + ", " + to_string(y) + ")";
}
Vector2::Vector2(int x, int y) {
	this->x = (float)x;
	this->y = (float)y;
}

Vector2 Vector2::operator/(float divideBy) {
	return Vector2(this->x / divideBy, this->y / divideBy);
}
Vector2 Vector2::operator*(float multiplier) {
	return Vector2(this->x * multiplier, this->y * multiplier);
}

float Vector2::distance(Vector2 pos) {
	float tmpx = pos.x - x;
	float tmpy = pos.y - y;

	return sqrt(abs(tmpx * tmpx + tmpy * tmpy));
}

Vector2 Vector2::operator+(Vector2 vec2) {
	return Vector2(x + vec2.x, y + vec2.y);
}
Vector2 Vector2::operator-(Vector2 vec2) {
	return Vector2(x - vec2.x, y - vec2.y);
}

Vector2 Vector2::operator+=(Vector2 vec2) {
	this->x += vec2.x;
	this->y += vec2.y;
	return *this;
}