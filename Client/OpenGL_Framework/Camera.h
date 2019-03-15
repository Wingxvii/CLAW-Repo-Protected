#pragma once
#include "Entity.h"

enum ProjectionType
{
	Perspective,
	Orthographic
};

class Camera : public Entity
{
public:
	Camera();
	Camera(ProjectionType projType);

	void perspective(
		float fovy, float aspect,
		float zNear, float zFar);

	void orthographic(
		float left, float right,
		float bottom, float top,
		float zNear, float zFar);

	glm::mat4 getView() const;
	glm::mat4 getProjection() const;

	Transform* transform = new Transform();
 
private:
	ProjectionType projectionType = ProjectionType::Perspective;
	glm::mat4 projection = glm::mat4(1.0);

	
};