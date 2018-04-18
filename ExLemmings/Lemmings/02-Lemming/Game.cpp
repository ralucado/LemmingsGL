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
	currentMenu = MAINMENU;
	menu.init(mainMenuBackground, geomMainMenu, mainMenuButtonSprite, mainMenuButtonsPos, NUM_BUTTONS_MAINMENU);
}

void Game::initScene(int i)
{
	int powers[NUM_POWERS] = {1, 1, 1, 1, 1, 1, 1, 1, 0};
	switch (i){
	case LEVEL1:
	    //                                      entry               exit                lemmings            drawing size         powercount
		scene.init(sceneMaps[0], sceneMasks[0], glm::vec2(180, 30), glm::vec2(340, 91), glm::vec2(180, 30), glm::vec2(512, 159), powers);
		break;
	case LEVEL2:
		scene.init(sceneMaps[1], sceneMasks[1], glm::vec2(110, 10), glm::vec2(330, 109), glm::vec2(110, 10), glm::vec2(443, 160), powers);
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
			menu.~Menu();
			currentMenu = MENUESC;
			menu.init(escMenuBackground, geomESCMenu,escMenuButtonSprite, escMenuButtonsPos, NUM_BUTTONS_ESCMENU);
			sceneActive = true;
			sceneVisible = true;
			initScene(LEVEL1);
			break;
		default:
			break;
		}
		break;
	case MENUESC:
		break;
	default:
		break;
	}

	if (sceneActive) {
		if (scene.checkFinished())
			initScene(LEVEL1);
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
	if(key == 27) // Escape code
		bPlay = false;
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






