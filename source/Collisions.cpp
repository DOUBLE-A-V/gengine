#include <Collisions.h>

const double pi = acos(-1);
const double radian = pi / 180.0;

vector<CollisionObject*> CollisionObject::allCollisions;

void CollisionObject::updatePoints() {
	dividedRectx = rect.x / 2;
	dividedRecty = rect.y / 2;

	point1 = Vector2(position.x - (dividedRectx * c) - (dividedRecty * s), position.y - (dividedRectx * s) + (dividedRecty * c));
	point2 = Vector2(position.x + (dividedRectx * c) - (dividedRecty * s), position.y + (dividedRectx * s) + (dividedRecty * c));
	point3 = Vector2(position.x + (dividedRectx * c) + (dividedRecty * s), position.y + (dividedRectx * s) - (dividedRecty * c));
	point4 = Vector2(position.x - (dividedRectx * c) + (dividedRecty * s), position.y - (dividedRectx * s) - (dividedRecty * c));
}
CollisionObject::CollisionObject(Vector2 rect) {
	rect = rect;
	this->rect = rect;
	this->oldRect = rect;
	this->position = Vector2(0, 0);
	this->rotation = 0;
	this->oldRotation = 0;
	this->s = 0;
	this->c = 0;
	this->maxVertexDistance = sqrt(rect.x*rect.x + rect.y*rect.y);

	this->updatePoints();

	allCollisions.push_back(this);
}

CollisionObject::~CollisionObject() {
	int count = 0;
	for (CollisionObject* col : allCollisions) {
		if (col == this) {
			allCollisions.erase(allCollisions.begin() + count);
			break;
		}
		count++;
	}
}

bool CollisionObject::isCollide(CollisionObject* col) {
	checkChanges();
	return isCollideNoCheckChanges(col);
}
bool CollisionObject::havePoint(Vector2 point) {
	checkChanges();
	return havePointNoCheckChanges(point);
}

void CollisionObject::checkChanges() {
	bool changed = false;
	if (rotation != oldRotation) {
		oldRotation = rotation;
		this->s = sin(radian * rotation);
		this->c = cos(radian * rotation);

		changed = true;
	}
	if (rect.x != oldRect.x || rect.y != oldRect.y) {
		this->maxVertexDistance = sqrt(rect.x * rect.x + rect.y * rect.y)/2;
		oldRect.x = rect.x;
		oldRect.y = rect.y;
		dividedRectx = rect.x / 2;
		dividedRecty = rect.y / 2;

		cout << dividedRectx << endl;

		changed = true;
	}
	if (position.x != oldPosition.x || position.y != oldPosition.y) {
		oldPosition = position;

		changed = true;
	}
	if (changed) { this->updatePoints(); }
}

bool CollisionObject::canCollide(CollisionObject* col) {
	return this->maxVertexDistance + col->maxVertexDistance > this->position.distance(col->position);
}

vector<CollisionObject*> CollisionObject::checkForCollisions() {
	vector<CollisionObject*> result;
	checkChanges();
	for (CollisionObject* col : allCollisions) {
		if (col != this && canCollide(col)) {
			col->checkChanges();
			if (isCollideNoCheckChanges(col)) {
				result.push_back(col);
			}
		}
	}
	return result;
}

bool CollisionObject::havePointNoCheckChanges(Vector2 point) {
	point.x -= position.x;
	point.y -= position.y;

	point = Vector2((point.x * c) - (point.y * s), (point.y * c) + (point.x * s));
	
	point.x += position.x;
	point.y += position.y;

	return ((point.x > position.x - dividedRectx && point.x < position.x + dividedRectx) &&
			(point.y > position.y - dividedRecty && point.y < position.y + dividedRecty));
}

bool CollisionObject::isCollideNoCheckChanges(CollisionObject* col) {
	if (havePointNoCheckChanges(col->point1)) { return true; };
	if (havePointNoCheckChanges(col->point2)) { return true; };
	if (havePointNoCheckChanges(col->point3)) { return true; };
	if (havePointNoCheckChanges(col->point4)) { return true; };

	if (col->havePointNoCheckChanges(point1)) { return true; };
	if (col->havePointNoCheckChanges(point2)) { return true; };
	if (col->havePointNoCheckChanges(point3)) { return true; };
	if (col->havePointNoCheckChanges(point4)) { return true; };

	return false;
}