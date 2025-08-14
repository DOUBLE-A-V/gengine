#include <Tweens.h>

bool Tweens::tweensHostWorking = false;
chrono::system_clock::time_point Tweens::prevTweensUpdateTime = chrono::system_clock::now();

vector<Tweens::Tween*> Tweens::tweens;
vector<thread*> Tweens::onCompleteThreads;


Tweens::Tween::~Tween() {
	int num = 0;
	for (Tween* tween : tweens) {
		if (tween == this) {
			tweens.erase(tweens.begin() + num);
			break;
		}
		num++;
	}
}


void Tweens::Tween::start(float duration, bool joined) {
	this->paused = false;
	this->time = duration;
	this->duration = duration;
	this->joined = joined;

	if (this->typeToTween == TypeInt) {
		this->deltaValueInt = this->targetValueInt - *(static_cast<int*>(this->target));
	} else if (this->typeToTween == TypeFloat) {
		this->deltaValueFloat = this->targetValueFloat - *(static_cast<float*>(this->target));
	} else if (this->typeToTween == TypeVector2) {
		this->deltaValueVector2 = this->targetValueVector2 - *(static_cast<Vector2*>(this->target));
	}
}
void Tweens::Tween::pause() {
	this->paused = true;
}
void Tweens::Tween::stop() {
	this->paused = true;
	if (this->destroyAfterComplete) {
		delete this;
	}
}

Tweens::Tween::Tween(void* target, int targetValue, Easing easing) {
	this->target = target;
	this->targetValueInt = targetValue;
	this->typeToTween = TypeTween::TypeInt;
	this->easing = easing;
	Tweens::tweens.push_back(this);
}
Tweens::Tween::Tween(void* target, float targetValue, Easing easing) {
	this->target = target;
	this->targetValueFloat = targetValue;
	this->typeToTween = TypeTween::TypeFloat;
	this->easing = easing;
	Tweens::tweens.push_back(this);
}
Tweens::Tween::Tween(void* target, Vector2 targetValue, Easing easing) {
	this->target = target;
	this->targetValueVector2 = targetValue;
	this->typeToTween = TypeTween::TypeVector2;
	this->easing = easing;
	Tweens::tweens.push_back(this);
}
void Tweens::tweensHost() {
	while (Tweens::tweensHostWorking) {
		double deltaTime = (chrono::system_clock::now() - prevTweensUpdateTime).count() / 10000000.0;
		prevTweensUpdateTime = chrono::system_clock::now();
		this_thread::sleep_for(chrono::milliseconds(5));
		for (Tween* tween : tweens) {
			if (!tween->paused) {
				tween->time -= deltaTime;
				if (tween->time <= 0) {
					if (tween->typeToTween == TypeInt) {
						*(static_cast<int*>(tween->target)) = tween->targetValueInt;
					}
					else if (tween->typeToTween == TypeFloat) {
						*(static_cast<float*>(tween->target)) = tween->targetValueFloat;
					}
					else if (tween->typeToTween == TypeVector2) {
						*(static_cast<Vector2*>(tween->target)) = tween->targetValueVector2;
					}
					if (tween->onCompleteFunc) {
						thread* t = new thread(tween->onCompleteFunc);
						onCompleteThreads.push_back(t);
					}
					tween->stop();
				}
				else {
					if (tween->easing == Easing::None) {
						double durationRatio = tween->duration / deltaTime;
						if (tween->typeToTween == TypeInt) {
							int* ptr = static_cast<int*>(tween->target);
							*ptr += tween->deltaValueInt / (durationRatio);
						} else if (tween->typeToTween == TypeFloat) {
							float* ptr = static_cast<float*>(tween->target);
							*ptr += tween->deltaValueFloat / (durationRatio);
						}
						else if (tween->typeToTween == TypeVector2) {
							Vector2* ptr = static_cast<Vector2*>(tween->target);
							*ptr += tween->deltaValueVector2 / (durationRatio);
						}
					}
				}
			}
		}
	}
}