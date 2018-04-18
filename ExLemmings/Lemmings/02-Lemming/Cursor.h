#ifndef _CURSOR_INCLUDE
#define _CURSOR_INCLUDE

#include "Sprite.h"
#include "VariableTexture.h"
#include "Utils.h"

class Cursor
{

public:

	void init( ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void setPosition(glm::vec2 position);

	void setActive();
	void setNormal();

private:


	void loadSpritesheet(string filename, int NUM_FRAMES, int NUM_ANIMS, const glm::vec2& position, int speed);

	enum CursorState
	{
		NORMAL,
		ACTIVE
	};

	//We need to use different textures for some animations
	//therefore their indices will be the same
	enum CursorAnim
	{
		NORMAL_ANIM, ACTIVE_ANIM
	};

	int _framesFromStart = 0; //frames from the start of some animation, useful when building, exploding, etc.

	CursorState _state;
	Texture _spritesheet;
	Sprite *_sprite;
	ShaderProgram _shaderProgram;
	float _dispX;
	float _dispY;
};


#endif // _CURSOR_INCLUDE


