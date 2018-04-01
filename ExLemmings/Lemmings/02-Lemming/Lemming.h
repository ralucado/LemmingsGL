#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "Utils.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void loadSpritesheet(string filename, int NUM_FRAMES, int NUM_ANIMS, const glm::vec2& position);
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setMapMask(VariableTexture *mapMask);

	void switchStopper();

	void switchBomber();

	void startBash(bool r);

	void startWalk(bool r);

	void switchBasher(bool r);
	
private:
	int collisionFloor(int maxFall);
	int collisionWall(int maxDeep, bool r);
	bool collision();

	void hole(int posX, int posY, int radius);

	void pop();

	void bashRow(int index, bool r);

	
private:

	glm::vec2 bashPixels[5] = {glm::vec2(13,6) , glm::vec2(14, 8) , glm::vec2(15, 10) , glm::vec2(14, 12), glm::vec2(14, 14)};

	enum LemmingState
	{
		WALKING_RIGHT, WALKING_LEFT,
		FALLING_RIGHT, FALLING_LEFT,
		STOPPED,
		EXPLODING,
		BASH_RIGHT, BASH_LEFT,
/*		CLIMBING,
		END_CLIMBING,
		START_FLOAT_RIGHT, START_FLOAT_LEFT,
		FLOAT_RIGHT, FLOAT_LEFT,
		BUILD_RIGHT, BUILD_LEFT, END_BUILD,
		MINE_RIGHT, MINE_LEFT,
		DIGGING, 
		DRWONING,
		SQUASHED*/
	};

	enum LemmingAnim
	{
		WALKING_RIGHT_ANIM, WALKING_LEFT_ANIM,
		FALLING_RIGHT_ANIM, FALLING_LEFT_ANIM,
		STOPPED_ANIM = 0,
		EXPLODING_ANIM = 0,
		BASH_RIGHT_ANIM=0, BASH_LEFT_ANIM=1,
		/*CLIMBING_ANIM,
		END_CLIMBING_ANIM,
		START_FLOAT_RIGHT_ANIM, START_FLOAT_LEFT_ANIM,
		FLOAT_RIGHT_ANIM, FLOAT_LEFT_ANIM,
		BUILD_RIGHT_ANIM, BUILD_LEFT_ANIM, END_BUILD_ANIM,
		MINE_RIGHT_ANIM, MINE_LEFT_ANIM,
		DIGGING_ANIM,
		DRWONING_ANIM,
		SQUASHED_ANIM*/
	};

	bool pressedKey = false;

	bool _canClimb = false;
	bool _canFloat = false;
	bool _dead = false;
	int _framesFromStart = 0; //frames from the start of some animation, useful when building, exploding, etc.
	LemmingState _state;
	Texture _spritesheet;
	Sprite *_sprite;
	VariableTexture *_mask;
	ShaderProgram _shaderProgram;
};


#endif // _LEMMING_INCLUDE


