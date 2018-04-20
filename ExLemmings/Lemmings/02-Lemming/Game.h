#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <glm/glm.hpp>
#include "Scene.h"
#include "Menu.h"
#include "Utils.h"

#define NUM_SCENES 3
#define NUM_BUTTONS_MAINMENU 4
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

	// Scene arguments
	string sceneMaps[NUM_SCENES] = {
		"images/fun1.png",
		"images/fun2.png",
		"images/fun3.png",
	};

	string sceneMasks[NUM_SCENES] = {
		"images/fun1_mask.png",
		"images/fun2_mask.png",
		"images/fun3_mask.png",
	};
	
	int scenePowers[NUM_SCENES][NUM_POWERS] = {
		{ 9, 9, 9, 9, 9, 9, 9, 9 },
		{ 9, 9, 9, 9, 9, 9, 9, 9 },
		{ 9, 9, 9, 9, 9, 9, 9, 9 }
	};

	glm::vec2 sceneEntries[NUM_SCENES]{
		glm::vec2(180, 30),
		glm::vec2(110, 10),
		glm::vec2(132, 3),
	};

	glm::vec2 sceneExits[NUM_SCENES]{
		glm::vec2(340, 91),
		glm::vec2(330, 109),
		glm::vec2(133, 113),
	};

	glm::vec2 sceneSizes[NUM_SCENES]{
		glm::vec2(512, 160),
		glm::vec2(443, 160),
		glm::vec2(437, 160),
	};

	int sceneTimes[NUM_SCENES]{
		300,
		300,
		300
	};

	// Main Menu
	string mainMenuBackground = "images/MainMenu.png";
	glm::vec2 geomMainMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 geomESCMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string mainMenuButtonSprite[NUM_BUTTONS_MAINMENU]{
		"images/buttonPlay.png",
		"images/buttonLevels.png",
		"images/buttonHelp.png",
		"images/buttonCredits.png",
	};

	glm::vec2 mainMenuButtonsPos[NUM_BUTTONS_MAINMENU]{
	    glm::vec2(CAMERA_WIDTH / 2 - 50, CAMERA_HEIGHT/2-45),
		glm::vec2(CAMERA_WIDTH / 2 - 50, CAMERA_HEIGHT/2 - 10),
		glm::vec2(CAMERA_WIDTH / 2 - 50, CAMERA_HEIGHT / 2+25),
		glm::vec2(CAMERA_WIDTH / 2 - 50, CAMERA_HEIGHT / 2 + 60)
	};

	// Select level Menu
	string lvlMenuBackground = "images/LevelMenu.png";
	glm::vec2 geomLvlMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string lvlMenuButtonSprite[NUM_SCENES + 1]{
		"images/buttonL1.png",
		"images/buttonL2.png",
		"images/buttonL3.png",
		"images/buttonBack.png"
	};

	glm::vec2 lvlMenuButtonsPos[NUM_SCENES + 1]{
		glm::vec2(50, 50),
		glm::vec2(85, 50),
		glm::vec2(120,50),
		glm::vec2(50, 150)
	};

	// ESC Menu (instructions)
	string escMenuBackground = "images/HelpMenu.png";

	string escMenuButtonSprite[NUM_BUTTONS_ESCMENU]{
		"images/buttonBack.png",
	};

	glm::vec2 escMenuButtonsPos[NUM_BUTTONS_ESCMENU]{
		glm::vec2(100, 100)
	};

	// Win Menu
	string winMenuBackground = "images/WinMenu.png";
	glm::vec2 geomWinMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string winMenuButtonSprite[NUM_BUTTONS_WINMENU]{
		"images/buttonRetry.png",
		"images/buttonContinue.png",
	};

	glm::vec2 winMenuButtonsPos[NUM_BUTTONS_WINMENU]{
		glm::vec2(CAMERA_WIDTH / 2 - 150, CAMERA_HEIGHT / 2 + 30),
		glm::vec2(CAMERA_WIDTH / 2 + 50, CAMERA_HEIGHT / 2 + 30),
	};

	// Lose Menu
	string loseMenuBackground = "images/LoseMenu.png";
	glm::vec2 geomLoseMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string loseMenuButtonSprite[NUM_BUTTONS_LOSEMENU]{
		"images/buttonRetry.png",
	};

	glm::vec2 loseMenuButtonsPos[NUM_BUTTONS_LOSEMENU]{
		glm::vec2(CAMERA_WIDTH / 2 - 50, CAMERA_HEIGHT / 2 + 30)
	};

	// Credits 
	string creditsBackground = "images/AboutMenu.png";
	glm::vec2 geomCredits[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string creditsButtonSprite[NUM_BUTTONS_CREDITS]{
		"images/buttonBack.png",
	};

	glm::vec2 creditsButtonsPos[NUM_BUTTONS_CREDITS]{
		glm::vec2(CAMERA_WIDTH / 2 - 50, CAMERA_HEIGHT / 2 + 60)
	};

	// Current Menu
	enum MenuState
	{
		MAINMENU,
		SELECTLVL,
		MENUESC,
		MENUWIN,
		MENULOSE,
		CREDITS
	};

	enum Scenes
	{
		LEVEL1, LEVEL2, LEVEL3, END
	};

	MenuState currentMenu;
	Scenes currentScene;
	void initScene(int i);
	void initMenu(int i);

};


#endif // _GAME_INCLUDE


