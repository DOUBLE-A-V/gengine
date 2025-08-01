#pragma once
#include <Render.h>
#include <Window.h>
#include <Windows.h>
#include <thread>

//all keys define
//chars keys
#define KEY_SPACE              32
#define KEY_APOSTROPHE         39
#define KEY_COMMA              44
#define KEY_MINUS              45
#define KEY_PERIOD             46
#define KEY_SLASH              47
#define KEY_0                  48
#define KEY_1                  49
#define KEY_2                  50
#define KEY_3                  51
#define KEY_4                  52
#define KEY_5                  53
#define KEY_6                  54
#define KEY_7                  55
#define KEY_8                  56
#define KEY_9                  57
#define KEY_SEMICOLON          59
#define KEY_EQUAL              61
#define KEY_A                  65
#define KEY_B                  66
#define KEY_C                  67
#define KEY_D                  68
#define KEY_E                  69
#define KEY_F                  70
#define KEY_G                  71
#define KEY_H                  72
#define KEY_I                  73
#define KEY_J                  74
#define KEY_K                  75
#define KEY_L                  76
#define KEY_M                  77
#define KEY_N                  78
#define KEY_O                  79
#define KEY_P                  80
#define KEY_Q                  81
#define KEY_R                  82
#define KEY_S                  83
#define KEY_T                  84
#define KEY_U                  85
#define KEY_V                  86
#define KEY_W                  87
#define KEY_X                  88
#define KEY_Y                  89
#define KEY_Z                  90
#define KEY_LEFT_BRACKET       91
#define KEY_BACKSLASH          92
#define KEY_RIGHT_BRACKET      93
#define KEY_GRAVE_ACCENT       96
#define KEY_WORLD_1            161
#define KEY_WORLD_2            162

//function keys
#define KEY_ESCAPE             256
#define KEY_ENTER              257
#define KEY_TAB                258
#define KEY_BACKSPACE          259
#define KEY_INSERT             260
#define KEY_DELETE             261
#define KEY_RIGHT              262
#define KEY_LEFT               263
#define KEY_DOWN               264
#define KEY_UP                 265
#define KEY_PAGE_UP            266
#define KEY_PAGE_DOWN          267
#define KEY_HOME               268
#define KEY_END                269
#define KEY_CAPS_LOCK          280
#define KEY_SCROLL_LOCK        281
#define KEY_NUM_LOCK           282
#define KEY_PRINT_SCREEN       283
#define KEY_PAUSE              284
#define KEY_F1                 290
#define KEY_F2                 291
#define KEY_F3                 292
#define KEY_F4                 293
#define KEY_F5                 294
#define KEY_F6                 295
#define KEY_F7                 296
#define KEY_F8                 297
#define KEY_F9                 298
#define KEY_F10                299
#define KEY_F11                300
#define KEY_F12                301
#define KEY_F13                302
#define KEY_F14                303
#define KEY_F15                304
#define KEY_F16                305
#define KEY_F17                306
#define KEY_F18                307
#define KEY_F19                308
#define KEY_F20                309
#define KEY_F21                310
#define KEY_F22                311
#define KEY_F23                312
#define KEY_F24                313
#define KEY_F25                314
#define KEY_KP_0               320
#define KEY_KP_1               321
#define KEY_KP_2               322
#define KEY_KP_3               323
#define KEY_KP_4               324
#define KEY_KP_5               325
#define KEY_KP_6               326
#define KEY_KP_7               327
#define KEY_KP_8               328
#define KEY_KP_9               329
#define KEY_KP_DECIMAL         330
#define KEY_KP_DIVIDE          331
#define KEY_KP_MULTIPLY        332
#define KEY_KP_SUBTRACT        333
#define KEY_KP_ADD             334
#define KEY_KP_ENTER           335
#define KEY_KP_EQUAL           336
#define KEY_LEFT_SHIFT         340
#define KEY_LEFT_CONTROL       341
#define KEY_LEFT_ALT           342
#define KEY_LEFT_SUPER         343
#define KEY_RIGHT_SHIFT        344
#define KEY_RIGHT_CONTROL      345
#define KEY_RIGHT_ALT          346
#define KEY_RIGHT_SUPER        347
#define KEY_MENU               348
#define KEY_LAST               KEY_MENU
#define MOD_SHIFT           0x0001
#define MOD_CONTROL         0x0002
#define MOD_ALT             0x0004
#define MOD_SUPER           0x0008
#define MOD_CAPS_LOCK       0x0010
#define MOD_NUM_LOCK        0x0020

//mouse
#define MOUSE_BUTTON_1         0
#define MOUSE_BUTTON_2         1
#define MOUSE_BUTTON_3         2
#define MOUSE_BUTTON_4         3
#define MOUSE_BUTTON_5         4
#define MOUSE_BUTTON_6         5
#define MOUSE_BUTTON_7         6
#define MOUSE_BUTTON_8         7
#define MOUSE_BUTTON_LAST      MOUSE_BUTTON_8
#define MOUSE_BUTTON_LEFT      MOUSE_BUTTON_1
#define MOUSE_BUTTON_RIGHT     MOUSE_BUTTON_2
#define MOUSE_BUTTON_MIDDLE    MOUSE_BUTTON_3

//joystick
#define JOYSTICK_1             0
#define JOYSTICK_2             1
#define JOYSTICK_3             2
#define JOYSTICK_4             3
#define JOYSTICK_5             4
#define JOYSTICK_6             5
#define JOYSTICK_7             6
#define JOYSTICK_8             7
#define JOYSTICK_9             8
#define JOYSTICK_10            9
#define JOYSTICK_11            10
#define JOYSTICK_12            11
#define JOYSTICK_13            12
#define JOYSTICK_14            13
#define JOYSTICK_15            14
#define JOYSTICK_16            15
#define JOYSTICK_LAST          JOYSTICK_16

//gamepad
#define GAMEPAD_BUTTON_A               0
#define GAMEPAD_BUTTON_B               1
#define GAMEPAD_BUTTON_X               2
#define GAMEPAD_BUTTON_Y               3
#define GAMEPAD_BUTTON_LEFT_BUMPER     4
#define GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define GAMEPAD_BUTTON_BACK            6
#define GAMEPAD_BUTTON_START           7
#define GAMEPAD_BUTTON_GUIDE           8
#define GAMEPAD_BUTTON_LEFT_THUMB      9
#define GAMEPAD_BUTTON_RIGHT_THUMB     10
#define GAMEPAD_BUTTON_DPAD_UP         11
#define GAMEPAD_BUTTON_DPAD_RIGHT      12
#define GAMEPAD_BUTTON_DPAD_DOWN       13
#define GAMEPAD_BUTTON_DPAD_LEFT       14
#define GAMEPAD_BUTTON_LAST            GAMEPAD_BUTTON_DPAD_LEFT
#define GAMEPAD_BUTTON_CROSS       GAMEPAD_BUTTON_A
#define GAMEPAD_BUTTON_CIRCLE      GAMEPAD_BUTTON_B
#define GAMEPAD_BUTTON_SQUARE      GAMEPAD_BUTTON_X
#define GAMEPAD_BUTTON_TRIANGLE    GAMEPAD_BUTTON_Y
#define GAMEPAD_AXIS_LEFT_X        0
#define GAMEPAD_AXIS_LEFT_Y        1
#define GAMEPAD_AXIS_RIGHT_X       2
#define GAMEPAD_AXIS_RIGHT_Y       3
#define GAMEPAD_AXIS_LEFT_TRIGGER  4
#define GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define GAMEPAD_AXIS_LAST          GAMEPAD_AXIS_RIGHT_TRIGGER

class Gengine
{
public:
	static int fixedUpdateDelta;
	static void (*startFunc)();
	static void (*updateFunc)(float);
	static void (*fixedUpdateFunc)();
	static bool running;
	static thread* renderThread;
	static void fixedUpdateThread();
	static bool gengineInitialized;
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
		Object* parent = mainTree;
		Object(string name, ObjectPreset type);
		void addModifier(string name, void* modifierClass);
		void addModifier(string name);
		void* getModifier(string name);
		template<typename T>
		T getModifier(string name) {
			for (Modifier* modifier : this->modifiers) {
				if (modifier->name == name) {
					return static_cast<T>(modifier->modifierClassPtr);
				}
			}
			return NULL;
		}
		Sprite* getSpriteModifier();
		Text* getTextModifier();
		bool hasModifier(string name);
		vector<Object*> findChilds(string name, bool recursive = false);
		Object* findFirstChild(string name, bool recursive = false);
		void setParent(Object* newParent);
		void addChild(Object* child);
	};

	static void* createModifierClass(string name);
	template<typename T>
	static T createModifierClass(string name) {
		return static_cast<T>(createModifierClass(name));
	}
	static Sprite* createSprite(string texturePath);
	static Sprite* createSprite();
	static Sprite* createSprite(string texturePath, Vector2 pos);

	static Text* createText(string text);
	static Text* createText(string text, Vector2 pos);

	static Object* createObject(string name, ObjectPreset type);

	static string getCurrentDir();

	static Texture* loadTexture(const char* path);
	static Texture* loadTexture(string path);

	static vector<Object*> objects;

	static string visualizeObjectTree(Object* parent);
	static string visualizeObjectTreeByDeep(Object* parent, int deep = -1);

	static Object* mainTree;

	static void terminate();
	static void terminateFromWindow(GLFWwindow* window);
	static int initialize(string windowTitle, int windowWidth, int windowHeight);

	static bool dirExists(string path);
	static bool fileExists(string path);

	static void startMainloop();

	static Object* createSpriteObject(string name, string texturePath);
	static Object* createTextObject(string name, string text);

	static void setStartFunc(void (*func)());
	static void setUpdateFunc(void (*func)(float));
	static void setFixedUpdateFunc(void (*func)());

	static bool getKey(int keyCode);
	static bool getMouseButton(int keyCode);

	static float getFPS();
};