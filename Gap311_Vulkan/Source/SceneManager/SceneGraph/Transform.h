#pragma once

#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

struct Transform
{
    glm::vec3 position;  // Position in 3D space
    glm::vec3 rotation;  // Euler Angles (pitch, yaw, roll)
    glm::vec3 scale;     // Scale factor

    Transform()
        : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f)
    {
        //
    }

    // Local transform matrix
    glm::mat4 GetLocalTransform() const
    {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);

        // Create rotation matrix from Euler angles (pitch, yaw, roll)
        glm::mat4 rotationMat = glm::mat4(1.0f);
        rotationMat = glm::rotate(rotationMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis 
        rotationMat = glm::rotate(rotationMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis 
        rotationMat = glm::rotate(rotationMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis 

        glm::mat4 scaling = glm::scale(glm::mat4(1.0f), scale);

        // Combine the transform
        return translation * rotationMat * scaling;
    }

    // World transform
    glm::mat4 GetWorldTransform(const glm::mat4& parentWorldTransform) const
    {
        return parentWorldTransform * GetLocalTransform();
    }

    void DecomposeFromMatrix(const aiMatrix4x4& aiMat)
    {
        // Convert aiMatrix4x4 to glm::mat4
        glm::mat4 glmMat = glm::transpose(glm::make_mat4(&aiMat.a1));

        // Decompose the glm::mat4
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::quat orientation;
        glm::vec3 translation, scaling;

        if (glm::decompose(glmMat, scaling, orientation, translation, skew, perspective))
        {
            position = translation;
            scale = scaling;
            rotation = glm::degrees(glm::eulerAngles(orientation));
        }
        else
        {
            // Handle decomposition failure
            position = glm::vec3(0.0f);
            scale = glm::vec3(1.0f);
            rotation = glm::vec3(0.0f);
        }
    }

};

