// Actor.h
// IComponentŬ������ ������ Ŭ���� (��� : �̸�, Ȱ��ȭ ����, ��ġ(Transform) ��ȯ, Component����)

#pragma once
#include "Component/IComponent.h"

enum class Direction
{
	Front,
	Back,
	Left,
	Right
};

class Actor final
{
public:
	Actor(Context* const context);
	~Actor();

	void Initialize();
	void Update();
	void Destroy();

	// ======================================================
	// [Property] : ���� ��ų� ��ȯ���ִ� ���
	// ======================================================

	// �̸�����
	const std::string& GetName() const { return name; }
	void SetName(const std::string& name) { this->name = name; }

	// Ȱ��ȭ ����
	bool IsActive() const { return is_active; }
	void SetActive(const bool& is_active) { this->is_active = is_active; }

	const std::shared_ptr<class TransformComponent>& GetTransform() const { return transform; }	// ����Ʈ �������� transform ��ȯ
	class TransformComponent* GetTransform_Raw() const { return transform.get(); }				// dull �������� transform ��ȯ

	// ======================================================
	// [Component] : �߰� �� �����ϴ� ���
	// ======================================================

	template<typename T>
	const std::shared_ptr<T> AddComponent();

	template<typename T>
	T* GetComponent_Raw();

	template<typename T>
	const std::shared_ptr<T> GetComponent();

	template<typename T>
	const std::vector<std::shared_ptr<T>> GetComponents();

	const std::vector<std::shared_ptr<IComponent>>& GetAllComponents() const { return components; }

	bool HasComponent(const ComponentType& type);

	template<typename T>
	bool HasCompoent();

	template<typename T>
	void RemoveComponent();

	const void SetDirection(const Direction& direction) { this->direction = direction; }
	const Direction& GetDirection() const { return direction; }

	const void SetScene(class Scene* scene) { this->scene = scene; }
	class Scene* GetScene() const { return scene; }

private:
	Context* context = nullptr;								// Actor�� �����ϴ� Manager�� �����ϴ� ����

	std::string name;										// ��ü �̸�
	std::shared_ptr<class TransformComponent> transform;	// ��ü�� transform�� ���� ���(ũ�⺯��, ��ġ�̵�)�ϹǷ� ���� ���� ����
	std::vector<std::shared_ptr<IComponent>> components;	// ��ü�� Component�� ���� ���� (�ٸ������� ���� ����ϱ� ������ shared_ptr ���)
	bool is_active = true;									// ��ü Ȱ��ȭ ����

	bool isMove = false;									// ĳ���Ͱ� ���� �̵� ������ ����
	Direction direction = Direction::Back;					// ĳ���Ͱ� �ٶ󺸰� �ִ� ����

	class Scene* scene;						// ��ü�� �����ϰ� �ִ� Scene
};

template<typename T>
inline const std::shared_ptr<T> Actor::AddComponent()	// int * const value; �� ���� ���
{
	/*
		is_base_of
			- ���� Ÿ���� ������ Ÿ���� �θ����� Ȯ���ϴ� ����ü
			- value���� �̿��Ͽ� �θ��� true, �ƴ϶�� false�� ��ȯ�Ѵ�.
	*/

	// IComponentŸ���� ��� ���� �ʴ� Ŭ����(Component�� �ƴ� Ŭ����) �� ���� �ֱ� ������ Ȯ��
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent"); 

	ComponentType type = IComponent::DeduceComponentType<T>();	// Component�� Ÿ���� �޾� ��

	// �ش� Component�� ���� Component�� ������ �������� �ʰ� ������ �ִ� ��ü�� ��ȯ (���� ������ Component�� �� �� �̻� ���� ����)
	if (HasComponent(type) == true)
		return GetComponent<T>();

	// �ش� Ÿ���� Component�� �־���
	components.emplace_back(std::make_shared<T>(context, this, transform.get()));

	// ===================================================================
	// ������ Component�� ������ ��ġ�� �����ϰ� �����ϱ� ���� Componet�� ��ȯ
	// ===================================================================

	std::shared_ptr<T> new_component = std::static_pointer_cast<T>(components.back());

	// Component�� ������
	new_component->Initialize();
	new_component->SetComponentType(type);

	if constexpr (std::is_same<T, class TransformComponent>::value == true)
		transform = new_component;

	return new_component;
}

// ���ø��� �Է� �޾� IcomponentŸ���� component���� �ش� Ÿ���� ������ �� �ִ� component�� �ٿ�ĳ�����Ͽ� ��ȯ���ִ� �Լ�
template<typename T>
inline T* Actor::GetComponent_Raw()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent");

	ComponentType type = IComponent::DeduceComponentType<T>();
	for (const auto& component : components)
	{
		// IComponentŸ���� component���� ���ø��� �Է��� ���� ã�� �ش� Ÿ������ �ٿ�ĳ�����Ͽ� ��ȯ��
		if (component->GetComponentType() == type)
			return std::static_pointer_cast<T>(component).get();
	}

	return nullptr;
}

// ����Ʈ �����Ͱ� �ƴ� dull ������ ���·� ��ȯ
template<typename T>
inline const std::shared_ptr<T> Actor::GetComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent");

	ComponentType type = IComponent::DeduceComponentType<T>();
	for (const auto& component : components)
	{
		if (component->GetComponentType() == type)
			return std::static_pointer_cast<T>(component);
	}

	return nullptr;
}

// ��ġ�ϴ� ������ �´� Component�� Vector�� ��ȯ�ϴ� �Լ�
template<typename T>
inline const std::vector<std::shared_ptr<T>> Actor::GetComponents()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent");
	
	ComponentType type = IComponent::DeduceComponentType<T>();

	std::vector<std::shared_ptr<T>> temp_components;	// �ش� Ÿ������ ��ȯ�� �� �ִ� Component�� ��� ���� �� �ִ� ����
	for (const auto& component : components)
	{
		if (component->GetComponentType() == type)
			temp_components.emplace_back(std::static_pointer_cast<T>(component));
	}

	return temp_components;
}

// �ش� Ÿ������ ��ȯ �������� Ȯ���ϴ� �Լ� (�̹� Ư��ȭ�� �ش� �Լ��� ����)
template<typename T>
inline bool Actor::HasCompoent()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent");

	return HasComponent(IComponent::DeduceComponentType<T>());
}

// �ش� Ÿ���� Component�� ����
template<typename T>
inline void Actor::RemoveComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent");

	ComponentType type = IComponent::DeduceComponentType<T>();
	for (auto iter = components.begin; iter != components.end();)	// erase�Լ��� ���ؼ� �����ϱ� ���� iterator�� ���
	{
		std::shared_ptr<T> component = *iter;

		if (component->GetComponentType() == type)
		{
			component->Destroy();

			iter = components.erase(iter);
		}

		else
			iter++;
	}
}