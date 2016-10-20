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
		void setMouseWheel(int vert, int horiz);

		bool isKeyDown(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

		glm::vec2 getMouseCoords() const { return mouseCoords_; }
		glm::vec2 getOldMouseCoords() const { return oldMouseCoords_; }

		int getMouseWheelVertical() const { return y_mwheel_; }
		int getMouseWheelHorizontal() const { return x_mwheel_; }
	private:
		bool wasKeyDown(unsigned int keyID);
		std::unordered_map<unsigned int, bool> keyMap_;
		std::unordered_map<unsigned int, bool> oldKeyMap_;
		glm::vec2 mouseCoords_;
		glm::vec2 oldMouseCoords_;
		int y_mwheel_;
		int x_mwheel_;
};
