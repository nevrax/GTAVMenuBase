#pragma once

#include <string>
#include <windows.h>
#include <vector>
#include <functional>
#include <array>
#include <unordered_map>
#include <map>

#include "menucontrols.h"
#include "menuutils.h"
#include "menusettings.h"

namespace NativeMenu {
class Menu {
public:
	/*
	 * c'tor and d'tor. As of current they don't do a lot.
	 */
	Menu();
	~Menu();

	/*
	 * Specify settings file name/location. If not set, it will use default settings.
	 * Calling this is pretty much mandatory. It's advised to call this just once after
	 * figuring out the directory structure.
	 */
	void SetFiles(const std::string & fileName);

	/*
	 * Read settings file. If no specified settings file, it'll use default settings.
	 * Calling this is pretty much mandatory. It's advised to call this whenever new settings
	 * need to be parsed, for example, on re-opening the menu or something.
	 */
	void ReadSettings();

	/*
	 * Registers a function that will be called when the menu is opened.
	 */
	void RegisterOnMain(std::function<void() > onMain);

	/*
	 * Registers a function that will be called when the menu is closed.
	 */
	void RegisterOnExit(std::function<void() > onExit);

	/*
	 * Main menu is always called "mainmenu".
	 * Otherwise menuname is a submenu name.
	 * Returns true when inside the submenu menuname.
	 */
	bool CurrentMenu(std::string menuname);

	/*
	 * Always assign a title to a submenu!
	 */
	void Title(std::string title);
	void Title(std::string title, float customSize);
	void Title(std::string title, std::string dict, std::string texture);
	void Title(std::string title, std::string dict, std::string texture, float customSize);

	/*
	 * Custom title textures have a resolution of 512x128, but any other
	 * resolution with the same aspect ratio should work (4:1)
	 */
	void Title(std::string title, int textureHandle);
	void Title(std::string title, int textureHandle, float customSize);

	/*
	 * A menu subtitle. This is optional. If added, this must be added directly
	 * below the title, before any options are specified.
	 */
	void Subtitle(std::string subtitle);

	/*
	 * Optional: Specify a different background texture for the footer.
	 * If not used, solid black is used by default.
	 */
	void Footer(Color color);
	void Footer(std::string dict, std::string texture);

	/*
	 * Normal option.
	 * Shows nothing special.
	 * Returns true on accept.
	 */
	bool Option(std::string option, std::vector<std::string> details = {});
    bool Option(std::string option, Color highlight, std::vector<std::string> details = {});

	/*
	 * Submenu option.
	 * Shows option with menu sign.
	 * Switches menu on action.
	 * Returns true on accept.
	 */
	bool MenuOption(std::string option, std::string menu, std::vector<std::string> details = {});

	/*
	 * Option that shows an extra pane to the right.
	 * Shows text with extra's specifyable in the detail pane.
	 * Function pointers can be passed and are called on right, left press.
	 * Custom pane title can be specified.
	 * Extra draws external image if a line is "!IMG:<file handle>W<height>H<width>".
	 * Extra draws sprite if a line is "!SPR:D<dict>N<name>W<height>H<width>".
	 * Extra can be left empty.
	 * "highlighted" indicates the highlight status of the option. It can be a nullptr.
	 * Returns true on accept.
	 */
	bool OptionPlus(std::string option, std::vector<std::string> &extra, bool *highlighted = nullptr, 
					std::function<void()> onRight = nullptr, std::function<void()> onLeft = nullptr ,
					std::string title = "Info", std::vector<std::string> details = {});

	/*
	 * Can be used to draw the OptionPlus extras separately. Useful when generating
	 * the extra text is costful or if no other things are needed. Running this when
	 * OptionPlus is highlighted can improve performance.
	 */
	void OptionPlusPlus(std::vector<std::string> &extra, std::string title = "Info");

	/*
	 * Option that changes an int value with optional custom-sized steps.
	 * Shows option with the value inside < > brackets.
	 * Returns true on accept, left and right.
	 */
	bool IntOption(		std::string option, int &var,	int min,	int max,	int step = 1,		std::vector<std::string> details = {});

	/*
	 * Option that changes a float value with optional custom-sized steps.
	 * Shows option with the value inside < > brackets.
	 * Returns true on accept, left and right.
	 */
	bool FloatOption(	std::string option, float &var, float min,	float max,	float step = 0.1f,	std::vector<std::string> details = {});

	/*
	 * Option that toggles a boolean.
	 * Shows option with a checkbox, which is checked when the passed var is "true".
	 * Returns true on accept.
	 */
	bool BoolOption(std::string option, bool &var, std::vector<std::string> details = {});

	/*
	 * Option that displays a boolean with a specifyable texture.
	 * Shows option with a checkbox, which is checked when the passed var is "true".
	 * Returns true on accept.
	 */
	bool BoolSpriteOption(std::string option, bool var, std::string category, std::string spriteOn, std::string spriteOff, std::vector<std::string> details = {});

	/*
	 * Option that shows a scrollable list of supplied ints.
	 * Shows option with the current value inside < > brackets.
	 * Value shown is display[iterator].
	 * On left or right press, iterator's value is incremented or decremented.
	 * Returns true on accept, left and right.
	 */
	bool IntArray(std::string option, std::vector<int> display, int &iterator, std::vector<std::string> details = {});

	/*
	 * Option that shows a scrollable list of supplied floats.
	 * Shows option with the current value inside < > brackets.
	 * Value shown is display[iterator].
	 * On left or right press, iterator's value is incremented or decremented.
	 * Returns true on accept, left and right.
	 */
	bool FloatArray(std::string option, std::vector<float> display, int &iterator, std::vector<std::string> details = {});
	
	/*
	 * Option that shows a scrollable list of supplied strings.
	 * Shows option with the current value inside < > brackets.
	 * Value shown is display[iterator].
	 * On left or right press, iterator's value is incremented or decremented.
	 * Returns true on accept, left and right.
	 */
	bool StringArray(std::string option, std::vector<std::string> display, int &iterator, std::vector<std::string> details = {});
    void drawInstructionalButtons();
    void drawMenuDetails();

    /*
	 * Draws the menu backgrounds and processes menu navigation key inputs.
	 * Must be called at the end of the menu processing block.
	 */
	void EndMenu();

	/*
	 * Must be used at the beginning of the menu update loop!
	 * Checks input keys and processes them for navigation in the menu with MenuControls
	 */
	void CheckKeys();

	/*
	 * Opens the menu programmatically and calls onMain.
	 * Does nothing if the menu is already open.
	 */
	void OpenMenu();


	/*
	 * Closes the menu and calls onExit.
	 */
	void CloseMenu();

	/*
	 * Returns the filled in menu controls. This can be used for display or 
	 * input verification purposes.
	 */
	const MenuControls &GetControls();

    /*
	 * Returns true if this instance of the menu is open
	 */
	bool IsThisOpen();

    /*
	 * Image prefix string for if you want to show an image in an OptionPlus.
	 */
	const std::string ImagePrefix = "!IMG:";
	const std::string SpritePrefix = "!SPR:";

	/*
	 * These should be filled in by MenuSettings.ReadSettings().
	 */
	float menuX = 0.000f;
	float menuY = 0.000f;
    std::string cheatString = "";

	Color titleTextColor = solidWhite;
	Color titleBackgroundColor = solidWhite;
	int titleFont = 1;

	Color optionsTextColor = solidWhite;
	Color optionsBackgroundColor = solidBlack;

	Color optionsTextSelectColor = solidBlack;
	Color optionsBackgroundSelectColor = solidWhite;
	int optionsFont = 0;

private:
	static const unsigned maxMenus = 255;
	enum class FooterType {
		Default,
		Color,
		Sprite
	};

	MenuControls controls;
	MenuSettings settings;

	std::function<void() > onMain = nullptr;
	std::function<void() > onExit = nullptr;

	/*
	 * Due to how this menu was designed initially, it's expected that 
	 * Menu.End() would be called at the end of the menu tick. At Menu.End(),
	 * all menu options and option counts are known. This information is needed
	 * by the background drawing tasks, so we will store the functions until
	 * the information we need is known. Since we're storing draw calls anyway,
	 * we can split them to draw them in specific "layers". This wasn't a 
	 * problem when the backgrounds were just rects, but with sprites this is
	 * important.
	 */
	typedef std::vector<std::function<void(void)>> functionList;
	functionList backgroundSpriteDraws;		// drawSprite
	functionList backgroundRectDraws;		// drawRect
	functionList highlightsSpriteDraws;		// drawSprite
	functionList foregroundSpriteCalls;		// drawSprite
	functionList textDraws;					// drawText
	
	/*
	 * Detail text also needs to know Y-coordinate to start drawing properly.
	 */
	std::vector<std::string> details;

	/*
	 * These members aren't as modifyable, as they depend on one another. I
	 * wasn't able to find relations between them, so these should not be changed
	 * runtime. They're set to resemble NativeUI / GTA V's UI as much as possible.
	 */
	float menuTextMargin = 0.005f;
	float optionRightMargin = 0.015f;

	float menuWidth = 0.225f;
	float menuWidthOriginal = menuWidth;

	float optionHeight = 0.035f;
	float optionTextSize = 0.45f;
	float optionTextureOffset = optionHeight/2.0f;

	float titleHeight = 0.1f;
	float titleTextSize = 1.15f;
	float titleTextOffset = 0.015f;
	float titleTextureOffset = titleHeight/2.0f;
	
	float subtitleHeight = optionHeight;
	float subtitleTextureOffset = subtitleHeight / 2.0f;
	float subtitleTextSize = 0.45f;

	float detailLineHeight = 0.025f;

	/*
	 * Max items to display. Can be higher or lower, but 10 is a nice
	 * number so the radar doesn't get covered while the description is
	 * readable.
	 */
	int maxDisplay = 10;

	/*
	 * Members for menu state.
	 */
	float totalHeight = 0.0f;
	int optioncount = 0;
	int currentoption = 0;
	bool optionpress = false;
	bool leftpress = false;
	bool rightpress = false;
	bool uppress = false;
	bool downpress = false;
	std::array<std::string, maxMenus> currentmenu;
	std::string actualmenu;
	//std::array<int, 100> lastoption;
	std::unordered_map<std::string, int> lastoption; // lastoption is now per unique submenu
	int menulevel = 0;
	float headerHeight = 0.0f;
	
	FooterType footerType = FooterType::Default;
	Color footerColor = { 0, 0, 0, 191 };
	Sprite footerSprite;

	float aspectR = 16.0f / 9.0f;

	/*
	 * Navigation-related members.
	 */
	unsigned int delay = GetTickCount();
	std::vector<int> menuTimeDelays = {
		240,
		120,
		75,
		40,
		20,
		10,
	};

	const unsigned int menuTimeRepeat = menuTimeDelays[0];

	unsigned int menuTime = menuTimeRepeat;
	bool useNative = true;

	/*
	 * Background textures!
	 * This is... deprecated.
	 * TODO: Remove and just use one default.
	 * DONE: Allow user to specify custom texture.
	 */
	const std::vector<std::string> textureNames = {
		"",
		"gradient_nav",
		"interaction_bgd",
		"gradient_bgd",
		"gradient_nav",
	};
	const std::vector<std::string> textureDicts = {
		"",
		"commonmenu",
		"commonmenu",
		"commonmenu",
		"commonmenu",
	};

	int titleTextureIndex = 2;
	int backgTextureIndex = 3;
	int highlTextureIndex = 4;

    const std::map<int, int, std::greater<int>> recordGlobals{
        { 0, 0 },
        { 10, 0x42CA + 0x09 },
        { 14, 0x42DE + 0x09 },
        { 26, 0x42FF + 0x09 },
        { 28, 0x42FF + 0x82 },
        { 38, 0x430A + 0X82 }
    };

	/*
	 * Functions! Should be self-explanatory.
	 */
	float getStringWidth(std::string text, float scale, int font);
	std::vector<std::string> splitString(float maxWidth, std::string &details, float scale, int font);
	void drawText(const std::string text, int font, float x, float y, float pUnknown, float scale, Color color, int justify);
	void drawRect(float x, float y, float width, float height, Color color);
	void drawSprite(std::string textureDict, std::string textureName, float x, float y, float width, float height, float rotation, Color color);
	void drawOptionPlusTitle(std::string title);
    void drawOptionPlusImage(std::string &extra, float &finalHeight);
    void drawOptionPlusSprite(std::string &extra, float &finalHeight);
    void drawOptionPlusText(std::string &extra, float &finalHeight);
    void drawOptionPlusExtras(std::vector<std::string> &extra, std::string title = "Info");
	void drawMenuDetails(std::vector<std::string> details, float y);
	void drawOptionValue(std::string printVar, bool highlighted, int max = 0);

	void changeMenu(std::string menuname);
	void nextOption();
	void previousOption();
	void backMenu();
	void menuBeep();
	void resetButtonStates();
	void disableKeysOnce();
	void enableKeysOnce();
    void hideHUDComponents();
    void disableKeys();
	void processMenuNav(std::function<void()> onMain, std::function<void()> onExit);
	void updateScreenSize();
	void fitTitle(std::string &title, float &newSize, float titleSize);

	template <typename T>
	bool processOptionItemControls(T &var, T min, T max, T step) {
		if (currentoption == optioncount) {
			if (leftpress) {
				if (var <= min) var = max;
				else var -= step;
				leftpress = false;
				return true;
			}
			if (var < min) var = max;
			if (rightpress) {
				if (var >= max) var = min;
				else var += step;
				rightpress = false;
				return true;
			}
			if (var > max) var = min;
		}

		if (optionpress && currentoption == optioncount)
			return true;
		return false;
	}

	// https://stackoverflow.com/questions/2333728/stdmap-default-value
	template <template<class, class, class...> class C, typename K, typename V, typename... Args>
	V getWithDef(const C<K, V, Args...>& m, K const& key, const V & defval) {
		typename C<K, V, Args...>::const_iterator it = m.find(key);
		if (it == m.end())
			return defval;
		return it->second;
	}
};

}
