#include "anepch.h"
#include "ColliderTest.h"

#include "ANE/Core/Scene/Components/RigidBodyComponent.h"
#include "ANE/Physics/PhysicsTypes.h"
#include "ANE/Physics/Types/RigidBody.h"

void Engine::ColliderTest::OnCreate()
{
    _rb = &GetComponent<RigidBodyComponent>();
    // _rb->GetRigidBody()->SetUseGravity(false);
    // _rb->GetRigidBody()->AddForce(-Vector3::UpVector(), false);
}

void Engine::ColliderTest::OnCollision(const CollisionEventType type, const CollisionData& collisionData)
{
    if(type != CollisionEventType::Enter) return;
    ANE_ELOG_INFO("CollisionTest {}", collisionData.Contacts[0].Normal.ToString());

    const RigidBody* rigidBody = _rb->GetRigidBody();
    rigidBody->AddForce(collisionData.Contacts[0].Normal*10.f, false);
}

void Engine::ColliderTest::OnUpdate(float deltaTime)
{
    //_rb->GetRigidBody()->AddForce(Vector3::UpVector()*deltaTime*60.f);
}
