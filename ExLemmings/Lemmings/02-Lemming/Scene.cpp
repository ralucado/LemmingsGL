#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

//hacer esta clase virtual?

Scene::Scene(){
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}

double pit_distance(int x1, int y1, int x2, int y2){
	return sqrt(double(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}



void Scene::init(string filenameMap, string filenameMask, const glm::vec2& positionEntry, const glm::vec2& positionExit, const glm::vec2& positionLemmings)
{
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT))};
	//glm::vec2 texCoords[2] = {glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f)};
	glm::vec2 texCoords[2] = {glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f)};

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile(filenameMap, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	tileTexture.loadFromFile("images/sand.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tileTexture.setMinFilter(GL_NEAREST);
	tileTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(filenameMask, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	
	lemming.init(positionLemmings, simpleTexProgram);
	lemming.setMapMask(&maskTexture);

	//button.init(glm::vec2(60, 30), "images/lemming.png", simpleTexProgram);
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	lemming.update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture, tileTexture);
	
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniform1f("time", currentTime);
	lemming.render();
	//button.render();
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	//button.mouseMoved(mouseX, mouseY, bLeftButton);
	if(bLeftButton)
		modifyMask(mouseX, mouseY, false);
	else if(bRightButton)
		modifyMask(mouseX, mouseY, true);
}

void Scene::keyPressed(int key) {
	if (key == 'q') lemming.switchStopper();
	else if (key == 'w') lemming.switchBomber();
	else if (key == 'e') lemming.switchBasher(false);
	else if (key == 'r') lemming.switchBasher(true);
	else if (key == 'a') lemming.switchFloater();
	else if (key == 's') lemming.revive();
	else if (key == 'd') lemming.switchDigger();
	else if (key == 'c') lemming.switchClimber();
	else if (key == 'v') lemming.switchBuilder(false);
	else if (key == 'b') lemming.switchBuilder(true);
	else if (key == 'n') lemming.switchMiner(false);
	else if (key == 'm') lemming.switchMiner(true);
}

void Scene::keyReleased(int key) {
}

void Scene::modifyMask(int mouseX, int mouseY, bool apply)
{
	int posX, posY, color, radius = 5;
	if (apply) color = 255;
	else color = 0;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 120;
	posY = mouseY / 3;

	for (int y = max(0, posY - radius); y <= min(maskTexture.height() - 1, posY + radius); y++)
		for (int x = max(0, posX - radius); x <= min(maskTexture.width() - 1, posX + radius); x++){
		if (Utils::instance().pit_distance(posX, posY, x, y) <= radius) maskTexture.setPixel(x, y, color);
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if(!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if(!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



