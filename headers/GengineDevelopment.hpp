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
}