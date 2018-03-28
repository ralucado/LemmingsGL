#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


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

	void makeStopper(bool b);

	void makeBomber(bool b);
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	
private:
	enum LemmingState
	{
		WALKING_RIGHT, WALKING_LEFT,
		FALLING_RIGHT, FALLING_LEFT,
		STOPPED,
		EXPLODING,
/*		CLIMBING,
		END_CLIMBING,
		START_FLOAT_RIGHT, START_FLOAT_LEFT,
		FLOAT_RIGHT, FLOAT_LEFT,
		BUILD_RIGHT, BUILD_LEFT, END_BUILD,
		BASH_RIGHT, BASH_LEFT,
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
		/*CLIMBING_ANIM,
		END_CLIMBING_ANIM,
		START_FLOAT_RIGHT_ANIM, START_FLOAT_LEFT_ANIM,
		FLOAT_RIGHT_ANIM, FLOAT_LEFT_ANIM,
		BUILD_RIGHT_ANIM, BUILD_LEFT_ANIM, END_BUILD_ANIM,
		BASH_RIGHT_ANIM, BASH_LEFT_ANIM,
		MINE_RIGHT_ANIM, MINE_LEFT_ANIM,
		DIGGING_ANIM,
		DRWONING_ANIM,
		SQUASHED_ANIM*/
	};

	bool _canClimb = false;
	bool _canFloat = false;
	LemmingState _state;
	Texture _spritesheet;
	Sprite *_sprite;
	VariableTexture *_mask;
	ShaderProgram _shaderProgram;
};


#endif // _LEMMING_INCLUDE


