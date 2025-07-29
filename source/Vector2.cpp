#include "Vector2.h"


Vector2::Vector2(float x, float y) {
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