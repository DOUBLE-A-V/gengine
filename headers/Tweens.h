#pragma once
#include <Vector2.h>
#include <vector>
#include <chrono>
#include <thread>

class Tweens {
public:
	static chrono::system_clock::time_point prevTweensUpdateTime;
	static bool tweensHostWorking;
	enum Easing {
		None = 0,
		In = 1,
		Out = 2,
		InOut = 3
	};
	enum TypeTween {
		TypeInt = 0,
		TypeFloat = 1,
		TypeVector2 = 2
	};
	class Tween {
	public:
		void* target;
		int targetValueInt;
		float targetValueFloat;
		Vector2 targetValueVector2;

		int deltaValueInt;
		float deltaValueFloat;
		Vector2 deltaValueVector2;
		
		bool destroyAfterComplete = true;
		bool paused = true;

		bool joined = false;

		float time = 0;
		float duration = 0;
		Easing easing = Easing::None;
		TypeTween typeToTween = TypeTween::TypeInt;

		void(*onCompleteFunc)() = NULL;
		void(*onStepFunc)() = NULL;

		~Tween();

		void start(float duration, bool joined = false);
		void stop();
		void pause();

		Tween(void* target, int targetValue, Easing easing = Easing::None);
		Tween(void* target, float targetValue, Easing easing = Easing::None);
		Tween(void* target, Vector2 targetValue, Easing easing = Easing::None);
	};
	static vector<Tween*> tweens;
	static vector<thread*> onCompleteThreads;
	static void tweensHost();
};