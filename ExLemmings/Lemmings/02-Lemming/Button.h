#ifndef _BUTTON_INCLUDE
#define _BUTTON_INCLUDE

#include "Texture.h"
#include "Sprite.h"

#define NUM_ANIM 4


class Button
{
public:
	void init(const glm::vec2 &position, string filename, ShaderProgram &shaderProgram);
	void loadSpritesheet(string filename, const glm::vec2& position);
	void render();
	void mouseMoved(int mouseXInput, int mouseYInput, bool bLeftButton);
	bool isSelected();
	void deselect();
private:

	Texture _spritesheet;
	Sprite *_sprite;

	float posX, posY;
	float buttonHeight, buttonWidth;
	enum ButtonState
	{
		NORMAL,
		MOUSEOVER,
		MOUSEPRESSED,
		SELECTED
	};

	ButtonState _state;
	ShaderProgram _shaderProgram;

};



#endif // _BUTTON_INCLUDE