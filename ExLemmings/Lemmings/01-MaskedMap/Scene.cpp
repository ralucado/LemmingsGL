#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"

double pit_distance(int x1, int y1, int x2, int y2){
	return sqrt(double(pow(x1 - x2, 2) + pow(y1 - y2, 2)));
}

Scene::Scene()
{
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}


void Scene::init()
{
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT))};
	glm::vec2 texCoords[2] = {glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f)};

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	colorTexture.loadFromFile("images/fun1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	sandTexture.loadFromFile("images/texSand.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sandTexture.setMinFilter(GL_NEAREST);
	sandTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/fun1_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	map->render(texProgram, colorTexture, maskTexture);
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if(bLeftButton)
		eraseMask(mouseX, mouseY);
	else if(bRightButton)
		applyMask(mouseX, mouseY);
}

void Scene::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;
	int radius = 5;
	posX = mouseX / 3 + 120;
	posY = mouseY / 3;
	for (int y = max(0, posY - radius); y <= min(maskTexture.height() - 1, posY + radius); y++)
	for (int x = max(0, posX - radius); x <= min(maskTexture.width() - 1, posX + radius); x++){
		if (pit_distance(posX, posY, x, y) <= radius) maskTexture.setPixel(x, y, 0);
	}
}

void Scene::applyMask(int mouseX, int mouseY)
{
	int posX, posY;
	int radius = 3;
	posX = mouseX/3 + 120;
	posY = mouseY/3;
	for(int y=max(0, posY-radius); y<=min(maskTexture.height()-1, posY+radius); y++)
	for (int x = max(0, posX - radius); x <= min(maskTexture.width() - 1, posX + radius); x++){
		if(pit_distance(mouseX, mouseY, x, y) <= radius) maskTexture.setPixel(x, y, 255);
	}

}

void Scene::initShaders()
{
	Shader vShader, fShader;

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
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



