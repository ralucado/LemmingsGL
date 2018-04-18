#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

Scene::Scene(){
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}

void Scene::loadSpritesheet(string filename, int NUM_FRAMES, int NUM_ANIMS, const glm::vec2& position, Sprite*& _sprite, Texture& _texture) {
	_texture.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	_texture.setMinFilter(GL_NEAREST);
	_texture.setMagFilter(GL_NEAREST);
	_sprite = Sprite::createSprite(glm::ivec2(_texture.width() / NUM_FRAMES, _texture.height() / NUM_ANIMS), glm::vec2(1.0f / float(NUM_FRAMES), 1.0f / float(NUM_ANIMS)), &_texture, &simpleTexProgram);
	_sprite->setNumberAnimations(NUM_ANIMS);
	int speed = 12;
	for (int i = 0; i < NUM_ANIMS; i++) {
		_sprite->setAnimationSpeed(i, speed);
	}
	float height = 1.0f / float(NUM_ANIMS);
	for (int frame = 0; frame < NUM_FRAMES; frame++) {
		float num_frame = float(frame) / float(NUM_FRAMES);
		for (int anim = 0; anim < NUM_ANIMS; anim++) {
			//_sprite->addKeyframe(anim, glm::vec2(num_frame, float(anim)*height + 0.5f / _texture.height()));
			_sprite->addKeyframe(anim, glm::vec2(num_frame, float(anim)*height));

		}
	}
	_sprite->setPosition(position);
}


void Scene::init(string filenameMap, string filenameMask, const glm::vec2& positionEntry, const glm::vec2& positionExit, const glm::vec2& positionLemmings, const glm::vec2& ttSize)
{
	_finished = false;
	_disp.x = 0;
	_disp.y = 0;
	textureTrueSize = ttSize;
	_clicked = false;
	//mapa
	colorTexture.loadFromFile(filenameMap, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	//mask
	maskTexture.loadFromFile(filenameMask, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);
	//tile
	tileTexture.loadFromFile("images/sand.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tileTexture.setMinFilter(GL_NEAREST);
	tileTexture.setMagFilter(GL_NEAREST);

	//coords mapa
	_texCoords[0] = glm::vec2(_disp.x / colorTexture.width(), _disp.y / colorTexture.height());
	_texCoords[1] = glm::vec2((_disp.x + float(CAMERA_WIDTH)) / colorTexture.width(), (_disp.y + float(CAMERA_HEIGHT)) / colorTexture.height());

	_geom[0] = glm::vec2(0.f, 0.f);
	_geom[1] = glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT));

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(_geom, _texCoords, maskedTexProgram);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	//cursor
	cursor.init(simpleTexProgram);
	
	//lemmings
	for (int i = 0; i < NUM_LEMMINGS; i++) {
		lemmings[i] = new Lemming;
		lemmings[i]->init(positionLemmings, simpleTexProgram);
		lemmings[i]->setMapMask(&maskTexture);
	}


	//exit
	exit.init(positionExit, simpleTexProgram);
	exit.setMapMask(&maskTexture);

	//entry
	entry.init(positionEntry, simpleTexProgram);
	entry.setMapMask(&maskTexture);

}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	//coords mapa
	_texCoords[0] = glm::vec2(_disp.x / colorTexture.width(), _disp.y / colorTexture.height());
	_texCoords[1] = glm::vec2((_disp.x + float(CAMERA_WIDTH)) / colorTexture.width(), (_disp.y + float(CAMERA_HEIGHT)) / colorTexture.height());
	map = MaskedTexturedQuad::createTexturedQuad(_geom, _texCoords, maskedTexProgram);
	//exit
	exit.update(deltaTime, _disp);
	//entry
	entry.update(deltaTime, _disp);
	//cursor
	cursor.update(deltaTime);
    //lemmings
	bool finished = true;
	for (int i = 0; i < NUM_LEMMINGS; i++) {
		if (lemmings[i]->checkAlive()) {
			finished = false;
			if (lemmings[i]->getPosition() == exit.getBasePosition())
				lemmings[i]->switchWin();
			lemmings[i]->update(deltaTime, _disp);
		}
	}
	_finished = finished;
}

void Scene::render()
{
	//shaders
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

	//exit
	exit.render();

	//entry
	entry.render();

	//lemmings
	for (int i = 0; i < NUM_LEMMINGS; i++) {
		lemmings[i]->render();
	}

	//cursor
	cursor.render();
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	//update cursor position
	cursor.setPosition(glm::vec2(mouseX/3-8, mouseY/3-8));
	//modify mask
	if (Game::instance().getKey(32)) { //space
		if(bLeftButton)
			modifyMask(mouseX, mouseY, false);
		else if(bRightButton)
			modifyMask(mouseX, mouseY, true);
	}
	else {
		if (bLeftButton) {
		    //update scrolling
			if (_clicked) {
				//disp = clickOrigin - mouse
				float dx = (_clickOrigin.x - mouseX / 3) + _disp.x;
				float dy = (_clickOrigin.y - mouseY / 3) + _disp.y;
				if(dx >= 0 && dx < textureTrueSize.x - CAMERA_WIDTH) _disp.x = dx;
				if(dy >= 0 && dy < textureTrueSize.y - CAMERA_HEIGHT) _disp.y = dy;
				_clickOrigin.x = mouseX / 3;
				_clickOrigin.y = mouseY / 3;

			}
			//start scrolling
			else if(maskTexture.pixel(mouseX / 3, mouseY / 3) == 255) {
				//first click, set click origin
				_clicked = true;
				_clickOrigin.x = mouseX/3;
				_clickOrigin.y = mouseY/3;
			}
		}
		//stop scrolling
		else if (_clicked){
			_clicked = false;
		}
	} 

}

void Scene::mouseLeftPressed(int mouseX, int mouseY)
{
	mouseX = mouseX / 3;
	mouseY = mouseY / 3;
	//update clicked lemming
	for (int i = 0; i < NUM_LEMMINGS; i++) {
		//calc 0,0 position from base position
		glm::vec2 lemPos = lemmings[i]->getPosition() - glm::vec2(7, 16);
		if (mouseX >= lemPos.x && mouseX <= lemPos.x + 16 && mouseY >= lemPos.y && mouseY <= lemPos.y + 16) {
			cout << "clicked lemming " << i << endl;
			break;
		}
	}
}

void Scene::keyPressed(int key) {
	if (key == 'q') lemmings[0]->switchStopper();
	else if (key == 'w') lemmings[0]->switchBomber();
	else if (key == 'e') lemmings[0]->switchBasher();
	else if (key == 'f') lemmings[0]->switchFloater();
	else if (key == 's') lemmings[0]->revive();
	else if (key == 'd') lemmings[0]->switchDigger();
	else if (key == 'c') lemmings[0]->switchClimber();
	else if (key == 'b') lemmings[0]->switchBuilder();
	else if (key == 'm') lemmings[0]->switchMiner();
    else if (key == 'y') lemmings[0]->switchWin();
}

void Scene::keyReleased(int key) {
}

void Scene::modifyMask(int mouseX, int mouseY, bool apply)
{
	int posX, posY, color, radius = 5;
	if (apply) color = 255;
	else color = 0;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced
	posX = mouseX / 3 + _disp.x;
	posY = mouseY / 3 + _disp.y;

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

bool Scene::checkFinished() {
	return _finished;
}
