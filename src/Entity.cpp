#include "Entity.h"

Entity::Entity(AssimpModel* model, const glm::vec3 pos, const glm::vec3 rot, const float scale) : 
	model_(*model), position_(pos), rotation_(rot), scale_(scale)
{
	matrix_ = glm::translate(matrix_, position_);
	matrix_ = glm::rotate(matrix_, rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
	matrix_ = glm::rotate(matrix_, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix_ = glm::rotate(matrix_, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));
	matrix_ = glm::scale(matrix_, glm::vec3(scale_, scale_, scale_));
}

Entity::~Entity()
{

}

void Entity::updateMatrix()
{
	matrix_ = glm::mat4(1.0f);
	matrix_ = glm::translate(matrix_, position_);
	matrix_ = glm::rotate(matrix_, rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
	matrix_ = glm::rotate(matrix_, rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix_ = glm::rotate(matrix_, rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));
	matrix_ = glm::scale(matrix_, glm::vec3(scale_, scale_, scale_));
}

const glm::mat4& Entity::getMatrix()
{
	return matrix_;
}
