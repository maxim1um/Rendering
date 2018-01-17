#include "rotation.h"

glm::quat RotationQuaternion(glm::quat p, glm::quat q)
{
	glm::quat result = glm::cross(p, q);

	return result;
}

glm::mat3 QuaternionToMatrix(glm::quat p)
{
	glm::mat3 result = {
		1 - 2 * p.y * p.y - 2 * p.z * p.z,  2 * p.x * p.y + 2 * p.w * p.z,      2 * p.x * p.z - 2 * p.w * p.y,
		2 * p.x * p.y - 2 * p.w * p.z,      1 - 2 * p.x * p.x - 2 * p.z * p.z,  2 * p.y * p.z + 2 * p.w * p.x,
		2 * p.x*p.z + 2 * p.w*p.y,          2 * p.y*p.z - 2 * p.w*p.x,          1 - 2 * p.x*p.x - 2 * p.y*p.y
	};

	return result;
}