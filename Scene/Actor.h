// Actor.h
// IComponent클래스를 구현한 클래스 (기능 : 이름, 활성화 여부, 위치(Transform) 반환, Component관리)

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
	// [Property] : 값을 얻거나 반환해주는 기능
	// ======================================================

	// 이름관리
	const std::string& GetName() const { return name; }
	void SetName(const std::string& name) { this->name = name; }

	// 활성화 여부
	bool IsActive() const { return is_active; }
	void SetActive(const bool& is_active) { this->is_active = is_active; }

	const std::shared_ptr<class TransformComponent>& GetTransform() const { return transform; }	// 스마트 포인터의 transform 반환
	class TransformComponent* GetTransform_Raw() const { return transform.get(); }				// dull 포인터의 transform 반환

	// ======================================================
	// [Component] : 추가 및 제거하는 기능
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
	Context* context = nullptr;								// Actor를 관리하는 Manager를 저장하는 변수

	std::string name;										// 객체 이름
	std::shared_ptr<class TransformComponent> transform;	// 객체의 transform은 많이 사용(크기변경, 위치이동)하므로 따로 변수 정의
	std::vector<std::shared_ptr<IComponent>> components;	// 객체의 Component를 담을 변수 (다른곳에서 많이 사용하기 때문에 shared_ptr 사용)
	bool is_active = true;									// 객체 활성화 여부

	bool isMove = false;									// 캐릭터가 현재 이동 중인지 여부
	Direction direction = Direction::Back;					// 캐릭터가 바라보고 있는 방향

	class Scene* scene;						// 객체가 존재하고 있는 Scene
};

template<typename T>
inline const std::shared_ptr<T> Actor::AddComponent()	// int * const value; 와 같은 기능
{
	/*
		is_base_of
			- 왼쪽 타입이 오른쪽 타입의 부모인지 확인하는 구조체
			- value값을 이용하여 부모라면 true, 아니라면 false를 반환한다.
	*/

	// IComponent타입을 상속 받지 않는 클래스(Component가 아닌 클래스) 일 수도 있기 때문에 확인
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent"); 

	ComponentType type = IComponent::DeduceComponentType<T>();	// Component의 타입을 받아 옴

	// 해당 Component를 가진 Component가 있으면 생성하지 않고 기존에 있던 객체를 반환 (같은 종류의 Component를 두 개 이상 갖지 않음)
	if (HasComponent(type) == true)
		return GetComponent<T>();

	// 해당 타입의 Component를 넣어줌
	components.emplace_back(std::make_shared<T>(context, this, transform.get()));

	// ===================================================================
	// 저장한 Component를 꺼내어 위치를 저장하고 세팅하기 위해 Componet를 반환
	// ===================================================================

	std::shared_ptr<T> new_component = std::static_pointer_cast<T>(components.back());

	// Component를 세팅함
	new_component->Initialize();
	new_component->SetComponentType(type);

	if constexpr (std::is_same<T, class TransformComponent>::value == true)
		transform = new_component;

	return new_component;
}

// 템플릿을 입력 받아 Icomponent타입의 component에서 해당 타입을 가지질 수 있는 component를 다운캐스팅하여 반환해주는 함수
template<typename T>
inline T* Actor::GetComponent_Raw()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent");

	ComponentType type = IComponent::DeduceComponentType<T>();
	for (const auto& component : components)
	{
		// IComponent타입인 component에서 템플릿에 입력한 형을 찾아 해당 타입으로 다운캐스팅하여 반환함
		if (component->GetComponentType() == type)
			return std::static_pointer_cast<T>(component).get();
	}

	return nullptr;
}

// 스마트 포인터가 아닌 dull 포인터 형태로 반환
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

// 일치하는 형식이 맞는 Component를 Vector로 반환하는 함수
template<typename T>
inline const std::vector<std::shared_ptr<T>> Actor::GetComponents()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent");
	
	ComponentType type = IComponent::DeduceComponentType<T>();

	std::vector<std::shared_ptr<T>> temp_components;	// 해당 타입으로 반환할 수 있는 Component를 모두 담을 수 있는 변수
	for (const auto& component : components)
	{
		if (component->GetComponentType() == type)
			temp_components.emplace_back(std::static_pointer_cast<T>(component));
	}

	return temp_components;
}

// 해당 타입으로 변환 가능한지 확인하는 함수 (이미 특수화된 해당 함수가 있음)
template<typename T>
inline bool Actor::HasCompoent()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent");

	return HasComponent(IComponent::DeduceComponentType<T>());
}

// 해당 타입의 Component를 제거
template<typename T>
inline void Actor::RemoveComponent()
{
	static_assert(std::is_base_of<IComponent, T>::value, "Provider type does no implement IComponent");

	ComponentType type = IComponent::DeduceComponentType<T>();
	for (auto iter = components.begin; iter != components.end();)	// erase함수를 통해서 제거하기 위해 iterator를 사용
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