#include "anepch.h"
#include "CollisionListener.h"

#include "CollisionData.h"
#include "Physics.h"

namespace Engine
{
    CollisionListener::CollisionListener() = default;

    CollisionListener::~CollisionListener()
    = default;

    void CollisionListener::onContact(const CallbackData& callbackData)
    {
        PhysicsSystem& physics = GetPhysicsSystem();

        for (uint i = 0; i < callbackData.getNbContactPairs(); ++i)
        {
            ContactPair contactPair = callbackData.getContactPair(i);
            reactphysics3d::Entity entity = contactPair.getBody1()->getEntity();
            if(!physics._reactEntity.contains(entity)) continue;

            const CollisionEventType type = static_cast<CollisionEventType>(contactPair.getEventType());
            CollisionData data;
            auto otherEntity = contactPair.getBody2()->getEntity();
            data.Entity = physics._reactEntity[otherEntity];
            data.RigidBody = physics._reactRigidBody[otherEntity];
            data.Collider = physics._reactCollider[contactPair.getCollider2()->getEntity()];

            data.Contacts.resize(contactPair.getNbContactPoints());
            for (uint contact = 0; contact < contactPair.getNbContactPoints(); ++contact)
            {
                auto contactPoint = contactPair.getContactPoint(contact);
                data.Contacts[contact].Normal = Vector3::Convert(-contactPoint.getWorldNormal());
                data.Contacts[contact].PenetrationDepth = contactPoint.getPenetrationDepth();
                data.Contacts[contact].Point = Vector3::Convert(contactPair.getCollider2()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider2());
            }

            physics.DispatchCollisionCallback(physics._reactEntity[entity], type, data);
        }
    }

    void CollisionListener::onTrigger(const rp3d::OverlapCallback::CallbackData& callbackData)
    {
        PhysicsSystem& physics = GetPhysicsSystem();

        for (uint i = 0; i < callbackData.getNbOverlappingPairs(); ++i)
        {
            rp3d::OverlapCallback::OverlapPair contactPair = callbackData.getOverlappingPair(i);
            rp3d::Entity entity = contactPair.getBody1()->getEntity();
            if(!physics._reactEntity.contains(entity)) continue;

            const CollisionEventType type = static_cast<CollisionEventType>(contactPair.getEventType());
            TriggerData data;
            auto otherEntity = contactPair.getBody2()->getEntity();
            data.Entity = physics._reactEntity[otherEntity];
            data.RigidBody = physics._reactRigidBody[otherEntity];
            data.Collider = physics._reactCollider[contactPair.getCollider2()->getEntity()];

            physics.DispatchTriggerCallback(physics._reactEntity[entity], type, data);
        }
    }
}


