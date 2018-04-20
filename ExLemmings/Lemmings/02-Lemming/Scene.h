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
#include "Text.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define NUM_BUTTONS 3
#define LEVEL_TIME 300

class Scene
{

public:
	Scene();
	~Scene();


	void init(string filenameMap, string filenameMask, const glm::vec2 & positionEntry, const glm::vec2 & positionExit, const glm::vec2 & ttSize, int powerCount[], int iniLemmings, int finLemmings);
	void update(int deltaTime);
	void render();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	void mouseLeftPressed(int mouseX, int mouseY);
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseReleased(int mouseX, int mouseY);
	bool checkFinished();
	bool checkWin();
	int getSaved();
	int getTotal();
	int getMin();


private:
	void initShaders();
	void initMenus();
	void modifyMask(int mouseX, int mouseY, bool apply);
	void givePower(int i);
private:
	enum Power {
		BLOCK, BOMB, BASH, FLOAT, DIG, CLIMB, BUILD, MINE, NONE
	};
	vector<int> _powerCount;
	Power _activePower;
	int _iniLemmings, _finLemmings;
	void loadSpritesheet(string filename, int NUM_FRAMES, int NUM_ANIMS, const glm::vec2 & position, Sprite*& _sprite, Texture& texture);
	Texture colorTexture;
	Cursor cursor;
	glm::vec2 textureTrueSize;
	int _selectedLemming;
	Texture tileTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram, lemmingTexProgram;
	float currentTime;
	bool _finished;
	int lemmingsSaved;
	int lemmingsDead;
	bool _clicked;
	glm::mat4 projection;
	vector<Lemming*>lemmings;
	Menu menuPowers, menuControl;
	Exit exit;
	Entry entry;
	glm::vec2 _geom[2];
	glm::vec2 _texCoords[2];
	glm::vec2 _disp;
	glm::vec2 _clickOrigin;
	glm::vec2 _positionEntry;
	set<pair<int, int>> _blockers;
	// Lemming Menu
	string menuPowersBackground = "images/MainMenu.png";
	glm::vec2 geomMenuPowers[2] = { glm::vec2(0.f, float(CAMERA_HEIGHT)-31.f), glm::vec2(float(CAMERA_WIDTH)*(8.f/13.f), float(CAMERA_HEIGHT)) };

	string menuPowersButtonSprite[NUM_POWERS]{
		"images/buttonBlock.png",
		"images/buttonBomb.png",
		"images/buttonBash.png",
		"images/buttonFloat.png",
		"images/buttonDig.png",
		"images/buttonClimb.png",
		"images/buttonBuild.png",
		"images/buttonMine.png",
	};

	glm::vec2 menuPowersButtonPos[NUM_POWERS];

	// Control Menu
	string menuControlBackground = "images/MainMenu.png";
  glm::vec2 geomMenuControl[2] = { glm::vec2(float(CAMERA_WIDTH)*(8.f / 13.f), float(CAMERA_HEIGHT) - 31.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string menuControlButtonSprite[NUM_BUTTONS]{
		"images/buttonPause.png",
		"images/buttonSpeed.png",
		"images/buttonNuke.png"
	};

	glm::vec2 menuControlButtonPos[NUM_BUTTONS];
	// Text
	string textString[4] = {
		"OUT: " + to_string(NUM_LEMMINGS),
		"SAVED: " + to_string(0),
		"MIN: " + to_string(NUM_LEMMINGS_MIN),
		"TIME: " + to_string(LEVEL_TIME/60) + ":00"
	};



	/*
	glm::vec2 textPos[4] = {
		glm::vec2(float(CAMERA_WIDTH)*(11.f / 13.f)*3, float(CAMERA_HEIGHT)*3 - 85.f),
		glm::vec2(float(CAMERA_WIDTH)*(11.f / 13.f)*3, float(CAMERA_HEIGHT)*3 - 68.f),
		glm::vec2(float(CAMERA_WIDTH)*(11.f / 13.f)*3, float(CAMERA_HEIGHT)*3 - 51.f),
		glm::vec2(float(CAMERA_WIDTH)*(11.f / 13.f)*3, float(CAMERA_HEIGHT)*3 - 34.f)
	};

	int textSize[4] = {
		15,
		15,
		15,
		15
	};

	glm::vec4 textColor[4] = {
		glm::vec4(1, 1, 1, 1),
		glm::vec4(1, 1, 1, 1),
		glm::vec4(1, 1, 1, 1),
		glm::vec4(1, 1, 1, 1)
	};*/

	int _totalLemmings, _targetLemmings;
	bool _nuke;
	float _spawnTime;
};


#endif // _SCENE_INCLUDE
