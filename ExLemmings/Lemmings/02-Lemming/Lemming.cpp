#include <cmath>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


bool Lemming::grounded()
{
	return( _state != FLOAT && _state != START_FLOAT && _state != FALLING && _state != SQUISHED && _state != WIN && _state != EXPLODING);
}

bool Lemming::acceptsPower() {
	if (grounded() && _state != STOPPED) {
		return true;
	}
	return false;
}

void Lemming::loadSpritesheet(string filename, int NUM_FRAMES,  int NUM_ANIMS, const glm::vec2& position, int speed) {
	_spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	_spritesheet.setMinFilter(GL_NEAREST);
	_spritesheet.setMagFilter(GL_NEAREST);
	_sprite = Sprite::createSprite(glm::ivec2(_spritesheet.width() / NUM_FRAMES, _spritesheet.height() / NUM_ANIMS), glm::vec2(1.0f / float(NUM_FRAMES), 1.0f / float(NUM_ANIMS)), &_spritesheet, &_shaderProgram);
	_sprite->setNumberAnimations(NUM_ANIMS);
	for (int i = 0; i < NUM_ANIMS; i++) {
		_sprite->setAnimationSpeed(i, speed);
	}
	float height = 1.0f / float(NUM_ANIMS);
	for (int frame = 0; frame < NUM_FRAMES; frame++) {
		float num_frame = float(frame) / float(NUM_FRAMES);
		for (int anim = 0; anim < NUM_ANIMS; anim++) {
			//_sprite->addKeyframe(anim, glm::vec2(num_frame, float(anim)*height + 0.5f / _spritesheet.height()));
			_sprite->addKeyframe(anim, glm::vec2(num_frame, float(anim)*height));

		}
	}
	_sprite->setPosition(position);
}

//void Lemming::init(const glm::vec2 &initialPosition, const glm::vec2 &positionExit, ShaderProgram &shaderProgram)
void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, set<pair<int, int>>* blockers, ISoundEngine * engine)
{
	_engine = engine;
	_state = FALLING;
	_shaderProgram = shaderProgram;
	_dir = true;
	_counted = false;
	_win = false;
	_dead = false;
	_blockers = blockers;
	_framesFromStart = 0;
	loadSpritesheet("images/lemming.png", 8, 4, initialPosition, 12);
	_sprite->changeAnimation(FALLING_RIGHT_ANIM);
	_dispX = 0;
	_dispY = 0;
}

void Lemming::update(int deltaTime, glm::vec2 disp)
{
	_currentTime += deltaTime;
	_sprite->position() += glm::vec2(_dispX - disp.x, _dispY - disp.y);
	_dispX = disp.x;
	_dispY = disp.y;
	if (_countdown == 1) {
		_count += deltaTime;
		if (_count >= 3000) startPop();
	}
	if(_dead || _sprite->update(deltaTime) == 0 || _win) return;
	++_framesFromStart;
	checkOutbounds();
	checkBlockers();

	switch(_state)
	{
	case FALLING:
		updateFalling();
		break;
	case FLOAT:
		updateFloating();
		break;
	case START_FLOAT:
		updateStartFloating();
		break;
	case WALKING:
		updateWalking();
		break;
	case BASH:
		updateBash();
		break;
	case CLIMB:
		updateClimb();
		break;
	case END_CLIMB:
		updateEndClimb();
		break;
	case BUILD:
		updateBuild();
		break;
	case END_BUILD:
		updateEndBuild();
		break;
	case MINE:
		updateMine();
		break;
	case DIGGING:
		updateDig();
		break;
	case EXPLODING:
 		if (_framesFromStart == 16) pop();
		break;
	case SQUISHED:
		if (_framesFromStart == 16) die();
	case WIN:
		if (_framesFromStart == 7) _win = true;
		break;
	}
}


void Lemming::render()
{
	if (!_dead && !_win) {

		_shaderProgram.setUniform1f("time", _currentTime);
		_shaderProgram.setUniform1i("act", _countdown);
		_sprite->render();
	}
}


void Lemming::checkBlockers() {
	std::set<pair<int, int> >::iterator it;
	int X = floor(_sprite->position().x + _dispX) + 7;
	int Y = floor(_sprite->position().y + _dispY) + 12;
	for (it = _blockers->begin(); it != _blockers->end(); ++it) {
		int X1 = it->first, Y1 = it->second;
		bool dir = !(X1 < 0);
		X1 = abs(X1); Y1 = abs(Y1);
		if (X == X1) {
			for (int i = 0; i < 16; ++i) {
				if (Y == Y1 + i) {
					if(_dir != dir) turn(dir);
				}
			}
		}
	}
}

void Lemming::updateFalling() {
	if (_fallenDistance > HEIGHT_TO_FLOAT && _canFloat) {
		startFloat();
	}
	if (!calculateFall()) {
		if (_fallenDistance > HEIGHT_TO_DIE) {
			startSquish();
		}
		else {
			startWalk();
		}
	}
}

void Lemming::updateStartFloating() {
	if (_framesFromStart == 3) {
		_state = FLOAT;
		_sprite->changeAnimation(_dir? FLOAT_RIGHT_ANIM : FLOAT_LEFT_ANIM);
	}
}

void Lemming::updateFloating() {
	if (!calculateFall()) {
		startWalk();
	}
}

void Lemming::updateWalking() {
	int dir = (_dir? 1 : -1);
	bool canDescend = true;
	glm::vec2 ori = _sprite->position(); //save original position
	_sprite->position() += glm::vec2(dir*1, -3); //try to put the lemming up top a 3 pixel climb
	while (collision() && canDescend)
	{
		//while the position is a collision, try a position lower
		_sprite->position() += glm::vec2(0, 1);
		//if we can not move more positions in a single movement
		if (_sprite->position().y - ori.y > 3) canDescend = false;
	}
	if (!canDescend) {
		if (_canClimb) {
			startClimb();
		}
		else {
			_sprite->position() = ori;
			//change direction
			_dir = !_dir;
			_sprite->changeAnimation(_dir ? WALKING_RIGHT_ANIM : WALKING_LEFT_ANIM);
		}
	}
	else
	{
		int fall = collisionFloor(7, 7, 16);
		_sprite->position() += glm::vec2(0, fall);
		if (fall > 6) {
			_state = FALLING;
			_sprite->changeAnimation(_dir ? FALLING_RIGHT_ANIM : FALLING_LEFT_ANIM);
		}
	}
}

void Lemming::updateBash() {
	glm::ivec2 posBase = _sprite->position() + glm::vec2(_dispX, _dispY); // Add the map displacement
	posBase += glm::ivec2(7, 15);
	//check that there is material to dig
	if (collisionWall(12, _dir, posBase) > 7+1*_dir) startWalk();
	_framesFromStart %= 32;

	int dir = (_dir ? 1 : -1);
	if ((_framesFromStart > 10 && _framesFromStart <= 15) ||
		(_framesFromStart > 26 && _framesFromStart <= 31))
		_sprite->position() += glm::vec2(dir*1, 0);
	if (_framesFromStart >= 2 && _framesFromStart <= 6)
		bashRow(_framesFromStart - 2);
	else if (_framesFromStart >= 18 && _framesFromStart <= 22)
		bashRow(_framesFromStart - 18);
}

void Lemming::updateClimb() {
	_framesFromStart %= 8;
	int dir = (_dir ? 1 : -1);
	int x = 7 + int(!_dir);
	glm::ivec2 posBase = _sprite->position() + glm::vec2(_dispX, _dispY); // Add the map displacement
	posBase += glm::vec2(x, 0);
	int col = collisionWall(7, _dir, posBase);
	//pared discontinua hacia fuera
 	if (col > 1) {
		int col2 = collisionWall(10, _dir, posBase + glm::ivec2(0, -4));
		//si hay espacio para subir
		if (col2 > 9) {
			endClimb();
		}
		//si no hay suficiente espacio
		else {
			_sprite->position() += glm::vec2(-dir, -4);
			_dir = !_dir;
			startWalk();
		}
	}
	//pared discontinua hacia dentro
	else if (col == 0) {
		_sprite->position() += glm::vec2(dir*-2, -2);
		_dir = !_dir;
		startWalk();
	}
	if (_framesFromStart > 3) _sprite->position() += glm::vec2(0, -1);
}

void Lemming::updateEndClimb() {
	int dir = (_dir ? 1 : -1);
	if (_framesFromStart < 4) {
		_sprite->position() += glm::vec2(0, -2);
	}
	else if (_framesFromStart >= 4) {
		_sprite->position() += glm::vec2(dir*1, -2);
		if (_framesFromStart == 8) startWalk();
	}
}

void Lemming::updateBuild() {
	int x = (_dir ? 12 : 3);
	int dir = (_dir ? 1 : -1);
	if (_framesFromStart == 1) {
		glm::ivec2 posBase = _sprite->position() + glm::vec2(_dispX, _dispY) + glm::vec2(x, 14);
		int col = collisionWall(3, _dir, posBase);
		if (col < 1) startWalk();
	}
	if (_framesFromStart == 16) {
		_framesFromStart = 0;
		++_builtSteps;
		paintStep(_dir);
		_sprite->position() += glm::vec2(dir * 2, -1);
	}
	if (_builtSteps == 12) endBuild();
}

void Lemming::updateEndBuild() {
	if (_framesFromStart == 7)
		startWalk();
}

void Lemming::updateMine() {
	_framesFromStart %= 24;
	int dir = (_dir ? 1 : -1);
	if (_framesFromStart == 0) {
		_sprite->position() += glm::vec2(0, 1);
	}
	else if (_framesFromStart == 3) {
		_sprite->position() += glm::vec2(0, 1);
	}
	else if (_framesFromStart == 4) {
		mineRow();
		_sprite->position() += glm::vec2(dir * 1, 0);
	}
	else if (_framesFromStart == 15) {
		_sprite->position() += glm::vec2(dir * 2, 0);
		int fall = collisionFloor(2, 7, 15);
		if (fall > 1) {
			startFall();
		}
	}
}

void Lemming::updateDig() {
	_framesFromStart %= 16;
	_sprite->position() += glm::vec2(0, -2);
	int fall = collisionFloor(1, 7, 16);
	_sprite->position() += glm::vec2(0, 2);
	if (fall > 0) {
		startFall();
	}
	else if (_framesFromStart == 7 || _framesFromStart == 15)
	{
		digRow();
		_sprite->position() += glm::vec2(0, 2);
	}



}

void Lemming::turn(bool dir) {
	switch (_state)
	{
	case WALKING:
		//turn
		turnWalk(dir);
		break;
	case BUILD:
		//turn
		turnBuild(dir);
		break;
	case BASH:
	case CLIMB:
	case END_CLIMB:
	case MINE:
		_dir = dir; //turn
		startWalk(); //walk
		break;
	}
}

void Lemming::turnWalk(bool dir) {
	_dir = dir;
	_sprite->changeAnimation((_dir ? WALKING_RIGHT_ANIM : WALKING_LEFT_ANIM));
}

void Lemming::turnBuild(bool dir) {
	_dir = dir;
	_sprite->changeAnimation((_dir ? BUILD_RIGHT_ANIM : BUILD_LEFT_ANIM));
	_framesFromStart = 0;
}


bool Lemming::calculateFall() {
	int fall = collisionFloor(2, 7, 16);
	if (fall > 0) {
		_sprite->position() += glm::vec2(0, fall);
		_fallenDistance += fall;
		return true;
	}
	return false;
}

bool Lemming::switchFloater() {
	if(_canFloat) return false;
	_canFloat = true;
	return true;
}

bool Lemming::switchClimber() {
	if (_canClimb || !acceptsPower()) return false;
	_canClimb = true;
	return true;

}

bool Lemming::switchStopper() {
	if (_state != STOPPED && acceptsPower()) {
		startStop();
		return true;
	}
	return false;
}

bool Lemming::switchBomber() {
	if (grounded()) {
		_countdown = 1;
		return true;
	}
	return false;

}

void Lemming::forceBomber() {
	_countdown = 1;
}


bool Lemming::switchBasher()
{
	if (_state != BASH && acceptsPower()) {
		startBash();
		return true;
	}
	return false;
}


bool Lemming::switchDigger()
{
	if (_state != DIGGING && acceptsPower()) {
		startDig();
		return true;
	}
	return false;
}

bool Lemming::switchBuilder()
{
	if (_state != BUILD && acceptsPower()) {
		startBuild();
		return true;
	}
	return false;
}

bool Lemming::switchMiner()
{
	if (_state != MINE && acceptsPower()) {
		startMine();
		return true;
	}
	return false;
}

void Lemming::switchWin() {
	if (_state != WIN) {
		startWin();
	}
}



void Lemming::startStop() {
	_state = STOPPED;
	loadSpritesheet("images/stopper.png", 16, 1, _sprite->position(), 12);
	_sprite->changeAnimation(STOPPED_ANIM);
	blockCells();
}

void Lemming::startPop() {
	unblockCells();
	loadSpritesheet("images/bomber.png", 16, 1, _sprite->position(), 12);
	_state = EXPLODING;
	_framesFromStart = 0;
	_count = 0;
	_countdown = 2;
	_sprite->changeAnimation(EXPLODING_ANIM);
}

void Lemming::startBash() {
	loadSpritesheet("images/basher.png", 32, 2, _sprite->position(), 12);
	_state = BASH;
	_framesFromStart = 0;
	_sprite->changeAnimation((_dir ? BASH_RIGHT_ANIM : BASH_LEFT_ANIM));
}

void Lemming::startWalk() {
	_state = WALKING;
	_fallenDistance = 0;
	_framesFromStart = 0;
	loadSpritesheet("images/lemming.png", 8, 4, _sprite->position(), 12);
	_sprite->changeAnimation((_dir ? WALKING_RIGHT_ANIM : WALKING_LEFT_ANIM));
}

void Lemming::startFall() {
	_state =FALLING;
	_fallenDistance = 0;
	loadSpritesheet("images/lemming.png", 8, 4, _sprite->position(), 12);
	_sprite->changeAnimation((_dir ? FALLING_RIGHT_ANIM : FALLING_LEFT_ANIM));
}

void Lemming::startFloat() {
	_state = START_FLOAT;
	loadSpritesheet("images/floater.png", 4, 4, _sprite->position(), 8);
	_framesFromStart = 0;
	_sprite->changeAnimation((_dir ? START_FLOAT_RIGHT_ANIM : START_FLOAT_LEFT_ANIM));
}

void Lemming::startSquish() {
	_state = SQUISHED;
	loadSpritesheet("images/squished.png", 16, 1, _sprite->position(), 12);
	_framesFromStart = 0;
	_sprite->changeAnimation(SQUISHED_ANIM);
}

void Lemming::startDig() {
	_state = DIGGING;
	//digging animation is displaced, it shows pixels of the lemming INSIDE of the ground
	//we move 2 squares down to achieve the effect
	_sprite->position() += glm::vec2(0, 2);
	loadSpritesheet("images/digger.png", 16, 1, _sprite->position(), 12);
	_framesFromStart = 0;
	_sprite->changeAnimation(DIGGING_ANIM);
}

void Lemming::startClimb() {
	_state = CLIMB;
	loadSpritesheet("images/climber.png", 8, 4, _sprite->position(), 12);
	_framesFromStart = 0;
	_sprite->changeAnimation((_dir ? CLIMB_RIGHT_ANIM : CLIMB_LEFT_ANIM));
}

void Lemming::endClimb() {
	_state = END_CLIMB;
	_framesFromStart = 0;
	_sprite->changeAnimation((_dir ? END_CLIMB_RIGHT_ANIM : END_CLIMB_LEFT_ANIM));
}

void Lemming::startBuild() {
	loadSpritesheet("images/builder.png", 16, 3, _sprite->position(), 12);
	_state =BUILD;
	_framesFromStart = 0;
	_builtSteps = 0;
	_sprite->changeAnimation((_dir ? BUILD_RIGHT_ANIM : BUILD_LEFT_ANIM));
}


void Lemming::endBuild() {
	_state = END_BUILD;
	_framesFromStart = 0;
	_builtSteps = 0;
	_sprite->changeAnimation(END_BUILD_ANIM);
}

void Lemming::startMine() {
	loadSpritesheet("images/miner.png", 24, 2, _sprite->position(), 12);
	_state = MINE;
	_framesFromStart = 0;
	_sprite->changeAnimation((_dir ? MINE_RIGHT_ANIM : MINE_LEFT_ANIM));
}

void Lemming::startWin() {
	loadSpritesheet("images/win.png", 7, 1, _sprite->position(), 12);
	_state = WIN;
	_framesFromStart = 0;
	_sprite->changeAnimation(WIN_ANIM);
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	_mask = mapMask;
}

int Lemming::collisionFloor(int maxFall, int x, int y)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = _sprite->position() + glm::vec2(_dispX, _dispY); // Add the map displacement
	
	posBase += glm::ivec2(x, y);
	while((fall < maxFall) && !bContact)
	{
		if((_mask->pixel(posBase.x, posBase.y+fall) == 0) && (_mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

int Lemming::collisionWall(int maxDeep, bool r, glm::ivec2 posBase)
{
	bool bContact = false;
	int deep = 0;
	while ((abs(deep) < maxDeep) && !bContact)
	{
		if (_mask->pixel(posBase.x + deep, posBase.y) == 0)
			deep = (r? ++deep : --deep);
		else
			bContact = true;
	}

	return abs(deep);
}


bool Lemming::collision()
{
	glm::ivec2 posBase = _sprite->position() + glm::vec2(_dispX, _dispY); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((_mask->pixel(posBase.x, posBase.y) == 0) && (_mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

void Lemming::hole(int posX, int posY, int radius) {
	for (int y = max(0, posY - radius); y <= min(_mask->height() - 1, posY + radius); y++)
		for (int x = max(0, posX - radius); x <= min(_mask->width() - 1, posX + radius); x++) {
			if (Utils::instance().pit_distance(posX, posY, x, y) <= radius) _mask->setPixel(x, y, 0);
		}
}

void Lemming::pop() {
	_engine->play2D("audio/pop.ogg");
	die();
	// Transform from mouse coordinates to map coordinates
	int posX = floor(_sprite->position().x+7 + _dispX);
	int posY = floor(_sprite->position().y+16 + _dispY);
	hole(posX, posY, 7);
}

void Lemming::bashRow(int index) {
	int displacement = _dispX;
	if (!_dir) displacement += 19; //the sprite width
	glm::ivec2 posBase = _sprite->position() + glm::vec2(displacement, _dispY);
	for (int i = 8; i <= bashPixels[index].x; ++i) {
		int aux = (_dir ? i : -i);
		_mask->setPixel(posBase.x + aux, posBase.y + bashPixels[index].y,     0);
		_mask->setPixel(posBase.x + aux, posBase.y + bashPixels[index].y + 1, 0);
	}
}


glm::vec2 Lemming::getPosition() {
	return _sprite->position() + glm::vec2(7, 16); 
}

bool Lemming::checkDead() {
	return _dead;
}

bool Lemming::getWin()
{
	if (!_counted && _win) {
		_counted = true;
		return true;
	}
	return false;
}

bool Lemming::checkActive() {
	return !(_win || _dead);
}

void Lemming::digRow() {
	int posX = floor(_sprite->position().x + _dispX) + 3;
	int posY = floor(_sprite->position().y + _dispY) + 16;
	for (int i = 0; i < 8; ++i){
		_mask->setPixel(posX + i, posY - 2, 0);
		_mask->setPixel(posX + i, posY - 1, 0);
	}
}

void Lemming::mineRow() {
	int posX = floor(_sprite->position().x + _dispX);
	int posY = floor(_sprite->position().y + _dispY);
	int Xini = (_dir ? 10 : 4);
	int Xfi = Xini + 6;
	for (int i = Xini; i <= Xfi; ++i) {
		for (int j = 5; j <= 15; ++j) {
			int X = posX + i; int Y = posY + j;
			if (!((X == posX + Xini + int(_dir) * 5 && Y == posY + 5) || (X == posX + Xini + int(_dir) * 5 + 1 && Y == posY + 5) ||
				  (X == posX + Xini + int(_dir) * 6 && Y == posY + 6) || (X == posX + Xini + int(_dir) * 6 && Y == posY + 14) ||
				  (X == posX + Xini + int(_dir) * 5 && Y == posY + 15) || (X == posX + Xini + int(_dir) * 5 + 1 && Y == posY + 15))) {
				_mask->setPixel(X, Y, 0);
			}
		}
	}
}

void Lemming::paintStep(bool r) {
	int ini = 2;
	if (r) ini = 10;
	int X = floor(_sprite->position().x + _dispX) + ini;
	int Y = floor(_sprite->position().y + _dispY) + 15;
	_mask->setPixel(X, Y, 255);
	_mask->setPixel(X + 1, Y, 255);
	_mask->setPixel(X + 2, Y, 255);
	_mask->setPixel(X + 3, Y, 255);
}

void Lemming::blockCells() {
	int X = floor(_sprite->position().x + _dispX);
	int Y = floor(_sprite->position().y + _dispY);
	_blockers->insert(make_pair(-(X + 3), -Y));
	_blockers->insert(make_pair(X + 12, Y));
}

void Lemming::unblockCells() {
	int X = floor(_sprite->position().x + _dispX);
	int Y = floor(_sprite->position().y + _dispY);

	set<pair<int, int> >::iterator it1 = _blockers->find(make_pair(-(X + 3), -Y));
	set<pair<int, int> >::iterator it2 = _blockers->find(make_pair(X + 12, Y));
	if (it1 != _blockers->end()) _blockers->erase(make_pair(-(X + 3), -Y));
	if (it2 != _blockers->end()) _blockers->erase(make_pair(X + 12, Y));
}

void Lemming::die() {
	_dead = true;
	_framesFromStart = 0;
	_fallenDistance = 0;
}

void Lemming::revive() {
	_dead = false;
	startWalk();
}
void Lemming::checkOutbounds() {
	if (_sprite->position().x > CAMERA_WIDTH || _sprite->position().x < 0 ||
		_sprite->position().y > CAMERA_HEIGHT || _sprite->position().y < 0) die();
}