#include "Component.h"
#include "Physics.h"
#include "CollisionShape.h"
class RigidBody : public Component
{
private:
	CollisionShape* m_shape;
	btRigidBody* m_rigidBody;
	btMotionState* m_mState = nullptr;
	float mass;
public:
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnAttach() override;
	void Init(CollisionShape* shape, float mass = 1.f, const glm::vec3 localInertia = glm::vec3(1));
	virtual void UpdateParent();
	virtual void UpdateRigidBody();
	virtual void ApplyForce();
	virtual void ApplyTorque();

	btRigidBody* Get() { return m_rigidBody; }
};
