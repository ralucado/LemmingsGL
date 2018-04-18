#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	sceneVisible = false;
	sceneActive = false;
	currentMenu = MAINMENU;
	menu.init(mainMenuBackground, geomMainMenu, mainMenuButtonSprite, mainMenuButtonsPos, NUM_BUTTONS_MAINMENU);
}

bool Game::update(int deltaTime)
{
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
			//                                      entry              exit               lemmings           drawing size
			scene.init(sceneMaps[0], sceneMasks[0], glm::vec2(60, 30), glm::vec2(360, 130), glm::vec2(180, 30), glm::vec2(512, 159));
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
			//                                      entry              exit               lemmings           drawing size
			scene.init(sceneMaps[0], sceneMasks[0], glm::vec2(60, 30), glm::vec2(240, 131), glm::vec2(180, 30), glm::vec2(512, 159));
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
	if(key == 27) // Escape code
		bPlay = false;
	if (key == 49) //1
		scene.init(sceneMaps[0], sceneMasks[0], glm::vec2(60, 30), glm::vec2(240, 131), glm::vec2(180, 30), glm::vec2(512, 159));
		//                                      entry              exit               lemmings           drawing size
	if (key == 50) //2
		scene.init(sceneMaps[1], sceneMasks[1], glm::vec2(60, 30), glm::vec2(240, 131), glm::vec2(110, 10), glm::vec2(443, 160));
	keys[key] = true;
	
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
	
	if (sceneActive)
		scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
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





