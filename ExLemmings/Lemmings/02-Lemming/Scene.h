#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "Menu.h"
#include "Utils.h"
#include "Exit.h"
#include "Entry.h"
#include "Cursor.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define NUM_BUTTONS 3
#define NUM_LEMMINGS 2

class Scene
{

public:
	Scene();
	~Scene();


	void init(string filenameMap, string filenameMask, const glm::vec2& positionEntry, const glm::vec2& positionExit, const glm::vec2& positionLemmings, const glm::vec2& ttSize, int powerCount[]);
	void update(int deltaTime);
	void render();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	void mouseLeftPressed(int mouseX, int mouseY);
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseReleased(int mouseX, int mouseY);
	bool checkFinished();

private:
	void initShaders();
	void modifyMask(int mouseX, int mouseY, bool apply);
	void givePower(int i);
private:
	enum Power {
		BLOCK, BOMB, BASH, FLOAT, DIG, CLIMB, BUILD, MINE, NONE
	};
	vector<int> _powerCount;
	Power _activePower;
	void loadSpritesheet(string filename, int NUM_FRAMES, int NUM_ANIMS, const glm::vec2 & position, Sprite*& _sprite, Texture& texture);
	Texture colorTexture;
	Cursor cursor;
	glm::vec2 textureTrueSize;
	int _selectedLemming;
	Texture tileTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	bool _finished;
	bool _clicked;
	glm::mat4 projection;
	Lemming *lemmings[NUM_LEMMINGS];
	Menu menuPowers, menuControl;
	Exit exit;
	Entry entry;
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
