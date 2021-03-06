#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <glm/glm.hpp>
#include "Scene.h"
#include "Menu.h"
#include "Utils.h"

#define NUM_SCENES 6
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#define NUM_BUTTONS_MAINMENU 4
#define NUM_BUTTONS_ESCMENU 1
#define NUM_BUTTONS_WINMENU 3
#define NUM_BUTTONS_LOSEMENU 2
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
	ISoundEngine * engine;
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
		"images/fun4.png",
		"images/fun5.png",
		"images/tricky1.png",
		/*
		"images/tricky2.png",
		"images/tricky3.png",
		"images/tricky4.png",
		"images/tricky5.png",
		"images/taxing1.png",
		"images/taxing2.png",
		"images/taxing3.png",
		"images/taxing4.png",
		"images/taxing5.png"*/
	};

	string sceneMasks[NUM_SCENES] = {
		"images/fun1_mask.png",
		"images/fun2_mask.png",
		"images/fun3_mask.png",
		"images/fun4_mask.png",
		"images/fun5_mask.png",
		"images/tricky1_mask.png",
		/*
		"images/tricky2_mask.png",
		"images/tricky3_mask.png",
		"images/tricky4_mask.png",
		"images/tricky5_mask.png",
		"images/taxing1_mask.png",
		"images/taxing2_mask.png",
		"images/taxing3_mask.png",
		"images/taxing4_mask.png",
		"images/taxing5_mask.png"*/
	};
	
	int scenePowers[NUM_SCENES][NUM_POWERS] = {
		// Fun
		{ 0,  0, 0,  0,  10, 0,  0, 0 },
		{ 0,  0, 0,  10, 0,  0,  0, 0 },
		{ 10, 0, 0,  0,  0,  0,  0, 0 },
		{ 0,  0, 0,  0,  0,  10, 0, 1 },
		{ 0,  0, 50, 0,  0,  0,  0, 0 },
		{ 10, 10, 10, 10, 10, 10, 10, 10 },
		/*
		// Tricky
		{ 20, 20, 20, 20, 20, 20, 40, 20 },
		{ 20, 20, 20, 20, 20, 20, 20, 20 },
		{ 20, 20, 20, 20, 20, 20, 20, 20 },
		{ 20, 0, 0, 0, 0, 0, 50, 0 },
		// Taxing
		{ 2, 2, 2, 2, 2, 2, 8, 2 },
		{ 10, 5, 5, 5, 5, 10, 10, 5 },
		{ 4, 5, 2, 0, 2, 0, 20, 2 },
		{ 4, 5, 10, 0, 3, 0, 20, 0 },
		{ 3, 3, 5, 0, 1, 0, 0, 0 }
		*/

	};

	glm::vec2 sceneEntries[NUM_SCENES]{
		glm::vec2(180, 30),
		glm::vec2(110, 10),
		glm::vec2(132, 3),
		glm::vec2(201, 12),
		glm::vec2(123, 23),
		glm::vec2(193, 53),
	};

	glm::vec2 sceneExits[NUM_SCENES]{
		glm::vec2(340, 91),
		glm::vec2(330, 109),
		glm::vec2(133, 113),
		glm::vec2(366, 8),
		glm::vec2(602, 91),
		glm::vec2(737, 77),
	};

	glm::vec2 sceneSizes[NUM_SCENES]{
		glm::vec2(512, 160),
		glm::vec2(443, 160),
		glm::vec2(437, 160),
		glm::vec2(564, 160),
		glm::vec2(848, 160),
		glm::vec2(924, 160),
		//glm::vec2(705, 160),
		//glm::vec2(1600, 160),
		//glm::vec2(1211, 160),
		//glm::vec2(396, 160),
	};
	
	int lemmingsNeeded[NUM_SCENES]{
		1, 1, 5, 10, 5, 50,
		// 50, 20, 4, 60,
		//99, 80, 30, 45, 30
	};

	int lemmingsTotal[NUM_SCENES]{
		10, 10, 50, 10, 50, 100,
		//100, 100, 5, 100,
		//100, 100, 40, 60, 50
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
		"images/buttonL1.png", "images/buttonL2.png", "images/buttonL3.png", "images/buttonL4.png", "images/buttonL5.png",
		"images/buttonL6.png", 
		//"images/buttonL7.png", "images/buttonL8.png", "images/buttonL9.png", "images/buttonL10.png",
		//"images/buttonL11.png", "images/buttonL12.png", "images/buttonL13.png", "images/buttonL14.png", "images/buttonL15.png",
		"images/buttonBack.png"
	};

	glm::vec2 lvlMenuButtonsPos[NUM_SCENES + 1]{
		glm::vec2(50, 50), glm::vec2(85, 50), glm::vec2(120, 50), glm::vec2(155, 50), glm::vec2(190, 50),
		glm::vec2(50, 85), 
		// glm::vec2(85, 85), glm::vec2(120, 85), glm::vec2(155, 85), glm::vec2(190, 85),
		//glm::vec2(50, 120), glm::vec2(85, 120), glm::vec2(120, 120), glm::vec2(155, 120), glm::vec2(190, 120),
		glm::vec2(50, 150)
	};

	// ESC Menu (instructions)
	string escMenuBackground = "images/HelpMenu.png";

	string escMenuButtonSprite[NUM_BUTTONS_ESCMENU]{
		"images/buttonBack.png"
	};

	string escMenuButtonSprite2[NUM_BUTTONS_ESCMENU+1]{
		"images/buttonBack.png",
		"images/buttonMenu.png",
	};

	glm::vec2 escMenuButtonsPos[NUM_BUTTONS_ESCMENU]{
		glm::vec2(CAMERA_WIDTH / 2 - 50, CAMERA_HEIGHT / 2 + 60)
	};

	glm::vec2 escMenuButtonsPos2[NUM_BUTTONS_ESCMENU+1]{
		glm::vec2(CAMERA_WIDTH / 2 + 50, CAMERA_HEIGHT / 2 + 60),
		glm::vec2(CAMERA_WIDTH / 2 - 150, CAMERA_HEIGHT / 2 + 60),
	};

	// Win Menu
	string winMenuBackground = "images/WinMenu.png";
	glm::vec2 geomWinMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string winMenuButtonSprite[NUM_BUTTONS_WINMENU]{
		"images/buttonRetry.png",
		"images/buttonContinue.png",
		"images/buttonMenu.png"
	};

	glm::vec2 winMenuButtonsPos[NUM_BUTTONS_WINMENU]{
		glm::vec2(CAMERA_WIDTH / 2 - 50, CAMERA_HEIGHT / 2 + 60),
		glm::vec2(CAMERA_WIDTH / 2 + 50, CAMERA_HEIGHT / 2 + 60),
		glm::vec2(CAMERA_WIDTH / 2 - 150, CAMERA_HEIGHT / 2 + 60)
	};

	// Lose Menu
	string loseMenuBackground = "images/LoseMenu.png";
	glm::vec2 geomLoseMenu[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string loseMenuButtonSprite[NUM_BUTTONS_LOSEMENU]{
		"images/buttonRetry.png",
		"images/buttonMenu.png"
	};

	glm::vec2 loseMenuButtonsPos[NUM_BUTTONS_LOSEMENU]{
		glm::vec2(CAMERA_WIDTH / 2 - 50, CAMERA_HEIGHT / 2 + 60),
		glm::vec2(CAMERA_WIDTH / 2 - 150, CAMERA_HEIGHT / 2 + 60)
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
		LEVEL1, LEVEL2, LEVEL3, LEVEL4, LEVEL5,
		LEVEL6, //LEVEL7, LEVEL8, LEVEL9, LEVEL10,
	//	LEVEL11, LEVEL12, LEVEL13, LEVEL14, LEVEL15,
		END
	};

	MenuState currentMenu;
	Scenes currentScene;
	void initScene(int i);
	void initMenu(int i);


	string motivationalLines[3]{
		"Try again",
		"Don't let your dreams be dreams",
		"YES, you can!"
	};

	string winLines[3]{
		"You saved them all! WOW",
		"100%? You are the real MVP!",
		"You saved them all! Good Job!"
	};

};


#endif // _GAME_INCLUDE


