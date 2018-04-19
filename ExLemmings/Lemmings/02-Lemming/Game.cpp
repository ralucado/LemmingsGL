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
	int powers[NUM_POWERS] = {1, 1, 1, 1, 1, 1, 1, 1, 0};
	switch (i){
	case LEVEL1:
	    //                                      entry               exit                lemmings            drawing size         powercount
		currentScene = LEVEL1;
		scene.init(sceneMaps[0], sceneMasks[0], glm::vec2(180, 30), glm::vec2(340, 91), glm::vec2(180, 30), glm::vec2(512, 159), powers);
		break;
	case LEVEL2:
		currentScene = LEVEL2;
		scene.init(sceneMaps[1], sceneMasks[1], glm::vec2(110, 10), glm::vec2(330, 109), glm::vec2(110, 10), glm::vec2(443, 160), powers);
		break;
	case END:
		sceneVisible = false;
		sceneActive = false;
		initMenu(CREDITS);
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
	case MENUESC:
		currentMenu = MENUESC;
		menu.init(escMenuBackground, geomESCMenu, escMenuButtonSprite, escMenuButtonsPos, NUM_BUTTONS_ESCMENU);
		break;
	case MENUWIN:
		currentMenu = MENUWIN;
		menu.init(winMenuBackground, geomWinMenu, winMenuButtonSprite, winMenuButtonsPos, NUM_BUTTONS_WINMENU);
		break;
	case MENULOSE:
		currentMenu = MENULOSE;
		menu.init(loseMenuBackground, geomLoseMenu, loseMenuButtonSprite, loseMenuButtonsPos, NUM_BUTTONS_LOSEMENU);
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
			initMenu(CREDITS);
			break;
		default:
			break;
		}
		break;
	case MENUESC:
		switch (menu.buttonPressed())
		{
		case 0:
			initMenu(MENUESC);
			sceneActive = true;
			sceneVisible = true;
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






