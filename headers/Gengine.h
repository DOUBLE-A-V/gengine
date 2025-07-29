#pragma once
#include <Render.h>
#include <Window.h>
#include <Windows.h>

class Gengine
{
public:
	using Sprite = Render::Sprite;
	using Texture = Render::Texture;
	using Text = Render::Text;
	struct ObjectPreset {
		string name;
		vector<string> modifiers;
		ObjectPreset(string name, string modifiers);
	};
	struct ObjectType {
		static ObjectPreset Object;
		static ObjectPreset Sprite;
		static ObjectPreset Text;
	};

	class Modifier {
	public:
		string name;
		void* modifierClassPtr;
	};
	class Object {
	public:
		vector<Modifier*> modifiers;
		string name;
		vector<Object*> childs;
		Object* parent;
		Object(string name, ObjectPreset type);
		void addModifier(string name, void* modifierClass);
		void addModifier(string name);
		void* getModifier(string name);
		template<typename T>
		T* getModifier(string name) {
			for (Modifier* modifier : this->modifiers) {
				if (modifier->name == name) {
					return static_cast<T*>(modifier->modifierClassPtr);
				}
			}
		}
		Sprite* getSpriteModifier();
		Text* getTextModifier();
		bool hasModifier(string name);
		vector<Object*> findChilds(string name, bool recursive = false);
		Object* findFirstChild(string name, bool recursive = false);
		void setParent(Object* newParent);
		void addChild(Object* child);
	};
	static Object* findFirstObject(string name, bool recursive = false);
	static vector<Object*> findObjects(string name, bool recursive = false);

	static void* createModifierClass(string name);
	static Sprite* createSprite(string texturePath);
	static Sprite* createSprite();
	static Sprite* createSprite(string texturePath, Vector2 pos);

	static Text* createText(string text);
	static Text* createText(string text, Vector2 pos);

	static Object* createObject(string name, ObjectPreset type);

	static string getCurrentDir();

	static Texture* loadTexture(const char* path);

	static vector<Object*> objects;
};