#pragma once
#ifndef ROTATION_H
#define ROTATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::quat RotationQuaternion(glm::quat p, glm::quat q);

glm::mat3 QuaternionToMatrix(glm::quat p);

#endif

