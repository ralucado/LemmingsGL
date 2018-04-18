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
#define NUM_BUTTONS 3

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
	Menu menuPowers, menuControl;
	glm::vec2 _geom[2];
	glm::vec2 _texCoords[2];
	glm::vec2 _disp;
	glm::vec2 _clickOrigin;

	// Lemming Menu
	string menuPowersBackground = "images/MainMenu.png";
	glm::vec2 geomMenuPowers[2] = { glm::vec2(0.f, 140.f), glm::vec2(float(CAMERA_WIDTH)*(8.f/11.f), float(CAMERA_HEIGHT)) };

	string menuPowersButtonSprite[NUM_POWERS]{
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png"
	};

	glm::vec2 menuPowersButtonPos[NUM_POWERS]{
		glm::vec2(geomMenuPowers[1][0] *(0.f / NUM_POWERS), geomMenuPowers[0][1]),
		glm::vec2(geomMenuPowers[1][0] *(1.f / NUM_POWERS), geomMenuPowers[0][1]),
		glm::vec2(geomMenuPowers[1][0] *(2.f / NUM_POWERS), geomMenuPowers[0][1]),
		glm::vec2(geomMenuPowers[1][0] *(3.f / NUM_POWERS), geomMenuPowers[0][1]),
		glm::vec2(geomMenuPowers[1][0] *(4.f / NUM_POWERS), geomMenuPowers[0][1]),
		glm::vec2(geomMenuPowers[1][0] *(5.f / NUM_POWERS), geomMenuPowers[0][1]),
		glm::vec2(geomMenuPowers[1][0] *(6.f / NUM_POWERS), geomMenuPowers[0][1]),
		glm::vec2(geomMenuPowers[1][0] *(7.f / NUM_POWERS), geomMenuPowers[0][1])
	};

	// Control Menu
	string menuControlBackground = "images/MainMenu.png";
	glm::vec2 geomMenuControl[2] = { glm::vec2(float(CAMERA_WIDTH)*(8.f / 11.f), 140.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string menuControlButtonSprite[NUM_BUTTONS]{
		"images/buttonPP.png",
		"images/buttonPP.png",
		"images/buttonPP.png"
	};

	glm::vec2 menuControlButtonPos[NUM_BUTTONS]{
		glm::vec2((geomMenuControl[1][0] - geomMenuControl[0][0]) * (0.f / NUM_BUTTONS) + geomMenuControl[0][0], geomMenuControl[0][1]),
		glm::vec2((geomMenuControl[1][0] - geomMenuControl[0][0]) * (1.f / NUM_BUTTONS) + geomMenuControl[0][0], geomMenuControl[0][1]),
		glm::vec2((geomMenuControl[1][0] - geomMenuControl[0][0]) * (2.f / NUM_BUTTONS) + geomMenuControl[0][0], geomMenuControl[0][1])
	};

};


#endif // _SCENE_INCLUDE

