#pragma once

namespace Engine
{
    struct TransformMatrix
    {
    public:
        TransformMatrix(const glm::mat4& transform = glm::mat4(1.f)) : _transformMatrix(transform) {}
        TransformMatrix(const TransformMatrix&) = default;

        void SetPosition(const glm::vec3 newPosition)
        {
            _transformMatrix[3] = xyz1(newPosition);
        }

        void AddPosition(const glm::vec3 delta)
        {
            _transformMatrix = translate(_transformMatrix, delta);
        }

        glm::vec3 GetPosition()
        {
            return xyz(_transformMatrix[3]);
        }

        void SetRotation(const glm::vec3 newRotation) // todo: fix, overwrite rotation
        {
            const glm::vec3 scale = GetScale();

            _transformMatrix[0] = glm::vec4(1, 0, 0, 0) * scale.x;
            _transformMatrix[1] = glm::vec4(0, 1, 0, 0) * scale.y;
            _transformMatrix[2] = glm::vec4(0, 0, 1, 0) * scale.z;
            _transformMatrix = rotate(_transformMatrix, glm::radians(newRotation.x), glm::vec3(1.f, 0.f, 0.f));
            _transformMatrix = rotate(_transformMatrix, glm::radians(newRotation.y), glm::vec3(0.f, 1.f, 0.f));
            _transformMatrix = rotate(_transformMatrix, glm::radians(newRotation.z), glm::vec3(0.f, 0.f, 1.f));
        }

        void SetRotation(const glm::quat newRotation)
        {
            const glm::vec3 scale = GetScale();

            glm::mat3 rotMatrix = mat3_cast(newRotation);

            _transformMatrix[0] = xyz0(rotMatrix[0]) * scale.x;
            _transformMatrix[1] = xyz0(rotMatrix[1]) * scale.y;
            _transformMatrix[2] = xyz0(rotMatrix[2]) * scale.z;
        }

        void AddRotation(const glm::vec3 delta)
        {
            _transformMatrix = rotate(_transformMatrix, glm::radians(delta.x), glm::vec3(1.f, 0.f, 0.f));
            _transformMatrix = rotate(_transformMatrix, glm::radians(delta.y), glm::vec3(0.f, 1.f, 0.f));
            _transformMatrix = rotate(_transformMatrix, glm::radians(delta.z), glm::vec3(0.f, 0.f, 1.f));
        }

        void AddRotation(const glm::quat delta)
        {
            glm::mat4 rotMatrix = mat4_cast(delta);
            rotMatrix[3][3] = 1;

            _transformMatrix *= rotMatrix;
        }

        glm::vec3 GetEulerRotation() const
        {
            return eulerAngles(quat_cast(_transformMatrix));
        }

        glm::quat GetRotation() const
        {
            return quat_cast(_transformMatrix);
        }

        glm::vec3 GetScale() const
        {
            glm::vec3 scale;

            scale.x = length(xyz(_transformMatrix[0]));
            scale.y = length(xyz(_transformMatrix[1]));
            scale.z = length(xyz(_transformMatrix[2]));
            return scale;
        }

        operator const glm::mat4&() const { return _transformMatrix; }
        operator glm::mat4&() { return _transformMatrix; }

    private:
        glm::mat4 _transformMatrix;
    };
}
