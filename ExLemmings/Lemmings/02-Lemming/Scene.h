#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "Menu.h"
#include "Utils.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define NUM_LEMMINGS 1
#define NUM_BUTTONS 10

class Scene
{

public:
	Scene();
	~Scene();

	void init(string filenameMap, string filenameMask, const glm::vec2& positionEntry, const glm::vec2& positionExit, const glm::vec2& positionLemmings, const glm::vec2& ttSize);
	void update(int deltaTime);
	void render();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseReleased(int mouseX, int mouseY);
	bool checkFinished();

private:
	void initShaders();
	void modifyMask(int mouseX, int mouseY, bool apply);

private:
	Texture colorTexture;
	glm::vec2 textureTrueSize;
	Texture tileTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	bool _finished;
	bool _clicked;
	glm::mat4 projection;
	Lemming *lemmings[NUM_LEMMINGS];
	glm::vec2 _positionExit;
	Menu menu;
	glm::vec2 _geom[2];
	glm::vec2 _texCoords[2];
	glm::vec2 _disp;
	glm::vec2 _clickOrigin;

	// Lemming Menu
	string menuBackground = "images/MainMenu.png";
	glm::vec2 geomMenu[2] = { glm::vec2(0.f, 140.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string menuButtonSprite[NUM_BUTTONS]{
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png"
	};

	glm::vec2 menuButtonPos[NUM_BUTTONS]{
		glm::vec2(float(CAMERA_WIDTH)*(0.f / 10.f), geomMenu[0][1]),
		glm::vec2(float(CAMERA_WIDTH)*(1.f / 10.f), geomMenu[0][1]),
		glm::vec2(float(CAMERA_WIDTH)*(2.f / 10.f), geomMenu[0][1]),
		glm::vec2(float(CAMERA_WIDTH)*(3.f / 10.f), geomMenu[0][1]),
		glm::vec2(float(CAMERA_WIDTH)*(4.f / 10.f), geomMenu[0][1]),
		glm::vec2(float(CAMERA_WIDTH)*(5.f / 10.f), geomMenu[0][1]),
		glm::vec2(float(CAMERA_WIDTH)*(6.f / 10.f), geomMenu[0][1]),
		glm::vec2(float(CAMERA_WIDTH)*(7.f / 10.f), geomMenu[0][1]),
		glm::vec2(float(CAMERA_WIDTH)*(8.f / 10.f), geomMenu[0][1]),
		glm::vec2(float(CAMERA_WIDTH)*(9.f / 10.f), geomMenu[0][1])
	};

};


#endif // _SCENE_INCLUDE

