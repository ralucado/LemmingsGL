#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	bPause = false;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	sceneVisible = false;
	sceneActive = false;
	initMenu(MAINMENU);
}

void Game::initScene(int i)
{
	initMenu(MENUESC);
	sceneActive = true;
	sceneVisible = true;
	if (i == END) {
		sceneVisible = false;
		sceneActive = false;
		initMenu(CREDITS);
	}
	else {
		currentScene = Scenes(i);
		scene.init(sceneMaps[i], sceneMasks[i], sceneEntries[i], sceneExits[i], sceneSizes[i], scenePowers[i], 10, 1);
	}
}

void Game::initMenu(int i)
{
	menu = Menu();
	switch (i) {
	case MAINMENU:
		currentMenu = MAINMENU;
		menu.init(mainMenuBackground, geomMainMenu, mainMenuButtonSprite, mainMenuButtonsPos, NUM_BUTTONS_MAINMENU);
		break;
	case SELECTLVL:
		currentMenu = SELECTLVL;
		//menu.init(lvlMenuBackground, geomLvlMenu, lvlMenuButtonSprite, lvlMenuButtonsPos, NUM_SCENES + 1);
		menu.init(lvlMenuBackground, geomLvlMenu, lvlMenuButtonSprite, lvlMenuButtonsPos, NUM_SCENES + 2);
		break;
	case MENUESC:
		currentMenu = MENUESC;
		menu.init(escMenuBackground, geomESCMenu, escMenuButtonSprite, escMenuButtonsPos, NUM_BUTTONS_ESCMENU);
		break;
	case MENUWIN:
		currentMenu = MENUWIN;
		menu.init(winMenuBackground, geomWinMenu, winMenuButtonSprite, winMenuButtonsPos, NUM_BUTTONS_WINMENU);
		menu.initText("SAVED: " + to_string(scene.getSaved()) + " OF " + to_string(scene.getTotal()), glm::vec2(100, 20), 30, glm::vec4(1, 1, 1, 1));
		menu.initText("NEED: "+to_string(scene.getMin()), glm::vec2(100, 50), 30, glm::vec4(1, 1, 1, 1));
		menu.initText("UEUEU CONGRATS ", glm::vec2(100, 70), 30, glm::vec4(1, 1, 1, 1));
		break;
	case MENULOSE:
		currentMenu = MENULOSE;
		menu.init(loseMenuBackground, geomLoseMenu, loseMenuButtonSprite, loseMenuButtonsPos, NUM_BUTTONS_LOSEMENU);
		menu.initText("SAVED: " + to_string(scene.getSaved()) + " OF " + to_string(scene.getTotal()), glm::vec2(100, 20), 30, glm::vec4(1, 1, 1, 1));
		menu.initText("NEED: " + to_string(scene.getMin()), glm::vec2(100, 50), 30, glm::vec4(1, 1, 1, 1));
		menu.initText("TRY AGAIN :c", glm::vec2(100, 70), 30, glm::vec4(1, 1, 1, 1));
		break;
	case CREDITS:
		currentMenu = CREDITS;
		menu.init(creditsBackground, geomCredits, creditsButtonSprite, creditsButtonsPos, NUM_BUTTONS_CREDITS);
		break;
	}
}

bool Game::update(int deltaTime)
{
	if (bPause) return bPlay;
	switch (currentMenu)
	{
	case MAINMENU:
		switch (menu.buttonPressed())
		{
		case 0:
			initScene(LEVEL1);
			break;
		case 1:
			initMenu(SELECTLVL);
			break;
		case 2:
			initMenu(MENUESC);
			break;
		case 3:
			initMenu(CREDITS);
			break;
		default:
			break;
		}
		break;
	case SELECTLVL:
		switch (menu.buttonPressed())
		{
		case 0:
			initScene(LEVEL1);
			break;
		case 1:
			initScene(LEVEL2);
			break;
		/*case 2:
			initScene(LEVEL3);
			break;*/
		case 3:
			initMenu(MAINMENU);
			break;
		default:
			break;
		}
		break;
	case MENUESC:
		switch (menu.buttonPressed())
		{
		case 0:
			if (sceneVisible) {
				initMenu(MENUESC);
				sceneActive = true;
			}
			else initMenu(MAINMENU);
			
			break;
		default:
			break;
		}
		break;
	case MENUWIN:
		switch (menu.buttonPressed())
		{
		case 0:
			initScene(currentScene);
			break;
		case 1:
			initScene(currentScene+1);
			break;
		default:
			break;
		}
		break;
	case MENULOSE:
		switch (menu.buttonPressed())
		{
		case 0:
			initScene(currentScene);
			break;
		default:
			break;
		}
		break;
	case CREDITS:
		switch (menu.buttonPressed())
		{
		case 0:
			initMenu(MAINMENU);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	if (sceneActive) {
		if (scene.checkFinished()) {
			sceneActive = false;
			sceneVisible = false;
			if (scene.checkWin())
				initMenu(MENUWIN);
			else
				initMenu(MENULOSE);
		}
		else
			scene.update(deltaTime);
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (sceneVisible)
		scene.render();
	if (!sceneActive)
		menu.render();
}

void Game::keyPressed(int key)
{
	keys[key] = true;
	if (key == 27) { // Escape code
		//bPlay = false;
		if (sceneActive) {
			sceneActive = false;
			initMenu(MENUESC);
		}
		else {
			if (sceneVisible) 
				sceneActive = true;
		}

	}
	if (key == 49) //1
		initScene(LEVEL1);
	if (key == 50) //2
		initScene(LEVEL2);
	if (key == 51) //3
		initScene(LEVEL3);
	if (key == 57) //9
		bPause = !bPause;
	if (sceneActive)
		scene.keyPressed(key);
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	if (sceneActive)
		scene.keyReleased(key);
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
	if (sceneActive)
		scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	else 
		menu.mouseMoved(mouseX, mouseY, bLeftMouse);
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
		bLeftMouse = true;
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = true;
	
	if (sceneActive) {
		scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		scene.mouseLeftPressed(mouseX, mouseY);
	}
	else
		menu.mouseMoved(mouseX, mouseY, bLeftMouse);
}

void Game::mouseRelease(int button)
{
	if (button == GLUT_LEFT_BUTTON) 
		bLeftMouse = false;
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = false;
	
	if (sceneActive)
		scene.mouseReleased(mouseX, mouseY);
	else
		menu.mouseReleased(mouseX, mouseY);
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}






