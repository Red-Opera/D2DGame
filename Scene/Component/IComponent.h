// IComponent.h
// Actor�� �����ؾ� �� ������ ��� ���� (��� : Actor, ��ü ��ġ, ComponentŸ�� �����, Ȱ��ȭ ���)

#pragma once

// �ش� Ŭ������ � �ڽ�Ŭ�������� �Ǵ��ϴ� enum��
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
	class Context* context = nullptr;						// Component�� �����ϴ� Manager�� �����ϴ� ����

	class Actor* actor = nullptr;							// Component�� ���� �ִ� ��ü
	class TransformComponent* transform = nullptr;			// Component�� ��ġ
	bool is_enabled = true;									// Ȱ��ȭ ����

	ComponentType component_type = ComponentType::Unknown;	// Component�� � ������ ����� �����ϴ� ����
};