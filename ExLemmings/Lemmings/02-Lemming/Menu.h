#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Button.h"

#define NUM_BUTTONS_MAIN 1

class Menu
{
public:
	void init(string background);
	void render();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton);

private:
	Button buttons[NUM_BUTTONS_MAIN];
	Texture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	bool _finished;
	glm::mat4 projection;
	void initShaders();

};

#endif // _MENU_INCLUDE

