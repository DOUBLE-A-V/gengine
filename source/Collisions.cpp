#include <Collisions.h>

const double pi = acos(-1);
const double radian = pi / 180.0;

Collision::Collision(Vector2 rect) {
	this->rect = rect;
	this->oldRect = rect;
	this->position = Vector2(0, 0);
	this->rotation = 0;
	this->oldRotation = 0;
	this->s = 0;
	this->c = 0;
	this->s1 = 0;
	this->c1 = 0;
	this->maxVertexDistance = sqrt(rect.x*rect.x + rect.y*rect.y)/2;

	updatePoints();

	collisions.push_back(this);
}

void Collision::updatePoints() {
	dividedRectx = rect.x / 2;
	dividedRecty = rect.y / 2;

	point1 = Vector2(position.x - (dividedRectx * c) - (dividedRecty * s), position.y - (dividedRectx * s) + (dividedRecty * c));
	point2 = Vector2(position.x + (dividedRectx * c) - (dividedRecty * s), position.y + (dividedRectx * s) + (dividedRecty * c));
	point3 = Vector2(position.x + (dividedRectx * c) + (dividedRecty * s), position.y + (dividedRectx * s) - (dividedRecty * c));
	point4 = Vector2(position.x - (dividedRectx * c) + (dividedRecty * s), position.y - (dividedRectx * s) - (dividedRecty * c));
}

Collision::~Collision() {
	int count = 0;
	for (Collision* col : collisions) {
		if (col == this) {
			collisions.erase(collisions.begin() + count);
			break;
		}
		count++;
	}
}

bool Collision::isCollide(Collision* col) {
	checkChanges();
	return isCollideNoCheckChanges(col);
}
bool Collision::havePoint(Vector2 point) {
	checkChanges();
	return havePointNoCheckChanges(point);
}

void Collision::checkChanges() {
	if (rotation != oldRotation) {
		oldRotation = rotation;
		this->s = sin(radian * rotation);
		this->c = cos(radian * rotation);
		this->s1 = sin(radian * -rotation);
		this->c1 = cos(radian * -rotation);
	}
	if (rect.x != oldRect.x || rect.y != oldRect.y) {
		this->maxVertexDistance = sqrt(rect.x * rect.x + rect.y * rect.y)/2;
		oldRect.x = rect.x;
		oldRect.y = rect.y;
		updatePoints();
	}
}

bool Collision::canCollide(Collision* col) {
	return this->maxVertexDistance + col->maxVertexDistance > this->position.distance(col->position);
}

vector<Collision*> Collision::checkForCollisions() {
	vector<Collision*> result;
	checkChanges();
	for (Collision* col : collisions) {
		if (col != this && canCollide(col)) {
			col->checkChanges();
			if (isCollideNoCheckChanges(col)) {
				result.push_back(col);
			}
		}
	}
	return result;
}

bool Collision::havePointNoCheckChanges(Vector2 point) {
	point.x -= position.x;
	point.y -= position.y;
	
	point = Vector2((point.x * c1) - (point.y * s1), (point.y * c1) + (point.x * s1));
	
	point.x += position.x;
	point.y += position.y;

	if ((point.x > position.x - dividedRectx && point.x < position.x + dividedRectx) &&
		(point.y > position.y - dividedRecty && point.y < position.y + dividedRecty)) {
		return true;
	}
	return false;
}

bool Collision::isCollideNoCheckChanges(Collision* col) {
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