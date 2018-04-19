#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <glm/glm.hpp>
#include "Scene.h"
#include "Menu.h"
#include "Utils.h"

#define NUM_SCENES 2
#define NUM_BUTTONS_MAINMENU 2
#define NUM_BUTTONS_ESCMENU 1
#define NUM_BUTTONS_WINMENU 2
#define NUM_BUTTONS_LOSEMENU 1
#define NUM_BUTTONS_CREDITS 1
// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	bool bPlay;                       // Continue to play game?
	bool bPause;
	Scene scene;                      // Scene to render
	Menu menu;
	bool sceneVisible;
	bool sceneActive;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	int mouseX, mouseY;               // Mouse position
	bool bLeftMouse, bRightMouse;     // Mouse button states

	// Scene
	string sceneMaps[NUM_SCENES] = {
		"images/fun1.png",
		"images/fun2.png"
	};

	string sceneMasks[NUM_SCENES] = {
		"images/fun1_mask.png",
		"images/fun2_mask.png"
	};
	
	// Main Menu
	string mainMenuBackground = "images/MainMenu.png";
	glm::vec2 geomMainMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 geomESCMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string mainMenuButtonSprite[NUM_BUTTONS_MAINMENU]{
		"images/buttonPP.png",
		"images/buttonPP.png"
	};

	glm::vec2 mainMenuButtonsPos[NUM_BUTTONS_MAINMENU]{
		glm::vec2(100, 100),
		glm::vec2(100, 150)
	};

	// ESC Menu
	string escMenuBackground = "images/ESCMenu.png";

	string escMenuButtonSprite[NUM_BUTTONS_MAINMENU]{
		"images/buttonPP.png"
	};

	glm::vec2 escMenuButtonsPos[NUM_BUTTONS_MAINMENU]{
		glm::vec2(100, 100)
	};

	// Win Menu
	string winMenuBackground = "images/MainMenu.png";
	glm::vec2 geomWinMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string winMenuButtonSprite[NUM_BUTTONS_WINMENU]{
		"images/buttonPP.png",
		"images/buttonPP.png"
	};

	glm::vec2 winMenuButtonsPos[NUM_BUTTONS_WINMENU]{
		glm::vec2(100, 100),
		glm::vec2(150, 100)
	};

	// Lose Menu
	string loseMenuBackground = "images/MainMenu.png";
	glm::vec2 geomLoseMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string loseMenuButtonSprite[NUM_BUTTONS_LOSEMENU]{
		"images/buttonPP.png"
	};

	glm::vec2 loseMenuButtonsPos[NUM_BUTTONS_LOSEMENU]{
		glm::vec2(100, 100)
	};

	// Credits 
	string creditsBackground = "images/MainMenu.png";
	glm::vec2 geomCredits[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string creditsButtonSprite[NUM_BUTTONS_CREDITS]{
		"images/buttonPP.png"
	};

	glm::vec2 creditsButtonsPos[NUM_BUTTONS_CREDITS]{
		glm::vec2(100, 100)
	};

	// Current Menu
	enum MenuState
	{
		MAINMENU,
		MENUESC,
		MENUWIN,
		MENULOSE,
		CREDITS
	};

	enum Scenes
	{
		LEVEL1, LEVEL2, END
	};

	MenuState currentMenu;
	Scenes currentScene;
	void initScene(int i);
	void initMenu(int i);


};


#endif // _GAME_INCLUDE


