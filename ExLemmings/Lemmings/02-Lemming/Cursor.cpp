#include <cmath>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Cursor.h"
#include "Game.h"


void Cursor::loadSpritesheet(string filename, int NUM_FRAMES, int NUM_ANIMS, const glm::vec2& position, int speed) {
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

//void Exit::init(const glm::vec2 &initialPosition, const glm::vec2 &positionExit, ShaderProgram &shaderProgram)
void Cursor::init(ShaderProgram &shaderProgram)
{
	_state = NORMAL;
	_shaderProgram = shaderProgram;
	_framesFromStart = 0;
	loadSpritesheet("images/cursor.png", 1, 2, glm::vec2(0,0), 12);
	_sprite->changeAnimation(NORMAL_ANIM);
	_dispX = 0;
	_dispY = 0;
}

void Cursor::update(int deltaTime)
{
	_sprite->update(deltaTime);
}

void Cursor::render()
{
	_sprite->render();
}

void Cursor::setPosition(glm::vec2 position)
{
	_sprite->setPosition(position);
}

void Cursor::setActive() {
	_sprite->changeAnimation(ACTIVE_ANIM);
}
void Cursor::setNormal() {
	_sprite->changeAnimation(NORMAL_ANIM);
}
