#pragma once
#include <Gengine.h>

namespace GengineDevelopment {
	using Sprite = Gengine::Sprite*;
	using Text = Gengine::Text*;
	using Texture = Gengine::Texture*;
	using ObjectType = Gengine::ObjectType;
	using Modifier = Gengine::Modifier*;
	using Object = Gengine::Object*;
	using ObjectPreset = Gengine::ObjectPreset;
	using Font = Render::Font*;

	static Object(*findFirstObject)(string, bool) { Gengine::findFirstObject };
	static vector<Object>(*findObjects)(string, bool) { Gengine::findObjects };
	template<typename T>
	static T(*createModifierClass)(string) { Gengine::createModifierClass };
	static Sprite(*createSprite)(string) { Gengine::createSprite };

	static Text(*createText)(string) { Gengine::createText };

	static Object(*createObject)(string, Gengine::ObjectPreset) { Gengine::createObject };
	static string(*getCurrentDir)() { Gengine::getCurrentDir };
	static Texture(*loadTexture)(string) { Gengine::loadTexture };
	static Font(*loadFont)(string, string) { Render::loadFont };
}