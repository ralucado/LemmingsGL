#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	//                                      entry              exit               lemmings            drawing size
	scene.init(sceneMaps[0], sceneMasks[0], glm::vec2(60, 30), glm::vec2(60, 30), glm::vec2(180, 30), glm::vec2(512, 159));
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}



void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (key == 49) //1
		scene.init(sceneMaps[0], sceneMasks[0], glm::vec2(60, 30), glm::vec2(60, 30), glm::vec2(180, 30), glm::vec2(512, 159));
		//                                      entry              exit               lemmings           drawing size
	if (key == 50) //2
		scene.init(sceneMaps[1], sceneMasks[1], glm::vec2(60, 30), glm::vec2(60, 30), glm::vec2(110, 10), glm::vec2(443, 160));
	keys[key] = true;
	scene.keyPressed(key);
}

void Game::keyReleased(int key)
{
	keys[key] = false;
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
	scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = true;
		scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;
		scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
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





