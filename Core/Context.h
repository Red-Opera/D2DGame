// Context.h
// ISubSystem을 상속받은 객체들을 배열로 저장하고 관리하는 파일 (등록, 검색, Update, 찾기)

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

	// subSystems에 있는 모든 객체의 Initialize 함수를 호출함
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

	// 해당 객체를 subSystems에 저장하는 함수
	template<typename T>
	const T* const RegisterSubSystem() 
	{
		static_assert(std::is_base_of<ISubSystem, T>::value, "Procided type does not implement ISubSystem");

		subSystems.emplace_back(new T(this));

		return static_cast<T*>(subSystems.back());
	}

	// 타입을 입력하여 타입 ID값으로 해당 객체를 가져오는 함수q
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

	// subSystems에 있는 모든 객체의 Update 함수를 호출함
	void UpdateSubSystems() 
	{
		for (const auto& subSystem : subSystems)
			if(typeid(*subSystem) != typeid(Renderer))
				subSystem->Update();
	}

private:
	std::vector<ISubSystem*> subSystems;	// ISubSystem을 상속받은 변수들을 저장하기 위한 Vector
};