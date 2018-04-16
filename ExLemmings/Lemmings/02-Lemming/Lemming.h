#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "Utils.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define HEIGHT_TO_FLOAT 25
#define HEIGHT_TO_DIE 40

// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:

	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	//void init(const glm::vec2 &initialPosition, const glm::vec2 &positionExit, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setMapMask(VariableTexture *mapMask);

	void switchStopper();
	void switchFloater();
	void switchClimber();
	void switchBomber();
	void switchWin();
	void switchBasher(bool r);
	glm::vec2 getPosition();
	bool checkAlive();
	
private:
	int collisionFloor(int maxFall);
	int collisionWall(int maxDeep, bool r);
	bool collision();
	bool updateFall();

	void startWalk(bool r);
	void startFloat(bool r);
	void startBash(bool r);
	void startSquish();

	void hole(int posX, int posY, int radius);
	void pop();
	void bashRow(int index, bool r);

	void loadSpritesheet(string filename, int NUM_FRAMES, int NUM_ANIMS, const glm::vec2& position);

	glm::vec2 bashPixels[5] = {glm::vec2(13,6) , glm::vec2(14, 8) , glm::vec2(15, 10) , glm::vec2(14, 12), glm::vec2(14, 14)};

	enum LemmingState
	{
		WALKING_RIGHT, WALKING_LEFT,
		FALLING_RIGHT, FALLING_LEFT,
		STOPPED,
		EXPLODING,
		BASH_RIGHT, BASH_LEFT,
		START_FLOAT_RIGHT, START_FLOAT_LEFT,
		FLOAT_RIGHT, FLOAT_LEFT,
		SQUISHED,
/*		CLIMBING,
		END_CLIMBING,
		BUILD_RIGHT, BUILD_LEFT, END_BUILD,
		MINE_RIGHT, MINE_LEFT,
		DIGGING, 
		DRWONING,
*/
	};

	//We need to use different textures for some animations
	//therefore their indices will be the same
	enum LemmingAnim
	{
		WALKING_RIGHT_ANIM=0, WALKING_LEFT_ANIM=1,
		FALLING_RIGHT_ANIM=2, FALLING_LEFT_ANIM=3,
		STOPPED_ANIM = 0,
		EXPLODING_ANIM = 0,
		BASH_RIGHT_ANIM=0, BASH_LEFT_ANIM=1,
		START_FLOAT_RIGHT_ANIM=0, FLOAT_RIGHT_ANIM=1,
		START_FLOAT_LEFT_ANIM=2, FLOAT_LEFT_ANIM=3,
		SQUISHED_ANIM = 0,
		/*
		CLIMBING_ANIM,
		END_CLIMBING_ANIM,
		BUILD_RIGHT_ANIM, BUILD_LEFT_ANIM, END_BUILD_ANIM,
		MINE_RIGHT_ANIM, MINE_LEFT_ANIM,
		DIGGING_ANIM,
		DRWONING_ANIM,
		*/
	};

	bool pressedKey = false;

	bool _canClimb = false;
	bool _canFloat = false;
	bool _dead = false;

	bool _win = false;
	glm::vec2 _positionExit;

	int _framesFromStart = 0; //frames from the start of some animation, useful when building, exploding, etc.
	int _fallenDistance = 0;
	LemmingState _state;
	Texture _spritesheet;
	Sprite *_sprite;
	VariableTexture *_mask;
	ShaderProgram _shaderProgram;
};


#endif // _LEMMING_INCLUDE


