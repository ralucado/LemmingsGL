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
#define NUM_LEMMINGS_MIN 2

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
	bool checkWin();

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
	int lemmingsSaved;
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
	glm::vec2 geomMenuPowers[2] = { glm::vec2(0.f, float(CAMERA_HEIGHT)-31.f), glm::vec2(float(CAMERA_WIDTH)*(8.f/11.f), float(CAMERA_HEIGHT)) };

	string menuPowersButtonSprite[NUM_POWERS]{
		"images/buttonBlock.png",
		"images/buttonBomb.png",
		"images/buttonBash.png",
		"images/buttonFloat.png",
		"images/buttonDig.png",
		"images/buttonClimb.png",
		"images/buttonBuild.png",
		"images/buttonMine.png"
	};

	glm::vec2 menuPowersButtonPos[NUM_POWERS];

	// Control Menu
	string menuControlBackground = "images/MainMenu.png";
	glm::vec2 geomMenuControl[2] = { glm::vec2(float(CAMERA_WIDTH)*(8.f / 11.f), float(CAMERA_HEIGHT) - 31.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };

	string menuControlButtonSprite[NUM_BUTTONS]{
		"images/buttonPause.png",
		"images/buttonSpeed.png",
		"images/buttonNuke.png"
	};

	glm::vec2 menuControlButtonPos[NUM_BUTTONS];

};


#endif // _SCENE_INCLUDE
