#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "Utils.h"

#define FALL_STEP 4
#define HEIGHT_TO_FLOAT 25
#define HEIGHT_TO_DIE 50

// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:

	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);

	void update(int deltaTime, glm::vec2 disp);

	void render();
	
	void setMapMask(VariableTexture *mapMask);

	bool switchStopper();
	bool switchFloater();
	bool switchClimber();
	bool switchBomber();
	bool switchBasher();
	bool switchDigger();
	bool switchBuilder();
	bool switchMiner();
	void revive();
	void switchWin();
  
	glm::vec2 getPosition();
	bool checkAlive();


	
private:
	void updateFalling();
	void updateStartFloating();
	void updateFloating();
	void updateWalking();
	void updateBash();
	void updateClimb();
	void updateEndClimb();
	void updateBuild();
	void updateEndBuild();
	void updateMine();
	void updateDig();

	int collisionFloor(int maxFall, int x, int y);
	int collisionWall(int maxDeep, bool r, glm::ivec2 posBase);
	bool collision();
	bool calculateFall();

	void startWalk();
	void startFall();
	void startStop();
	void startPop();
	void startFloat();
	void startBash();
	void startSquish();
	void startDig();
	void startClimb();
	void endClimb();
	void startBuild();
	void endBuild();
	void startMine();
	void startWin();

	void hole(int posX, int posY, int radius);
	void pop();
	void bashRow(int index);

	void digRow();

	void mineRow();

	void paintStep(bool r);

	void blockCells();

	void die();

	bool grounded();
	bool acceptsPower();
	void loadSpritesheet(string filename, int NUM_FRAMES, int NUM_ANIMS, const glm::vec2& position, int speed);

	glm::vec2 bashPixels[5] = {glm::vec2(13,6) , glm::vec2(14, 8) , glm::vec2(15, 10) , glm::vec2(14, 12), glm::vec2(14, 14)};

	enum LemmingState
	{
		WALKING,
		FALLING,
		STOPPED,
		EXPLODING,
		BASH,
		START_FLOAT,
		FLOAT,
		SQUISHED,
		DIGGING, 
		CLIMB,
		END_CLIMB,
		BUILD,
		END_BUILD,
		MINE,
		WIN,
/*		
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
		DIGGING_ANIM = 0,
		CLIMB_RIGHT_ANIM = 0, CLIMB_LEFT_ANIM = 1,
		END_CLIMB_RIGHT_ANIM = 2, END_CLIMB_LEFT_ANIM = 3,
		BUILD_RIGHT_ANIM = 0, BUILD_LEFT_ANIM = 1, END_BUILD_ANIM = 2,
		MINE_RIGHT_ANIM = 0, MINE_LEFT_ANIM = 1,
		WIN_ANIM = 0,
		/*
		DRWONING_ANIM,
		*/
	};

	bool pressedKey = false;
	float DISPLACEMENT = 120;

	bool _canClimb = false;
	bool _canFloat = false;
	bool _dead = false;
	bool _win = false;
	glm::vec2 _positionExit;
	int _builtSteps = 0;
	int _framesFromStart = 0; //frames from the start of some animation, useful when building, exploding, etc.
	int _fallenDistance = 0;
	bool _dir = true;
	LemmingState _state;
	Texture _spritesheet;
	Sprite *_sprite;
	VariableTexture *_mask;
	ShaderProgram _shaderProgram;
	float _dispX;
	float _dispY;
};


#endif // _LEMMING_INCLUDE


