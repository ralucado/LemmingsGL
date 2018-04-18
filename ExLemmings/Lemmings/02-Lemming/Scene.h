#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "Button.h"
#include "Utils.h"
#include "Exit.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

#define NUM_LEMMINGS 1

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
	bool checkFinished();

private:
	void initShaders();
	void modifyMask(int mouseX, int mouseY, bool apply);

private:
	void loadSpritesheet(string filename, int NUM_FRAMES, int NUM_ANIMS, const glm::vec2 & position, Sprite*& _sprite, Texture& texture);
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
	Exit exit;
	glm::vec2 _geom[2];
	glm::vec2 _texCoords[2];
	glm::vec2 _disp;
	glm::vec2 _clickOrigin;
	glm::vec2 _positionExit;
};


#endif // _SCENE_INCLUDE

