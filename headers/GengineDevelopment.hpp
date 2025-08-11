#pragma once
#include <Gengine.h>

namespace GengineDevelopment {
	bool gengineInitialized = false;
	using Sprite = Gengine::Sprite*;
	using Text = Gengine::Text*;
	using Texture = Gengine::Texture*;
	using ObjectType = Gengine::ObjectType;
	using Modifier = Gengine::Modifier*;
	using Object = Gengine::Object*;
	using ObjectPreset = Gengine::ObjectPreset;
	using Font = Render::Font*;
	using Collision = CollisionObject*;

	static Object mainTree = Gengine::mainTree;

	template<typename T>
	static T(*createModifierClass)(string) { Gengine::createModifierClass };
	static Sprite(*createSprite)(string) { Gengine::createSprite };

	static Text(*createText)(string) { Gengine::createText };

	static Object(*createObject)(string, Gengine::ObjectPreset) { Gengine::createObject };
	static string(*getCurrentDir)() { Gengine::getCurrentDir };
	static Texture(*loadTexture)(string) { Gengine::loadTexture };
	static Font(*loadFont)(string, string) { Render::loadFont };

	static string(*visualizeObjectTree)(Object) { Gengine::visualizeObjectTree };

	static bool (*dirExists)(string path) { Gengine::dirExists };
	static bool (*fileExists)(string path) { Gengine::fileExists };

	static void (*exit)() { Gengine::terminate };

	static int(*gengineInit)(string, int, int) { Gengine::initialize };

	static void(*startMainloop)() { Gengine::startMainloop };

	static Object(*createSpriteObject)(string, string) { Gengine::createSpriteObject };
	static Object(*createTextObject)(string, string) { Gengine::createTextObject };

	static void(*setStartFunc)(void(*)()) { Gengine::setStartFunc };
	static void(*setUpdateFunc)(void(*)(float)) { Gengine::setUpdateFunc };
	static void(*setFixedUpdateFunc)(void(*)()) { Gengine::setFixedUpdateFunc };

	static bool(*getKey)(int) { Gengine::getKey };
	static bool(*getMouseButton)(int) { Gengine::getMouseButton };

	static float(*getFPS)() { Gengine::getFPS };

	static Vector2(*getMousePos)() { Gengine::getMousePos };
}