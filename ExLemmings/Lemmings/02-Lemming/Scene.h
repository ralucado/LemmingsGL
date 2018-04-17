#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "Button.h"
#include "Utils.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	static Scene &instance()
	{
		static Scene S;

		return S;
	}

	void init(string filenameMap, string filenameMask, const glm::vec2& positionEntry, const glm::vec2& positionExit, const glm::vec2& positionLemmings);
	void update(int deltaTime);
	void render();
	
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	void keyPressed(int key);

	void keyReleased(int key);

	float getDisplacementX() const;
	float getDisplacementY() const;
private:
	void initShaders();
	void modifyMask(int mouseX, int mouseY, bool apply);

private:
	Texture colorTexture;
	Texture tileTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	glm::mat4 projection;
	Lemming lemming;
	Button button;
	glm::vec2 _geom[2];
	glm::vec2 _texCoords[2];
	float _dispX, _dispY;

};


#endif // _SCENE_INCLUDE

