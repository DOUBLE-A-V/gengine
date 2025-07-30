#include <Gengine.h>

Gengine::ObjectPreset Gengine::ObjectType::Object = ObjectPreset("Object", "");
Gengine::ObjectPreset Gengine::ObjectType::Sprite = ObjectPreset("Sprite", "sprite");
Gengine::ObjectPreset Gengine::ObjectType::Text = ObjectPreset("Text", "text");

vector<Gengine::Object*>Gengine::objects;

bool Gengine::gengineInitialized = false;

Gengine::Object* Gengine::mainTree = new Gengine::Object("Main tree", Gengine::ObjectType::Object);

thread* Gengine::fixedUpdateThread;
bool Gengine::running = false;
Gengine::Sprite* Gengine::createSprite(string texturePath) {
	Sprite* sprite = new Sprite(texturePath.c_str());
	sprite->position = Vector2(0, 0);
	sprite->resize(sprite->texture->width, sprite->texture->height);
	Render::sprites.push_back(sprite);
	return sprite;
}

Gengine::Sprite* Gengine::createSprite(string texturePath, Vector2 pos) {
	Sprite* sprite = new Sprite(texturePath.c_str());
	sprite->position = Vector2(pos.x, pos.y);
	sprite->resize(sprite->texture->width, sprite->texture->height);
	Render::sprites.push_back(sprite);
	return sprite;
}

Gengine::Sprite* Gengine::createSprite() {
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
bool Gengine::dirExists(std::string dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}
bool Gengine::fileExists(std::string fileName)
{
	DWORD ftyp = GetFileAttributesA(fileName.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return false;
	return true;
}
Gengine::Text* Gengine::createText(string text) {
	Text* textObj = Render::createText(text, 0, 0);
	return textObj;
}
Gengine::Text* Gengine::createText(string text, Vector2 pos) {
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
Gengine::Texture* Gengine::loadTexture(const char* path) {
	return Render::loadTexture(path);
}
Gengine::Texture* Gengine::loadTexture(string path) {
	return Render::loadTexture(path.c_str());
}

Gengine::Object* Gengine::createObject(string name, ObjectPreset type) {
	Object* obj = new Object(name, type);
	objects.push_back(obj);
	return obj;
}
bool Gengine::Object::hasModifier(string name) {
	for (Modifier* modifier : this->modifiers) {
		if (modifier->name == name) {
			return true;
		}
	}
	return false;
}

Gengine::Sprite* Gengine::Object::getSpriteModifier() {
	for (Modifier* modifier : this->modifiers) {
		if (modifier->name == "Sprite") {
			return static_cast<Gengine::Sprite*>(modifier->modifierClassPtr);
		}
	}
	return NULL;
}
Gengine::Text* Gengine::Object::getTextModifier() {
	for (Modifier* modifier : this->modifiers) {
		if (modifier->name == "Text") {
			return static_cast<Gengine::Text*>(modifier->modifierClassPtr);
		}
	}
	return NULL;
}

vector<Gengine::Object*> Gengine::Object::findChilds(string name, bool recursive) {
	vector<Gengine::Object*> list;
	for (Gengine::Object* object : this->childs) {
		if (object->name == name) {
			list.push_back(object);
			if (recursive && object->childs.size() != 0) {
				for (Gengine::Object* obj2 : object->findChilds(name, true)) {
					list.push_back(obj2);
				}
			}
		}
	}
	return list;
}
Gengine::Object* Gengine::Object::findFirstChild(string name, bool recursive) {
	for (Gengine::Object* object : this->childs) {
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

void Gengine::Object::setParent(Gengine::Object* newParent) {
	int count = 0;
	for (Object* obj : parent->childs) {
		if (obj == this)
		{
			parent->childs.erase(parent->childs.begin() + count);
			break;
		}
		count++;
	}
	if (parent) {

	}
	this->parent = newParent;
	for (Object* child : parent->childs) {
		if (child == this) {
			return ;
		}
	}
	parent->childs.push_back(this);
}
void Gengine::Object::addChild(Object* child) {
	for (Object* obj : this->childs) {
		if (obj == child) {
			return ;
		}
	}
	this->childs.push_back(child);
	child->parent = this;
}

string Gengine::visualizeObjectTreeByDeep(Object* parent, int deep) {
	vector<Object*> lst;
	string text;
	deep++;
	for (int i = deep; i--; i > 0) {
		text += "    ";
	}

	text += "|-" + parent->name;
	text += "\n";
	for (int i = deep; i--; i > 0) {
		text += "    ";
	}
	text += " ---";
	text += "\n";
	
	cout << "start" << endl;
	for (Object* obj : Gengine::objects) {
		cout << obj->parent << endl;
		if (obj->parent == parent) {
			lst.push_back(obj);
		}
	}
	for (Object* obj : lst) {
		text += visualizeObjectTreeByDeep(obj, deep);
	}
	return text;
}
string Gengine::visualizeObjectTree(Object* parent) {
	return visualizeObjectTreeByDeep(parent);
}

void Gengine::terminate() {
	running = false;
	Render::terminate();
	for (Object* obj : Gengine::objects) {
		delete obj;
	}
	delete mainTree;
}
int Gengine::initialize(string windowTitle, int windowWidth, int windowHeight) {
	int windowInit = Window::init(windowTitle.c_str(), windowWidth, windowHeight);
	if (windowInit != 0) {
		Logger::error("window init failed", "Gengine::initialize");
		return -1;
	}
	if (!Render::loadFont(getCurrentDir() + "/gengine/default_font.davf", "default")) {
		Logger::error("default font loading failed", "Gengine::initialize");
		return -2;
	}
	gengineInitialized = true;
	running = true;
	return 1;
}

void Gengine::startMainloop() {
	Window::mainloop();
}