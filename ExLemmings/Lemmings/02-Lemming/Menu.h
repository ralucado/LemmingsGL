#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Button.h"



class Menu
{
public:
	~Menu();
	void init(string background, string button[],  glm::vec2 buttonPositions[]);
	void render();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton);
	int buttonPressed();

private:
	vector<Button*> buttons;
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

