#pragma once
#include<Vector2.h>
#include<vector>

class CollisionObject {
public:
	CollisionObject(Vector2 rect);
	void* parent;
	void checkChanges();
	Vector2 rect;
	Vector2 position;
	Vector2 localPosition;
	float rotation = 0;
	float localRotation = 0;
	vector<CollisionObject*> checkForCollisions();
	bool isCollide(CollisionObject* col);
	bool havePoint(Vector2 point);
	~CollisionObject();
	void updatePoints();
	Vector2 oldRect;
	float oldRotation;
	Vector2 oldPosition;

	bool canCollide(CollisionObject* col);
	bool isCollideNoCheckChanges(CollisionObject* col);
	bool havePointNoCheckChanges(Vector2 point);
	float s;
	float c;

	float dividedRectx;
	float dividedRecty;

	Vector2 point1;
	Vector2 point2;
	Vector2 point3;
	Vector2 point4;

	float maxVertexDistance;
};
static vector<CollisionObject*> collisions;