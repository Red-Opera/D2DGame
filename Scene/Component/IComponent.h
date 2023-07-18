// IComponent.h
// Actor를 구성해야 할 정보를 담는 변수 (기능 : Actor, 객체 위치, Component타입 입출력, 활성화 기능)

#pragma once

// 해당 클래스가 어떤 자식클래스인지 판단하는 enum값
enum class ComponentType : uint
{
	Unknown, Camera, Transform, MeshRenderer, MoveScript,
	AIScript, Trace, Animator, BackGround, Collision,
	GaugeBar, MonsterAttack, Bullet, PlayerAttack, MultipleAttackTypes,
	PlayerSkill
};

class IComponent
{
public:
	template<typename T>
	static constexpr ComponentType DeduceComponentType();

public:
	IComponent(class Context* const context, class Actor* const actor, class TransformComponent* const transform);
	virtual ~IComponent() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Destroy() = 0;

	class Actor* GetActor() const { return actor; }
	void SetActor(class Actor* const actor) { this->actor = actor; }

	class TransformComponent* GetTransform() const { return transform; }
	void SetTransform(class TransformComponent* const transform) { this->transform = transform; }
	
	ComponentType GetComponentType() const { return component_type; }
	void SetComponentType(const ComponentType& type) { this->component_type = type; }

	bool IsEnabled() const { return is_enabled; }
	void SetEnabled(const bool& is_enabled) { this->is_enabled = is_enabled; }

protected:
	class Context* context = nullptr;						// Component를 관리하는 Manager를 저장하는 변수

	class Actor* actor = nullptr;							// Component를 갖고 있는 객체
	class TransformComponent* transform = nullptr;			// Component의 위치
	bool is_enabled = true;									// 활성화 여부

	ComponentType component_type = ComponentType::Unknown;	// Component가 어떤 형식을 띄는지 저장하는 변수
};