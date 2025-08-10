#pragma once
#include<Vector2.h>
#include<vector>

class Collision {
public:
	Collision(Vector2 rect);
	void* parent;
	void checkChanges();
	Vector2 rect;
	Vector2 position;
	Vector2 localPosition;
	float rotation = 0;
	float localRotation = 0;
	vector<Collision*> checkForCollisions();
	bool isCollide(Collision* col);
	bool havePoint(Vector2 point);
	~Collision();
private:
	void updatePoints();
	Vector2 oldRect;
	float oldRotation;
	Vector2 oldPos;

	bool canCollide(Collision* col);
	bool isCollideNoCheckChanges(Collision* col);
	bool havePointNoCheckChanges(Vector2 point);
	float s;
	float c;
	float s1;
	float c1;

	float dividedRectx;
	float dividedRecty;

	Vector2 point1;
	Vector2 point2;
	Vector2 point3;
	Vector2 point4;

	float maxVertexDistance;
};
static vector<Collision*> collisions;