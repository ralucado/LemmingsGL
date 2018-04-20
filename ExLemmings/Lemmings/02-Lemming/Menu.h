#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Button.h"
#include "Text.h"




class Menu
{
public:
	~Menu();
	void init(string background, glm::vec2 geom2[2], string button[],  glm::vec2 buttonPositions[], int num_buttons);
	void initText(string textString, glm::vec2 textPositions, int size, const glm::vec4 color);
	void updateText(int i, string textString);
	void updateColor(int i, const glm::vec4 color);
	void render();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton);
	void mouseReleased(int mouseX, int mouseY);
	int buttonPressed();

private:
	vector<Button*> buttons;

	vector<Text*> texts;
	vector<string> _textString;
	vector<glm::vec2> _textPositions;
	vector<glm::vec4> _textColor;
	vector<int> _textSize;

	Texture colorTexture;
	MaskedTexturedQuad *backgroundQuad;
	ShaderProgram simpleTexProgram, simpleTexProgram2;
	float currentTime;
	bool _finished;
	glm::mat4 projection;
	void initShaders();

};

#endif // _MENU_INCLUDE

