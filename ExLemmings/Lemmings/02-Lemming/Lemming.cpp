#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define NUM_FRAMES 8
#define NUM_ANIMS 4

enum LemmingAnims
{
	WALKING_RIGHT, WALKING_LEFT, FALLING_RIGHT, FALLING_LEFT
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.0f/float(NUM_FRAMES), 1.0f/float(NUM_ANIMS)), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(NUM_ANIMS);

		float height = float(1.0f / NUM_ANIMS);
		
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / float(NUM_FRAMES), float(WALKING_RIGHT)*height));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / float(NUM_FRAMES), float(WALKING_LEFT)*height));
		
		sprite->setAnimationSpeed(FALLING_RIGHT, 12);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / float(NUM_FRAMES), float(FALLING_RIGHT)*height));
		
		sprite->setAnimationSpeed(FALLING_LEFT, 12);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i) / float(NUM_FRAMES), float(FALLING_LEFT)*height));
		
	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);
}

void Lemming::update(int deltaTime)
{
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;

	switch(state)
	{
	case FALLING_LEFT_STATE:
		fall = collisionFloor(2);
		if(fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else{
			state = WALKING_LEFT_STATE;
			sprite->changeAnimation(WALKING_LEFT);
		}

		break;
	case FALLING_RIGHT_STATE:
		fall = collisionFloor(2);
		if(fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else{
			state = WALKING_RIGHT_STATE;
			sprite->changeAnimation(WALKING_RIGHT);
		}
			
		break;
	case WALKING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		if(collision())
		{
			sprite->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if(fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if(fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2){
				state = FALLING_LEFT_STATE;
				sprite->changeAnimation(FALLING_LEFT);
			}

		}
		break;
	case WALKING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		if(collision())
		{
			sprite->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if(fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else{
				state = FALLING_RIGHT_STATE;
				sprite->changeAnimation(FALLING_RIGHT);
			}
				
		}
		break;
	}
}

void Lemming::render()
{
	sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}







