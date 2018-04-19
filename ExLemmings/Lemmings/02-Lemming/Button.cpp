#include "Button.h"


void Button::loadSpritesheet(string filename, const glm::vec2& position) {
	_spritesheet.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA);
	_spritesheet.setMinFilter(GL_NEAREST);
	_spritesheet.setMagFilter(GL_NEAREST);
	_sprite = Sprite::createSprite(glm::ivec2(_spritesheet.width(), _spritesheet.height() / NUM_ANIM), glm::vec2(1.0f , 1.0f / float(NUM_ANIM)), &_spritesheet, &_shaderProgram);
	_sprite->setNumberAnimations(NUM_ANIM);

	for (int i = 0; i < NUM_ANIM; i++) {
		_sprite->setAnimationSpeed(i, 12);
	}
	float height = 1.0f / float(NUM_ANIM);
	for (int anim = 0; anim < NUM_ANIM; anim++) {
		_sprite->addKeyframe(anim, glm::vec2(0, float(anim)*height));
	}
	_sprite->setPosition(position);
}

void Button::init(const glm::vec2 &position, string filename, ShaderProgram &shaderProgram)
{
	_state = NORMAL;
	_shaderProgram = shaderProgram;
	loadSpritesheet(filename, position);
	_sprite->setPosition(position);
	posX = position.x;
	posY = position.y;
	buttonHeight = float(_spritesheet.height()) / NUM_ANIM;
	buttonWidth = float(_spritesheet.width());
}

void Button::mouseMoved(int mouseXInput, int mouseYInput, bool bLeftButton)
{
	int mouseX = mouseXInput / 3;
	int mouseY = mouseYInput / 3;
	bool over = false;
	if (mouseX >= posX && mouseX <= posX + buttonWidth && mouseY >= posY && mouseY <= posY + buttonHeight)
		over = true;

	switch (_state)
	{
	case NORMAL:
		if (over && bLeftButton)
		{
			_sprite->changeAnimation(MOUSEPRESSED);
			_state = MOUSEPRESSED;
		}
		else if (over)
		{
			_sprite->changeAnimation(MOUSEOVER);
			_state = MOUSEOVER;
		}
		break;

	case MOUSEPRESSED:
		if (over && !bLeftButton)
		{
			_sprite->changeAnimation(SELECTED);
			_state = SELECTED;
		}
		else if (!over)
		{
			_sprite->changeAnimation(NORMAL);
			_state = NORMAL;
		}
		break;

	case MOUSEOVER:
		if (over && bLeftButton)
		{
			_sprite->changeAnimation(MOUSEPRESSED);
			_state = MOUSEPRESSED;
		}
		else if (!over)
		{
			_sprite->changeAnimation(NORMAL);
			_state = NORMAL;
		}
		break;
	}
}


bool Button::mouseReleased(int mouseXInput, int mouseYInput) {
	int mouseX = mouseXInput / 3;
	int mouseY = mouseYInput / 3;
	bool over = false;
	if (mouseX >= posX && mouseX <= posX + buttonWidth && mouseY >= posY && mouseY <= posY + buttonHeight)
		over = true;
	if (_state == SELECTED && over) {
		this->deselect();
		return true;
	}
	return false;
}

bool Button::isSelected() 
{
	if (_state == SELECTED)
		return true;
	return false;
}

void Button::deselect() 
{
	_state = NORMAL;
	_sprite->changeAnimation(NORMAL);
}

void Button::render()
{
	_sprite->render();
}

