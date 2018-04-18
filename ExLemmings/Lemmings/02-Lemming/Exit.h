#ifndef _EXIT_INCLUDE
#define _EXIT_INCLUDE

#include "Sprite.h"
#include "VariableTexture.h"
#include "Utils.h"

class Exit
{

public:

	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);

	void update(int deltaTime, glm::vec2 disp);

	void render();

	void setMapMask(VariableTexture *mapMask);

	glm::vec2 getBasePosition();

private:


	void loadSpritesheet(string filename, int NUM_FRAMES, int NUM_ANIMS, const glm::vec2& position, int speed);

	enum ExitState
	{
		NORMAL
	};

	//We need to use different textures for some animations
	//therefore their indices will be the same
	enum ExitAnim
	{
		ANIM = 0
	};

	int _framesFromStart = 0; //frames from the start of some animation, useful when building, exploding, etc.

	ExitState _state;
	Texture _spritesheet;
	Sprite *_sprite;
	VariableTexture *_mask;
	ShaderProgram _shaderProgram;
	float _dispX;
	float _dispY;
};


#endif // _EXIT_INCLUDE


