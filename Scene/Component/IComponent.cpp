#include "stdafx.h"
#include "IComponent.h"
#include "CameraComponent.h"
#include "MeshRendererComponent.h"

#include "MoveComponent/TransformComponent.h"
#include "MoveComponent/MoveScriptComponent.h"
#include "MoveComponent/AIScriptComponent.h"
#include "MoveComponent/TraceComponent.h"
#include "MoveComponent/CollisionComponent.h"
#include "MoveComponent/TeleportComponent.h"

#include "AnimationComponent/AnimatorComponent.h"
#include "AnimationComponent/BackGroundManager.h"
#include "AnimationComponent/GaugeBarComponent.h"

#include "AttackComponent/MonsterAttackComponent.h"
#include "AttackComponent/BulletComponent.h"
#include "AttackComponent/PlayerAttackComponent.h"
#include "AttackComponent/MultipleAttackTypesComponent.h"
#include "AttackComponent/PlayerSkillComponent.h"

#define REGISTER_COMPONENT_TYPE(T, enum_type) template<> ComponentType IComponent::DeduceComponentType<T>() { return enum_type; }

IComponent::IComponent(class Context* const context, Actor* const actor, TransformComponent* const transform) : context(context), actor(actor), transform(transform)
{
}

template<typename T>
constexpr ComponentType IComponent::DeduceComponentType()
{
	return ComponentType::Unknown;                                                                        
}

// Transform�� ���� DeduceComponentType() Ư��ȭ
REGISTER_COMPONENT_TYPE(TransformComponent, ComponentType::Transform);						// ��ġ���� Component ���
REGISTER_COMPONENT_TYPE(CameraComponent, ComponentType::Camera);							// ī�޶� Component ���
REGISTER_COMPONENT_TYPE(MeshRendererComponent, ComponentType::MeshRenderer);				// ������ Component ���
REGISTER_COMPONENT_TYPE(MoveScriptComponent, ComponentType::MoveScript);					// �̵����� Component ���
REGISTER_COMPONENT_TYPE(AIScriptComponent, ComponentType::AIScript);						// AI���� Component ���
REGISTER_COMPONENT_TYPE(TraceComponent, ComponentType::Trace);								// �������� Component ���
REGISTER_COMPONENT_TYPE(AnimatorComponent, ComponentType::Animator);						// �ִϸ��̼ǰ��� Component ���
REGISTER_COMPONENT_TYPE(BackGroundComponent, ComponentType::BackGround);					// ������ Component ���
REGISTER_COMPONENT_TYPE(CollisionComponent, ComponentType::Collision);						// �浹���� Component ���
REGISTER_COMPONENT_TYPE(GaugeBarComponent, ComponentType::GaugeBar);						// �������ٰ��� Component ���
REGISTER_COMPONENT_TYPE(MonsterAttackComponent, ComponentType::MonsterAttack);				// ���Ͱ��ݰ��� Component ���
REGISTER_COMPONENT_TYPE(BulletComponent, ComponentType::Bullet);							// �Ѿ˰��� Component ���
REGISTER_COMPONENT_TYPE(PlayerAttackComponent, ComponentType::PlayerAttack);				// �÷��̾���ݰ��� Component ���
REGISTER_COMPONENT_TYPE(MultipleAttackTypesComponent, ComponentType::MultipleAttackTypes);	// �پ��Ѱ��ݰ��� Component ���
REGISTER_COMPONENT_TYPE(PlayerSkillComponent, ComponentType::PlayerSkill);					// �÷��̾ų���� Component ���
REGISTER_COMPONENT_TYPE(TeleportComponent, ComponentType::Teleport);						// �ڷ���Ʈ���� Component ���