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
	for (int i = 0; i < lemmings.size(); i++) {
		if (lemmings[i] != NULL) {
			delete lemmings[i];
		}
	}
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
			_sprite->addKeyframe(anim, glm::vec2(num_frame, float(anim)*height));
		}
	}
	_sprite->setPosition(position);
}


void Scene::init(string filenameMap, string filenameMask, const glm::vec2& positionEntry, const glm::vec2& positionExit, const glm::vec2& ttSize, int powerCount[], int iniLemmings, int finLemmings, int time, int lvl)
{
	_finished = false;
	_nuke = false;
	lemmingsSaved = 0;
	lemmingsDead = 0;
	_totalLemmings = iniLemmings;
	_targetLemmings = finLemmings;
	_levelTime = time;
	_levelNum = lvl;
	_spawnTime = 0.f;
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

	//update available powers count
	_powerCount = vector<int>(NUM_POWERS, 0);
	for (int i = 0; i < _powerCount.size(); ++i) {
		_powerCount[i] = powerCount[i];
	}

	//cursor
	cursor.init(simpleTexProgram);
	
	//menus
	initMenus();
	
	//exit
	exit.init(positionExit, simpleTexProgram);
	exit.setMapMask(&maskTexture);

	//entry
	_positionEntry = positionEntry;
	entry.init(positionEntry, simpleTexProgram);
	entry.setMapMask(&maskTexture);

	//lemmings
	for (int i = 0; i < lemmings.size(); ++i) {
		delete lemmings[i];
	}
	lemmings = vector<Lemming*>();
	

}

void Scene::initMenus() {
	menuPowers = Menu();
	menuControl = Menu();
	for (int i = 0; i < NUM_POWERS; i++) 
		menuPowersButtonPos[i] = glm::vec2(geomMenuPowers[1][0] * (i / float(NUM_POWERS)), geomMenuPowers[0][1]);

	for (int i = 0; i < NUM_BUTTONS; i++)
		menuControlButtonPos[i] = glm::vec2((geomMenuControl[1][0] - geomMenuControl[0][0]) * (i / float(NUM_BUTTONS + 2)) + geomMenuControl[0][0], geomMenuControl[0][1]);

	menuPowers.init(menuPowersBackground, geomMenuPowers, menuPowersButtonSprite, menuPowersButtonPos, NUM_POWERS);
	menuControl.init(menuControlBackground, geomMenuControl, menuControlButtonSprite, menuControlButtonPos, NUM_BUTTONS);

	// Info Level
	for (int i = 0; i < 5; i++) 
		menuControl.initText(textString[i], glm::vec2(float(CAMERA_WIDTH)*(11.f / 14.f)+10.f, float(CAMERA_HEIGHT) - (28.f - 5.f*(i+1))), 12, glm::vec4(1, 1, 1, 1));

	menuControl.updateText(3, "MIN: " + to_string(_targetLemmings));
	menuControl.updateText(2, "SAVED: " + to_string(0));

	for (int i = 0; i < NUM_POWERS; i++)
		menuControl.initText(to_string(_powerCount[i]), (menuPowersButtonPos[i] + glm::vec2(6, 28)), 16, glm::vec4(1, 1, 1, 1));
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{

	if (menuControl.buttonPressed() != 0) { //pause
		if (menuControl.buttonPressed() == 1) deltaTime *= 2; //fast
		currentTime += deltaTime;
		_spawnTime += deltaTime;
		//lemmings
		if (_spawnTime >= 2000 && lemmings.size() < _totalLemmings && !_nuke) {
			int i = lemmings.size();
			glm::vec2 positionLemmings = glm::vec2(_positionEntry.x + 13, _positionEntry.y);
			lemmings.push_back(new Lemming);
			lemmings[i]->init(positionLemmings, lemmingTexProgram, &_blockers);
			lemmings[i]->setMapMask(&maskTexture);
			_spawnTime = 0.f;
			menuControl.updateText(1, "OUT: " + to_string(lemmings.size() - (lemmingsSaved + lemmingsDead)));
		}


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
		bool finished = (lemmings.size() > 0);
		int lemmingsDeadAUX = 0;
		for (int i = 0; i < lemmings.size(); i++) {
			if (lemmings[i]->checkActive()) {
				finished = false;
				if (hodor(i)) { //sorry
					lemmings[i]->switchWin();
				}
				lemmings[i]->update(deltaTime, _disp);
			}
			else if (lemmings[i]->checkDead()) {
				lemmingsDeadAUX++;
			}
			else if (lemmings[i]->getWin()) {
				cout << "win " << i << endl;
				lemmingsSaved++;
				menuControl.updateText(1, "OUT: " + to_string(lemmings.size()-(lemmingsSaved+lemmingsDead)));
				menuControl.updateText(2, "SAVED: " + to_string(lemmingsSaved));
			}
		}
		if (lemmingsDeadAUX != lemmingsDead) {
			lemmingsDead = lemmingsDeadAUX;
			menuControl.updateText(1, "OUT: " + to_string(lemmings.size() - (lemmingsSaved + lemmingsDead)));
		}

		_activePower = Power(menuPowers.buttonPressed());
		if (menuControl.buttonPressed() == 2) { //nuke
			_nuke = true;
			for (int i = 0; i < lemmings.size(); i++)
				lemmings[i]->forceBomber();
		}
		_finished = finished;
	}

	// Time
	int levelTime;
	if ((currentTime / 1000) > 1) levelTime = _levelTime - ((currentTime / 1000) - 2);
	else levelTime = _levelTime;
	int minutes = levelTime / 60;
	int seconds = levelTime % 60;
	if (levelTime < 30)
		menuControl.updateColor(3, glm::vec4(1, 0.2f, 0.2f, 1));
	if (seconds < 10)
		menuControl.updateText(4, "TIME: " + to_string(minutes) + ":0" + to_string(seconds));
	else 
		menuControl.updateText(4, "TIME: " + to_string(minutes) + ":" + to_string(seconds));
	
	if (levelTime < 1)
		_finished = true;

}

void Scene::render()
{
	//shaders
	glm::mat4 modelview;
	modelview = glm::mat4(1.0f);

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniform1f("time", currentTime);

	//entry
	entry.render();
	//exit
	exit.render();
	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture, tileTexture);
	
	lemmingTexProgram.use();
	lemmingTexProgram.setUniformMatrix4f("projection", projection);
	lemmingTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	lemmingTexProgram.setUniformMatrix4f("modelview", modelview);
	//lemmings
	for (int i = 0; i < lemmings.size(); i++) {
		lemmings[i]->render();
	}
	//cursor
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniform1f("time", currentTime);
	cursor.render();
	//menus
	menuPowers.render();
	menuControl.render();
	//cursor
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	simpleTexProgram.setUniform1f("time", currentTime);
	cursor.render();
}


void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
  //update cursor position
	cursor.setPosition(glm::vec2(mouseX/3-8, mouseY/3-8));
  if (!(((mouseX / 3) > geomMenuPowers[0].x && (mouseX / 3) < geomMenuControl[1].x) &&
		((mouseY / 3) > geomMenuPowers[0].y && (mouseY / 3) < geomMenuControl[1].y))){
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
	menuPowers.mouseMoved(mouseX, mouseY, bLeftButton);
	menuControl.mouseMoved(mouseX, mouseY, bLeftButton);
}

void Scene::mouseReleased(int mouseX, int mouseY) {
	menuPowers.mouseReleased(mouseX, mouseY);
	menuControl.mouseReleased(mouseX, mouseY);
}

void Scene::mouseLeftPressed(int mouseX, int mouseY)
{
	mouseX = mouseX / 3;
	mouseY = mouseY / 3;
	//update clicked lemming
	for (int i = 0; i < lemmings.size(); i++) {
		//calc 0,0 position from base position
		glm::vec2 lemPos = lemmings[i]->getPosition() - glm::vec2(7, 16);
		if (mouseX >= lemPos.x && mouseX <= lemPos.x + 16 && mouseY >= lemPos.y && mouseY <= lemPos.y + 16) {
			cout << "clicked lemming " << i << endl;
			if (_activePower >= 0 && _activePower < _powerCount.size() && _powerCount[_activePower] > 0) {
				givePower(i);
			}
			break;
		}
	}
}

void Scene::keyPressed(int key) {
	if (key == 'q') _activePower = BLOCK;
	else if (key == 'w') _activePower = BOMB;
	else if (key == 'e') _activePower = BASH;
	else if (key == 'f') _activePower = FLOAT;
	else if (key == 'd') _activePower = DIG;
	else if (key == 'c') _activePower = CLIMB;
	else if (key == 'b') _activePower = BUILD;
	else if (key == 'm') _activePower = MINE;
	//godmode
	else if (key == 's') lemmings[0]->revive();
    else if (key == 'y') lemmings[0]->switchWin();
}

void Scene::givePower(int i) {
	bool success = false;
	switch (_activePower) {
	case BLOCK:
		success = lemmings[i]->switchStopper();
		break;
	case BOMB:
		success = lemmings[i]->switchBomber();
		break;
	case BASH:
		success = lemmings[i]->switchBasher();
		break;
	case FLOAT:
		success = lemmings[i]->switchFloater();
		break;
	case DIG:
		success = lemmings[i]->switchDigger();
		break;
	case CLIMB:
		success = lemmings[i]->switchClimber();
		break;
	case BUILD:
		success = lemmings[i]->switchBuilder();
		break; 
	case MINE:
		success = lemmings[i]->switchMiner();
		break;
	default:
		break;
	}
	if (success) {
		--_powerCount[_activePower];
		menuControl.updateText(_activePower + 5, to_string(_powerCount[_activePower]));
		if (_powerCount[_activePower] == 0)
			menuControl.updateColor(_activePower + 5, glm::vec4(0.6f, 0.6f, 0.6f, 1));
	}
}

void Scene::keyReleased(int key) {
}

bool Scene::hodor(int i) {
	int X = lemmings[i]->getPosition().x; int Y = lemmings[i]->getPosition().y;
	int X1 = exit.getBasePosition().x; int Y1 = exit.getBasePosition().y;
	return (X > X1 - 5 && X < X1 + 5 && Y > Y1 - 7 && Y < Y1 + 3);
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

bool Scene::checkFinished() { return _finished; }
bool Scene::checkWin() { return lemmingsSaved >= _targetLemmings; }

int Scene::getSaved() { return lemmingsSaved; }
int Scene::getTotal() { return _totalLemmings; }
int Scene::getMin() { return _targetLemmings; }

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

	vShader.initFromFile(VERTEX_SHADER, "shaders/textureLemming.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/textureLemming.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	lemmingTexProgram.init();
	lemmingTexProgram.addShader(vShader);
	lemmingTexProgram.addShader(fShader);
	lemmingTexProgram.link();
	if (!lemmingTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << lemmingTexProgram.log() << endl << endl;
	}
	lemmingTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

}
