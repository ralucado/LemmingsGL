#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//scene.init(sceneMaps[0], sceneMasks[0], glm::vec2(60, 30), glm::vec2(240, 131), glm::vec2(60, 30));
	menuActive = true;
	mainMenu.init("images/MainMenu.png");
}

bool Game::update(int deltaTime)
{
	
	if (!menuActive) 
		if (scene.checkFinished())
			scene.init(sceneMaps[0], sceneMasks[0], glm::vec2(60, 30), glm::vec2(240, 131), glm::vec2(60, 30));
	else 
		scene.update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//scene.render();
	if (menuActive)
		mainMenu.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (key == 49) 
		scene.init(sceneMaps[0], sceneMasks[0], glm::vec2(60, 30), glm::vec2(360, 130), glm::vec2(60, 30));
	if (key == 50)
		scene.init(sceneMaps[1], sceneMasks[1], glm::vec2(60, 30), glm::vec2(360, 130), glm::vec2(60, 30));
	keys[key] = true;
	//scene.keyPressed(key);
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	//scene.keyReleased(key);
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
	if (menuActive)
		mainMenu.mouseMoved(mouseX, mouseY, bLeftMouse);
	//else
	//	scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = true;
		if (menuActive)
			mainMenu.mouseMoved(mouseX, mouseY, bLeftMouse);
		//else
		//	scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;
		if (menuActive)
			mainMenu.mouseMoved(mouseX, mouseY, bLeftMouse);
		//else
		//	scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
}

void Game::mouseRelease(int button)
{
	if(button == GLUT_LEFT_BUTTON)
		bLeftMouse = false;
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





