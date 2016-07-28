#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

class InputManager
{
	public:
		InputManager();
		~InputManager();

		void update();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		void setMouseCoords(float x, float y);

		bool isKeyDown(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		glm::vec2 getMouseCoords() const { return mouseCoords_; }
	private:
		bool wasKeyDown(unsigned int keyID);
		std::unordered_map<unsigned int, bool> keyMap_;
		std::unordered_map<unsigned int, bool> oldKeyMap_;
		glm::vec2 mouseCoords_;
};
