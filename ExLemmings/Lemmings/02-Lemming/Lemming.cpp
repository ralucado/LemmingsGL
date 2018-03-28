#include <cmath>
#include <iostream>
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

	for (int i = 0; i < NUM_ANIMS; i++) {
		_sprite->setAnimationSpeed(i, 12);
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

	if(_sprite->update(deltaTime) == 0)
		return;

	switch(_state)
	{
	case FALLING_LEFT:
		fall = collisionFloor(2);
		if(fall > 0)
			_sprite->position() += glm::vec2(0, fall);
		else{
			_state = WALKING_LEFT;
			_sprite->changeAnimation(WALKING_LEFT_ANIM);
		}

		break;
	case FALLING_RIGHT:
		fall = collisionFloor(2);
		if(fall > 0)
			_sprite->position() += glm::vec2(0, fall);
		else{
			_state = WALKING_RIGHT;
			_sprite->changeAnimation(WALKING_RIGHT_ANIM);
		}
			
		break;
	case WALKING_LEFT:
		_sprite->position() += glm::vec2(-1, -1);
		if(collision())
		{
			_sprite->position() -= glm::vec2(-1, -1);
			_sprite->changeAnimation(WALKING_RIGHT_ANIM);
			_state = WALKING_RIGHT;
		}
		else
		{
			fall = collisionFloor(3);
			if(fall > 0)
				_sprite->position() += glm::vec2(0, 1);
			if(fall > 1)
				_sprite->position() += glm::vec2(0, 1);
			if (fall > 2){
				_state = FALLING_LEFT;
				_sprite->changeAnimation(FALLING_LEFT_ANIM);
			}
		}
		break;
	case WALKING_RIGHT:
		_sprite->position() += glm::vec2(1, -1);
		if(collision())
		{
			_sprite->position() -= glm::vec2(1, -1);
			_sprite->changeAnimation(WALKING_LEFT_ANIM);
			_state = WALKING_LEFT;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall > 0)
				_sprite->position() += glm::vec2(0, 1);
			if (fall > 1)
				_sprite->position() += glm::vec2(0, 1);
			if (fall > 2) {
				_state = FALLING_RIGHT;
				_sprite->changeAnimation(FALLING_RIGHT_ANIM);
			}
		}
		break;
	}
}


void Lemming::render()
{
	_sprite->render();
}

void Lemming::makeStopper(bool b) {
	if (b && _state != STOPPED) {
		_state = STOPPED;
		loadSpritesheet("images/stopper.png", 16, 1, _sprite->position());
		_sprite->changeAnimation(STOPPED_ANIM);
	}
	//TEST
	else if (!b && _state == STOPPED) {
		_state = WALKING_RIGHT;
		loadSpritesheet("images/lemming.png", 8, 4, _sprite->position());
		_sprite->changeAnimation(WALKING_RIGHT_ANIM);
	}
}

void Lemming::makeBomber(bool b) {
	if (b && _state != EXPLODING) {
		loadSpritesheet("images/bomber.png", 16, 1, _sprite->position());
		_state = EXPLODING;
		_sprite->changeAnimation(EXPLODING_ANIM);
	}
	//TEST
	else if (!b && _state == EXPLODING) {
		_state = WALKING_RIGHT;
		loadSpritesheet("images/lemming.png", 8, 4, _sprite->position());
		_sprite->changeAnimation(WALKING_RIGHT_ANIM);
	}
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

bool Lemming::collision()
{
	glm::ivec2 posBase = _sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((_mask->pixel(posBase.x, posBase.y) == 0) && (_mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}







