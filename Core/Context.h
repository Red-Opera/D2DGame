// Context.h
// ISubSystem�� ��ӹ��� ��ü���� �迭�� �����ϰ� �����ϴ� ���� (���, �˻�, Update, ã��)

#pragma once

#include "stdafx.h"
#include "Core/SubSystem/ISubSystem.h"
#include "Core/SubSystem/Renderer/Renderer.h"

class Context final
{
public:
	Context() = default;
	~Context() 
	{ 
		for (auto iter = subSystems.rbegin(); iter != subSystems.rend(); iter++)
			SAFE_DELETE(*iter);
	}

	// subSystems�� �ִ� ��� ��ü�� Initialize �Լ��� ȣ����
	const bool InitializeSubSystems()
	{
		bool result = true;

		for (const auto& subSystem : subSystems)
		{
			if (!subSystem->Initialize()) // == result &= subSystem->Initialize();
				result = false;
		}

		return result;
	}

	// �ش� ��ü�� subSystems�� �����ϴ� �Լ�
	template<typename T>
	const T* const RegisterSubSystem() 
	{
		static_assert(std::is_base_of<ISubSystem, T>::value, "Procided type does not implement ISubSystem");

		subSystems.emplace_back(new T(this));

		return static_cast<T*>(subSystems.back());
	}

	// Ÿ���� �Է��Ͽ� Ÿ�� ID������ �ش� ��ü�� �������� �Լ�q
	template<typename T>
	T* const GetSubSystem() 
	{
		for (const auto& subSystem : subSystems)
		{
			if (typeid(T) == typeid(*subSystem))
				return static_cast<T*>(subSystem);
		}

		return nullptr;
	}

	// subSystems�� �ִ� ��� ��ü�� Update �Լ��� ȣ����
	void UpdateSubSystems() 
	{
		for (const auto& subSystem : subSystems)
			if(typeid(*subSystem) != typeid(Renderer))
				subSystem->Update();
	}

private:
	std::vector<ISubSystem*> subSystems;	// ISubSystem�� ��ӹ��� �������� �����ϱ� ���� Vector
};