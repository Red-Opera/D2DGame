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

// Transform에 대한 DeduceComponentType() 특수화
REGISTER_COMPONENT_TYPE(TransformComponent, ComponentType::Transform);						// 위치관련 Component 등록
REGISTER_COMPONENT_TYPE(CameraComponent, ComponentType::Camera);							// 카메라 Component 등록
REGISTER_COMPONENT_TYPE(MeshRendererComponent, ComponentType::MeshRenderer);				// 랜더링 Component 등록
REGISTER_COMPONENT_TYPE(MoveScriptComponent, ComponentType::MoveScript);					// 이동관련 Component 등록
REGISTER_COMPONENT_TYPE(AIScriptComponent, ComponentType::AIScript);						// AI관련 Component 등록
REGISTER_COMPONENT_TYPE(TraceComponent, ComponentType::Trace);								// 추적관련 Component 등록
REGISTER_COMPONENT_TYPE(AnimatorComponent, ComponentType::Animator);						// 애니메이션관련 Component 등록
REGISTER_COMPONENT_TYPE(BackGroundComponent, ComponentType::BackGround);					// 배경관련 Component 등록
REGISTER_COMPONENT_TYPE(CollisionComponent, ComponentType::Collision);						// 충돌관련 Component 등록
REGISTER_COMPONENT_TYPE(GaugeBarComponent, ComponentType::GaugeBar);						// 게이지바관련 Component 등록
REGISTER_COMPONENT_TYPE(MonsterAttackComponent, ComponentType::MonsterAttack);				// 몬스터공격관련 Component 등록
REGISTER_COMPONENT_TYPE(BulletComponent, ComponentType::Bullet);							// 총알관련 Component 등록
REGISTER_COMPONENT_TYPE(PlayerAttackComponent, ComponentType::PlayerAttack);				// 플레이어공격관련 Component 등록
REGISTER_COMPONENT_TYPE(MultipleAttackTypesComponent, ComponentType::MultipleAttackTypes);	// 다양한공격관련 Component 등록
REGISTER_COMPONENT_TYPE(PlayerSkillComponent, ComponentType::PlayerSkill);					// 플레이어스킬관련 Component 등록
REGISTER_COMPONENT_TYPE(TeleportComponent, ComponentType::Teleport);						// 텔레포트관련 Component 등록