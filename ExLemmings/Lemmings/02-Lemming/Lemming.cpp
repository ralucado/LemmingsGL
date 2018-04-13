#include <cmath>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"

void Lemming::loadSpritesheet(string filename, int NUM_FRAMES,  int NUM_ANIMS, const glm::vec2& position) {
	_spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	_spritesheet.setMinFilter(GL_NEAREST);
	_spritesheet.setMagFilter(GL_NEAREST);
	_sprite = Sprite::createSprite(glm::ivec2(_spritesheet.width() / NUM_FRAMES, _spritesheet.height() / NUM_ANIMS), glm::vec2(1.0f / float(NUM_FRAMES), 1.0f / float(NUM_ANIMS)), &_spritesheet, &_shaderProgram);
	_sprite->setNumberAnimations(NUM_ANIMS);
	int speed = 12;
	if (_state == START_FLOAT_RIGHT || _state == START_FLOAT_LEFT) speed = 8;
	for (int i = 0; i < NUM_ANIMS; i++) {
		_sprite->setAnimationSpeed(i, speed);
	}
	float height = 1.0f / float(NUM_ANIMS);
	for (int frame = 0; frame < NUM_FRAMES; frame++) {
		float num_frame = float(frame) / float(NUM_FRAMES);
		for (int anim = 0; anim < NUM_ANIMS; anim++) {
			_sprite->addKeyframe(anim, glm::vec2(num_frame + 0.5f / _spritesheet.width(), float(anim)*height + 0.5f / _spritesheet.height()));
			//_sprite->addKeyframe(anim, glm::vec2(num_frame, float(anim)*height));

		}
	}
	_sprite->setPosition(position);
}

void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	_state = FALLING_RIGHT;
	_shaderProgram = shaderProgram;
	loadSpritesheet("images/lemming.png", 8, 4, initialPosition);
	_sprite->changeAnimation(FALLING_RIGHT_ANIM);
}

void Lemming::update(int deltaTime)
{
	int fall;

	if(_dead || _sprite->update(deltaTime) == 0)
		return;
	bool canDescend;
	glm::vec2 ori;
	switch(_state)
	{
	case FALLING_LEFT:
		if (_fallenDistance > HEIGHT_TO_FLOAT && _canFloat) {
			startFloat(false);
		}
		if (!updateFall()) {
			if (_fallenDistance > HEIGHT_TO_DIE) {
				startSquish();
			}
			else {
				_fallenDistance = 0;
				_sprite->changeAnimation(WALKING_LEFT_ANIM);
				_state = WALKING_LEFT;
			}
		}
		break;
	case FALLING_RIGHT:
		if (_fallenDistance > HEIGHT_TO_FLOAT && _canFloat) {
			startFloat(true);
		}
		if (!updateFall()) {
			if (_fallenDistance > HEIGHT_TO_DIE) {
				startSquish();
			}
			else {
				_fallenDistance = 0;
				_sprite->changeAnimation(WALKING_RIGHT_ANIM);
				_state = WALKING_RIGHT;
			}
		}
		break;
	case FLOAT_LEFT:
		if(!updateFall()){
			startWalk(false);
		}
		break;
	case FLOAT_RIGHT:
		if (!updateFall()){
			startWalk(true);
		}
		break;
	case START_FLOAT_LEFT:
		++_framesFromStart;
		if (_framesFromStart == 3) {
			_state = FLOAT_LEFT;
			_sprite->changeAnimation(FLOAT_LEFT_ANIM);
		}
		break;
	case START_FLOAT_RIGHT:
		++_framesFromStart;
		if (_framesFromStart == 3) {
			_state = FLOAT_RIGHT;
			_sprite->changeAnimation(FLOAT_RIGHT_ANIM);
		}
		break;
	case WALKING_LEFT:
		canDescend = true;
		ori = _sprite->position(); //save original position
		_sprite->position() += glm::vec2(-1, -3); //try to put the lemming up top a 3 pixel climb
		while (collision() && canDescend)
		{
			//while the position is a collision, try a position lower
			_sprite->position() += glm::vec2(0, 1);
			//if we can not move more positions in a single movement
			if (_sprite->position().y - ori.y > 3) canDescend = false;
		}
		if (!canDescend) {
			_sprite->position() = ori;
			_sprite->changeAnimation(WALKING_RIGHT_ANIM);
			_state = WALKING_RIGHT;
		}
		else
		{
			fall = collisionFloor(7);
			_sprite->position() += glm::vec2(0, fall);
			if (fall > 6){
				_state = FALLING_LEFT;
				_sprite->changeAnimation(FALLING_LEFT_ANIM);
			}
		}
		break;
	case WALKING_RIGHT:
		canDescend = true;
		ori = _sprite->position(); //save original position
		_sprite->position() += glm::vec2(1, -3); //try to put the lemming up top a 3 pixel climb
		while(collision() && canDescend)
		{
			//while the position is a collision, try a position lower
			_sprite->position() += glm::vec2(0, 1);
			//if we can not move more positions in a single movement
			if (_sprite->position().y - ori.y > 3) canDescend = false;
		}
		if (!canDescend) {
			_sprite->position() = ori;
			_sprite->changeAnimation(WALKING_LEFT_ANIM);
			_state = WALKING_LEFT;
		}
		else
		{
			fall = collisionFloor(7);
			_sprite->position() += glm::vec2(0, fall);
			if (fall > 6) {
				_state = FALLING_RIGHT;
				_sprite->changeAnimation(FALLING_RIGHT_ANIM);
			}
		}
		break;
	case BASH_LEFT:
		//check that there is material to dig
		if (collisionWall(12, false) > 8) startWalk(false);

		++_framesFromStart;
		_framesFromStart %= 32;
		if ((_framesFromStart > 10 && _framesFromStart <= 15) ||
			(_framesFromStart > 26 && _framesFromStart <= 31))
				_sprite->position() += glm::vec2(-1, 0);
		if (_framesFromStart >= 2 && _framesFromStart <= 6)
			bashRow(_framesFromStart - 2, false);
		else if (_framesFromStart >= 18 && _framesFromStart <= 22)
			bashRow(_framesFromStart - 18, false);
		break;
	case BASH_RIGHT:
		//check that there is material to dig
		if (collisionWall(12, true) > 8) startWalk(true);

		++_framesFromStart;
		_framesFromStart %= 32;
		if ((_framesFromStart > 10 && _framesFromStart <= 15) ||
			(_framesFromStart > 26 && _framesFromStart <= 31))
			_sprite->position() += glm::vec2(1, 0);
		if (_framesFromStart >= 2 && _framesFromStart <= 6)
			bashRow(_framesFromStart - 2, true);
		else if (_framesFromStart >= 18 && _framesFromStart <= 22)
			bashRow(_framesFromStart - 18, true);
		break;
	case DIGGING:
		++_framesFromStart;
		_framesFromStart %= 16;
		_sprite->position() += glm::vec2(0, -2);
		fall = collisionFloor(1);
		_sprite->position() += glm::vec2(0, 2);
		if (fall > 0) {
			startWalk(true);
		}
		else if (_framesFromStart == 7 || _framesFromStart == 15)
		{
			digRow();
			_sprite->position() += glm::vec2(0, 2);
		}
		break;
	case EXPLODING:
		++_framesFromStart;
		if (_framesFromStart == 16) pop();
		break;
	case SQUISHED:
		++_framesFromStart;
		if (_framesFromStart == 16) die();
	}
}

bool Lemming::updateFall() {
	int fall = collisionFloor(2);
	if (fall > 0) {
		_sprite->position() += glm::vec2(0, fall);
		_fallenDistance += fall;
		return true;
	}
	return false;
}


void Lemming::render()
{
	if (!_dead) {
		//_shaderProgram.setUniform1i("clicked", b);
		_shaderProgram.setUniform2f("center", _sprite->position().x, _sprite->position().y);

		//	.setUniformMatrix4f("modelview", modelview);
		_sprite->render();
	}
}

void Lemming::switchFloater() {
	_canFloat = !_canFloat;
}

void Lemming::switchClimber() {
	_canClimb = !_canClimb;
}

void Lemming::switchStopper() {
	if (_state != STOPPED) {
		startStop();
	}
	//TEST
	else if (_state == STOPPED) {
		startWalk(true);
	}
}

void Lemming::switchBomber() {
	if (_state != EXPLODING) {
		startPop();
	}
	//TEST
	else if (_state == EXPLODING) {
		_dead = false;
		startWalk(true);
	}
}

void Lemming::switchBasher(bool r)
{
	if (_state != BASH_RIGHT && _state != BASH_LEFT) {
		startBash(r);
	}
	//TEST
	else if (_state == BASH_RIGHT || _state == BASH_LEFT) {
		startWalk(r);
	}
}

void Lemming::switchDigger()
{
	if (_state != DIGGING) {
		startDig();
	}
	//TEST
	else if (_state == DIGGING) {
		startWalk(true);
	}
}

void Lemming::startStop() {
	_state = STOPPED;
	loadSpritesheet("images/stopper.png", 16, 1, _sprite->position());
	_sprite->changeAnimation(STOPPED_ANIM);
}

void Lemming::startPop() {
	loadSpritesheet("images/bomber.png", 16, 1, _sprite->position());
	_state = EXPLODING;
	_framesFromStart = 0;
	_sprite->changeAnimation(EXPLODING_ANIM);
}

void Lemming::startBash(bool r) {
	loadSpritesheet("images/basher.png", 32, 2, _sprite->position());
	_state = (r? BASH_RIGHT : BASH_LEFT);
	_framesFromStart = 0;
	_sprite->changeAnimation((r ? BASH_RIGHT_ANIM : BASH_LEFT_ANIM));
}

void Lemming::startWalk(bool r) {
	_state = (r ? WALKING_RIGHT : WALKING_LEFT);
	_fallenDistance = 0;
	loadSpritesheet("images/lemming.png", 8, 4, _sprite->position());
	_sprite->changeAnimation((r ? WALKING_RIGHT_ANIM : WALKING_LEFT_ANIM));
}

void Lemming::startFloat(bool r) {
	_state = (r ? START_FLOAT_RIGHT : START_FLOAT_LEFT);
	loadSpritesheet("images/floater.png", 4, 4, _sprite->position());
	_framesFromStart = 0;
	_sprite->changeAnimation((r ? START_FLOAT_RIGHT_ANIM : START_FLOAT_LEFT_ANIM));
}

void Lemming::startSquish() {
	_state = SQUISHED;
	loadSpritesheet("images/squished.png", 16, 1, _sprite->position());
	_framesFromStart = 0;
	_sprite->changeAnimation(SQUISHED_ANIM);
}

void Lemming::startDig() {
	_state = DIGGING;
	//digging animation is displaced, it shows pixels of the lemming INSIDE of the ground
	//we move 2 squares down to achieve the effect
	_sprite->position() += glm::vec2(0, 2);
	loadSpritesheet("images/digger.png", 16, 1, _sprite->position());
	_framesFromStart = 0;
	_sprite->changeAnimation(DIGGING_ANIM);
}


void Lemming::setMapMask(VariableTexture *mapMask)
{
	_mask = mapMask;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = _sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((_mask->pixel(posBase.x, posBase.y+fall) == 0) && (_mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

int Lemming::collisionWall(int maxDeep, bool r)
{
	bool bContact = false;
	int deep = 0;
	glm::ivec2 posBase = _sprite->position() + glm::vec2(120, 0); // Add the map displacement

	posBase += glm::ivec2(7, 15);
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
	glm::ivec2 posBase = _sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
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
	die();
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	int posX = floor(_sprite->position().x + 120)+7;
	int posY = floor(_sprite->position().y)+16;
	hole(posX, posY, 5);
}

void Lemming::bashRow(int index, bool r) {
	int displacement = 120;
	if (!r) displacement += 19; //the sprite width
	glm::ivec2 posBase = _sprite->position() + glm::vec2(displacement, 0);
	for (int i = 8; i <= bashPixels[index].x; ++i) {
		int aux = (r? i : -i);
		_mask->setPixel(posBase.x + aux, posBase.y + bashPixels[index].y,     0);
		_mask->setPixel(posBase.x + aux, posBase.y + bashPixels[index].y + 1, 0);
	}
}

void Lemming::digRow() {
	int posX = floor(_sprite->position().x + 120) + 3;
	int posY = floor(_sprite->position().y) + 16;
	for (int i = 0; i < 8; ++i){
		_mask->setPixel(posX + i, posY - 2, 0);
		_mask->setPixel(posX + i, posY - 1, 0);
	}
}

void Lemming::die() {
	_dead = true;
	_framesFromStart = 0;
	_fallenDistance = 0;
}

void Lemming::revive() {
	_dead = false;
	startWalk(true);
}