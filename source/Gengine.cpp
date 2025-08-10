#include <Gengine.h>

Gengine::ObjectPreset Gengine::ObjectType::Object = ObjectPreset("Object", "");
Gengine::ObjectPreset Gengine::ObjectType::Sprite = ObjectPreset("Sprite", "sprite");
Gengine::ObjectPreset Gengine::ObjectType::Text = ObjectPreset("Text", "text");

vector<Gengine::Object*>Gengine::objects;

bool Gengine::gengineInitialized = false;

Gengine::Object* Gengine::mainTree = new Gengine::Object("Main tree", Gengine::ObjectType::Object);

bool Gengine::running = false;
int Gengine::fixedUpdateDelta = 5;

void(*Gengine::fixedUpdateFunc)() = NULL;

const double pi = acos(-1);
const double radian = pi / 180.0;

bool Gengine::getKey(int key) {
	int keyState = glfwGetKey(Render::window, key);
	if (keyState == GLFW_PRESS) {
		return true;
	}
	else {
		return false;
	}
}
bool Gengine::getMouseButton(int key) {
	int keyState = glfwGetMouseButton(Render::window, key);
	if (keyState == GLFW_PRESS) {
		return true;
	}
	else {
		return false;
	}
}

void Gengine::setStartFunc(void (*func)()) {
	Window::startFunc = func;
}
void Gengine::setUpdateFunc(void (*func)(float)) {
	Window::updateFunc = func;
}
void Gengine::setFixedUpdateFunc(void (*func)()) {
	fixedUpdateFunc = func;
}

void Gengine::fixedUpdateThread() {
	while (Gengine::running) {
		this_thread::sleep_for(chrono::microseconds(fixedUpdateDelta));
		if (fixedUpdateFunc) {
			Gengine::fixedUpdateFunc();
		}
	}
}

Gengine::Sprite* Gengine::createSprite(string texturePath) {
	Sprite* sprite = new Sprite(texturePath.c_str());
	sprite->position = Vector2(0, 0);
	sprite->resize(sprite->texture->width, sprite->texture->height);
	Render::sprites.push_back(sprite);
	sprite->parent = mainTree;
	return sprite;
}

Gengine::Sprite* Gengine::createSprite(string texturePath, Vector2 pos) {
	Sprite* sprite = new Sprite(texturePath.c_str());
	sprite->position = Vector2(pos.x, pos.y);
	sprite->resize(sprite->texture->width, sprite->texture->height);
	Render::sprites.push_back(sprite);
	sprite->parent = mainTree;
	return sprite;
}

Gengine::Sprite* Gengine::createSprite() {
	Sprite* sprite = new Sprite();
	sprite->position = Vector2(0, 0);
	Render::sprites.push_back(sprite);
	sprite->parent = mainTree;
	return sprite;
}

void Gengine::updateAllPoses() {
	mainTree->realPosition = mainTree->position;
	mainTree->realRotation = mainTree->rotation;
	mainTree->updateAllPoses();
}

void Gengine::Object::updateAllPoses() {
	if (rotation != oldRotation || position.x != oldPosition.x || position.y != oldPosition.y) {
		transformChanged = true;
	}
	float s;
	float c;
	if (transformChanged) {
		oldRotation = rotation;
		oldPosition = position;
		s = sin(-realRotation * radian);
		c = cos(-realRotation * radian);
		realPosition.x = parent->realPosition.x + (position.x * c) + (position.y * s);
		realPosition.y = parent->realPosition.x + (position.x * s) + (position.y * c);
		realRotation = parent->realRotation + rotation;
	}

	for (Modifier* mod : modifiers) {
		if (mod->name == "Sprite") {
			Sprite* tmp = static_cast<Sprite*>(mod->modifierClassPtr);
			tmp->rotation = realRotation + tmp->localRotation;
			if (transformChanged) {
				tmp->position.x = realPosition.x + (tmp->localPosition.x * c) + (tmp->localPosition.y * s);
				tmp->position.y = realPosition.y + (tmp->localPosition.x * s) - (tmp->localPosition.y * c);
			}
		}
		else if (mod->name == "Text") {
			Text* tmp = static_cast<Text*>(mod->modifierClassPtr);
			tmp->position = realPosition + tmp->localPosition;
		}
		else if (mod->name == "Collision") {
			Collision* tmp = static_cast<Collision*>(mod->modifierClassPtr);
			tmp->position = realPosition + tmp->localPosition;
		}
	}
	for (Object* obj : childs) {
		obj->transformChanged = transformChanged;
		obj->updateAllPoses();
	}
	transformChanged = false;
}

void* Gengine::createModifierClass(string name, Object* parent) {
	if (name == "Sprite") {
		Sprite* tmp = createSprite();
		tmp->parent = parent;
		return tmp;
	}
	else if (name == "Text") {
		Text* tmp = createText("");
		tmp->parent = parent;
		return tmp;
	}
	else if (name == "Collision") {
		Collision* tmp = createCollision(Vector2(50, 50));
		tmp->parent = parent;
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
	modifier->modifierClassPtr = createModifierClass(name, this);
	this->modifiers.push_back(modifier);
}
Gengine::Object::Object(string name, ObjectPreset type) {
	this->name = name;
	this->position = Vector2(0, 0);
	this->realPosition = Vector2(0, 0);
	for (string modifier : type.modifiers) {
		this->addModifier(type.name, createModifierClass(type.name, this));
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
	textObj->parent = mainTree;
	return textObj;
}
Gengine::Text* Gengine::createText(string text, Vector2 pos) {
	Text* textObj = Render::createText(text, pos.x, pos.y);
	textObj->parent = mainTree;
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
	path = repairPath(path).c_str();
	return Render::loadTexture(path);
}
Gengine::Texture* Gengine::loadTexture(string path) {
	path = repairPath(path).c_str();
	return Render::loadTexture(path.c_str());
}

Gengine::Object* Gengine::createObject(string name, ObjectPreset type) {
	Object* obj = new Object(name, type);
	objects.push_back(obj);
	obj->setParent(mainTree);
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
	
	for (Object* obj : Gengine::objects) {
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

void Gengine::terminateFromWindow(GLFWwindow* window) {
	terminate();
}
void Gengine::terminate() {
	running = false;
	this_thread::sleep_for(chrono::milliseconds(100));
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
	glfwSetWindowCloseCallback(Render::window, Gengine::terminateFromWindow);
	Window::updateAllPoses = updateAllPoses;

	gengineInitialized = true;
	running = true;
	return 1;
}

void Gengine::startMainloop() {
	thread t(fixedUpdateThread);
	Window::mainloop();
}

Gengine::Object* Gengine::createSpriteObject(string name, string texturePath) {
	Object* obj = createObject(name, ObjectType::Sprite);
	texturePath = repairPath(texturePath);
	obj->getModifier<Sprite*>("Sprite")->setTexture(loadTexture(texturePath));
	return obj;
}
Gengine::Object* Gengine::createTextObject(string name, string text) {
	Object* obj = createObject(name, ObjectType::Text);
	obj->getModifier<Text*>("Text")->text = text;
	return obj;
}

float Gengine::getFPS() {
	return Window::fps;
}

string Gengine::repairPath(string path) {
	for (char ch : path) {
		if (ch == ':') {
			return path;
		}
	}
	string newPath = getCurrentDir();
	newPath += "/" + path;
	return newPath;
}
Collision* Gengine::createCollision(Vector2 rect) {
	//Collision* col = new Collision(rect);
	//col->parent = mainTree;
	return NULL;
}