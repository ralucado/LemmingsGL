#include "Menu.h"
#include <glm/gtc/matrix_transform.hpp>


void Menu::init(string background, glm::vec2 geom[2], string buttonSprites[], glm::vec2 buttonPositions[], int num_buttons)
{

	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	initShaders();
	
	backgroundQuad = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, simpleTexProgram);
	colorTexture.loadFromFile(background, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);

	for (int i = 0; i < num_buttons; i++) {
		Button *b = new Button;
		b->init(buttonPositions[i], buttonSprites[i], simpleTexProgram);
		buttons.push_back(b);
	}
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

}


void Menu::initText(string textString, glm::vec2 textPositions, int size, const glm::vec4 color) {
	
	_textString.push_back(textString);
	_textPositions.push_back(textPositions*glm::vec2(3,3));
	_textSize.push_back(size);
	_textColor.push_back(color);
	Text *t = new Text;
	texts.push_back(t);
	if (!texts[texts.size()-1]->init("fonts/PressStart2P.ttf"))
		cout << "Could not load font!!!" << endl;
	
}

void Menu::updateText(int i, string textString) {
	_textString[i] = textString;
}

void Menu::updateColor(int i, const glm::vec4 color) {
	_textColor[i] = color;
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if (!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram2.init();
	simpleTexProgram2.addShader(vShader);
	simpleTexProgram2.addShader(fShader);
	simpleTexProgram2.link();
	if (!simpleTexProgram2.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram2.log() << endl << endl;
	}
	simpleTexProgram2.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

}

void Menu::render()
{
	glm::mat4 modelview;

	simpleTexProgram2.use();
	simpleTexProgram2.setUniformMatrix4f("projection", projection);
	simpleTexProgram2.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram2.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram2.setUniform1f("time", currentTime);
	backgroundQuad->render(colorTexture);
	
	
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniform1f("time", currentTime);
	

	for (int i = 0; i < buttons.size(); i++)
		buttons[i]->render();
	
	for (int i = 0; i < texts.size(); i++)
		texts[i]->render(_textString[i], _textPositions[i], _textSize[i], _textColor[i]);
	


}

void Menu::mouseMoved(int mouseX, int mouseY, bool bLeftButton)
{
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->mouseMoved(mouseX, mouseY, bLeftButton);
	}
}

void Menu::mouseReleased(int mouseX, int mouseY) {
	int buttonSelected = buttonPressed();
	for (int i = 0; i < buttons.size(); i++) {
		if (!(buttons[i]->mouseReleased(mouseX, mouseY))) {
			buttons[i]->mouseMoved(mouseX, mouseY, false);
			if (buttons[i]->isSelected())
				if (buttonSelected != -1 && buttonSelected != i)
					buttons[buttonSelected]->deselect();
		}
	}
}

int Menu::buttonPressed() 
{
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i]->isSelected()) 
			return i;
		
	}
	return -1;
}

Menu::~Menu() {
	for (int i = 0; i < texts.size(); i++)
		delete texts[i];
	for (int i = 0; i < buttons.size(); i++)
		delete buttons[i];
}

