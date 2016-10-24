#pragma once
#include "LSUtil.h"
#include "AssimpModel.h"

class Entity
{
	public:
		Entity(AssimpModel* model, const glm::vec3 pos, const glm::vec3 rot, const float scale);
		~Entity();

		const glm::mat4& getMatrix();
		AssimpModel& getModel() const { return model_; }

		glm::vec3 getPosition() const { return position_; }
		glm::vec3 getRotation() const { return rotation_; }
		float getScale() const { return scale_; }

		void setPosition(glm::vec3 pos) { position_ = pos; updateMatrix(); }
		void setRotation(glm::vec3 rot) { rotation_ = rot; updateMatrix(); }
		void setScale(float scale) { scale_ = scale; updateMatrix(); }

	protected:
		void updateMatrix();

		AssimpModel& model_;
		glm::mat4 matrix_ = glm::mat4(1.0f);
		glm::vec3 position_;
		glm::vec3 rotation_;
		float scale_;
};
