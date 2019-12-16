#include "pch.h"
#include "RigidBody.h"
#include "Physics.h"
#include "Entity.h"
void RigidBody::OnUpdate(float deltaTime)
{
}
void RigidBody::OnRender()
{
}
void RigidBody::Init(CollisionShape* shape, float mass, const glm::vec3 localInertia)
{
	btTransform bT = Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());
	m_mState = new btDefaultMotionState(bT);
	m_shape = shape;
	btVector3 li = btVector3(localInertia.x, localInertia.y,localInertia.z);
	m_shape->Get()->calculateLocalInertia(mass, li);
	m_rigidBody = new btRigidBody(btScalar(mass), m_mState,
	m_shape->Get(), li);
	Physics::GetInstance()->AddRigidbody(this);
	m_rigidBody->setSleepingThresholds(0, 0);
	m_rigidBody->setFriction(1);
}
void RigidBody::UpdateParent()
{
	// Get the transform from the rigidbody's motionstate and apply it to the game objects transform

	btTransform bT;
	m_rigidBody->getMotionState()->getWorldTransform(bT);


	Physics::ConvertbtTransformToTransform(bT, m_entity->GetTransform());
}

void RigidBody::UpdateRigidBody()
{
	btTransform t = Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());
	m_rigidBody->setWorldTransform(t);
	m_rigidBody->getMotionState()->setWorldTransform(t);
}

void RigidBody::OnAttach()
{

}
//Method to apply the force to the 3d model
void RigidBody::ApplyForce()
{

	btTransform bT;
	m_rigidBody->getMotionState()->getWorldTransform(bT);

	m_rigidBody->applyForce(btVector3(0, 100, 0), m_rigidBody->getCenterOfMassPosition());

	Physics::ConvertbtTransformToTransform(bT, m_entity->GetTransform());
}
//Method to apply the torque to the 3d model
void RigidBody::ApplyTorque()
{
	btTransform bT;
	m_rigidBody->getMotionState()->getWorldTransform(bT);

	m_rigidBody->applyTorque(btVector3(0, 100, 0));

	Physics::ConvertbtTransformToTransform(bT, m_entity->GetTransform());
}
