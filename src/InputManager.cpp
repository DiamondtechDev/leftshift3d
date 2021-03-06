#include "InputManager.h"

InputManager::InputManager() : mouseCoords_(0, 0)
{

}

InputManager::~InputManager()
{

}

void InputManager::update()
{
	for(auto& it : keyMap_)
	{
		oldKeyMap_[it.first] = it.second;
	}

	// Keep old mouse coordinates
	oldMouseCoords_ = mouseCoords_;

	// Reset the mouse wheel scroll, as we will ever only need it once.
	y_mwheel_ = 0;
	x_mwheel_ = 0;
}

void InputManager::pressKey(unsigned int keyID)
{
	keyMap_[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID)
{
	keyMap_[keyID] = false;
}

void InputManager::setMouseCoords(float x, float y)
{
	mouseCoords_.x = x;
	mouseCoords_.y = y;
}

bool InputManager::isKeyDown(unsigned int keyID)
{
	auto it = keyMap_.find(keyID);
	if(it != keyMap_.end())
		return it->second;
	else
		return false;
}

bool InputManager::wasKeyDown(unsigned int keyID)
{
	auto it = oldKeyMap_.find(keyID);
	if(it != oldKeyMap_.end())
		return it->second;
	else
		return false;
}

bool InputManager::isKeyPressed(unsigned int keyID)
{
	if(isKeyDown(keyID) == true && wasKeyDown(keyID) == false)
		return true;
	return false;
}

void InputManager::setMouseWheel(int vert, int horiz)
{
	y_mwheel_ = vert;
	x_mwheel_ = horiz;
}
