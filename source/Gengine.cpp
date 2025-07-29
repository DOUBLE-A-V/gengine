#include <Gengine.h>

using GE = Gengine;

Gengine::ObjectPreset Gengine::ObjectType::Object = ObjectPreset("Object", "");
Gengine::ObjectPreset Gengine::ObjectType::Sprite = ObjectPreset("Sprite", "sprite");
Gengine::ObjectPreset Gengine::ObjectType::Text = ObjectPreset("Text", "text");

vector<GE::Object*>GE::objects;

GE::Sprite* Gengine::createSprite(string texturePath) {
	Sprite* sprite = new Sprite(texturePath.c_str());
	sprite->position = Vector2(0, 0);
	sprite->resize(sprite->texture->width, sprite->texture->height);
	Render::sprites.push_back(sprite);
	return sprite;
}
GE::Sprite* Gengine::createSprite(string texturePath, Vector2 pos) {
	Sprite* sprite = new Sprite(texturePath.c_str());
	sprite->position = Vector2(pos.x, pos.y);
	sprite->resize(sprite->texture->width, sprite->texture->height);
	Render::sprites.push_back(sprite);
	return sprite;
}
GE::Sprite* Gengine::createSprite() {
	Sprite* sprite = new Sprite();
	sprite->position = Vector2(0, 0);
	Render::sprites.push_back(sprite);
	return sprite;
}

void* Gengine::createModifierClass(string name) {
	if (name == "Sprite") {
		return createSprite();
	}
	else if (name == "Text") {
		return createText("hello world");
	}
	return NULL;
}
void* Gengine::Object::getModifier(string name) {
	for (Modifier* modifier : this->modifiers) {
		if (modifier->name == name) {
			return modifier->modifierClassPtr;
		}
	}
	return NULL;
}
void Gengine::Object::addModifier(string name, void* modifierClass) {
	Modifier* modifier = new Modifier();
	modifier->name = name;
	modifier->modifierClassPtr = modifierClass;
	this->modifiers.push_back(modifier);
}
void Gengine::Object::addModifier(string name) {
	Modifier* modifier = new Modifier();
	modifier->name = name;
	modifier->modifierClassPtr = createModifierClass(name);
	this->modifiers.push_back(modifier);
}
Gengine::Object::Object(string name, ObjectPreset type) {
	this->name = name;
	for (string modifier : type.modifiers) {
		this->addModifier(type.name, createModifierClass(type.name));
	}
}
Gengine::ObjectPreset::ObjectPreset(string name, string modifiers) {
	vector<string> separated;
	char* t = strtok((char*)modifiers.c_str(), ";");

	while (t != nullptr) {
		separated.push_back(t);
		t = strtok(nullptr, ";");
	}
	for (string modifier : separated) {
		this->modifiers.push_back(modifier);
	}
	this->name = name;
}

GE::Text* Gengine::createText(string text) {
	Text* textObj = Render::createText(text, 0, 0);
	return textObj;
}
GE::Text* Gengine::createText(string text, Vector2 pos) {
	Text* textObj = Render::createText(text, pos.x, pos.y);
	return textObj;
}

string Gengine::getCurrentDir() {
	char path[256];

	GetModuleFileNameA(NULL, path, 256);

	string result = path;
	result = result.substr(0, result.find_last_of("\\/"));

	return result;
}


GE::Texture* Gengine::loadTexture(const char* path) {
	return Render::loadTexture(path);
}

Gengine::Object* Gengine::createObject(string name, ObjectPreset type) {
	Object* obj = new Object(name, type);
	objects.push_back(obj);
	return obj;
}
bool GE::Object::hasModifier(string name) {
	for (Modifier* modifier : this->modifiers) {
		if (modifier->name == name) {
			return true;
		}
	}
	return false;
}

GE::Sprite* GE::Object::getSpriteModifier() {
	for (Modifier* modifier : this->modifiers) {
		if (modifier->name == "Sprite") {
			return static_cast<GE::Sprite*>(modifier->modifierClassPtr);
		}
	}
	return NULL;
}
GE::Text* GE::Object::getTextModifier() {
	for (Modifier* modifier : this->modifiers) {
		if (modifier->name == "Text") {
			return static_cast<GE::Text*>(modifier->modifierClassPtr);
		}
	}
	return NULL;
}

vector<GE::Object*> GE::findObjects(string name, bool recursive = false) {
	vector<GE::Object*> list;
	for (GE::Object* object : GE::objects) {
		if ((object->parent == NULL || recursive) && object->name == name) {
			list.push_back(object);
		}
	}
	return list;
}

vector<GE::Object*> GE::Object::findChilds(string name, bool recursive = false) {
	vector<GE::Object*> list;
	for (GE::Object* object : this->childs) {
		if (object->name == name) {
			list.push_back(object);
			if (recursive && object->childs.size() != 0) {
				for (GE::Object* obj2 : object->findChilds(name, true)) {
					list.push_back(obj2);
				}
			}
		}
	}
	return list;
}

GE::Object* GE::findFirstObject(string name, bool recursive = false) {
	for (GE::Object* object : GE::objects) {
		if ((!object->parent || recursive) && object->name == name) {
			return object;
		}
	}
	return NULL;
}
GE::Object* GE::Object::findFirstChild(string name, bool recursive = false) {
	for (GE::Object* object : this->childs) {
		if (object->name == name) {
			return object;
		}
		else if (recursive) {
			Object* tmp = object->findFirstChild(name, true);
			if (tmp) {
				return tmp;
			}
		}
	}
	return NULL;
}

void GE::Object::setParent(GE::Object* newParent) {
	this->parent = newParent;
	for (Object* child : parent->childs) {
		if (child == this) {
			return ;
		}
	}
	parent->childs.push_back(this);
}
void GE::Object::addChild(Object* child) {
	for (Object* obj : this->childs) {
		if (obj == child) {
			return ;
		}
	}
	this->childs.push_back(child);
	child->parent = this;
}