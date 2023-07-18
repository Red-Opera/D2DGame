#pragma once

#include <filesystem>	// c++ 17이상만 지원

#include "ISubSystem.h"
#include "Resource/IResource.h"

enum class AssetType : uint
{
	Texture,
	Shader,
	Animation
};

class ResourceManager final : public ISubSystem
{
public:
	ResourceManager(class Context* const context);
	~ResourceManager() = default;

	// ISubSystem을(를) 통해 상속됨
	virtual bool Initialize() override;
	virtual void Update() override;

	// ==============================================
	//					Resource
	// ==============================================

	template <typename T>
	const std::shared_ptr<T> Load(const std::string& path);

	// 리소스를 찾는 함수
	template <typename T>
	const std::shared_ptr<T> GetResourceFromName(const std::string& name);

	template <typename T>
	const std::shared_ptr<T> GetResourceFromPath(const std::string& path);

	// 리소스 등록
	template <typename T>
	void RegisterResource(const std::shared_ptr<T>& resource);

	// 특정 경로와 타입을 가진 파일이 존재하는지 확인하는 함수
	bool HasResource(const std::string& resource_name, const ResourceType& resource_type);

	// ==============================================
	//				    Directory
	// ==============================================

	const std::string GetAssetDirectory() const { return "Assets/"; }	// 모든 에셋의 경로

	const std::string GetAssetDirectory(const AssetType& type);			// 해당 타입에 대한 에셋의 경로 

private:
	void RegisterDirectory(const AssetType& type, const std::string& directory);

private:
	using resource_group_t = std::vector<std::shared_ptr<IResource>>;
	std::map<ResourceType, resource_group_t> resource_groups;			// 리소스 데이터

	std::mutex resource_mutex;											// 스레드 관련 변수
	std::map<AssetType, std::string> asset_directories;					// 에셋의 경로
};

template<typename T>
inline const std::shared_ptr<T> ResourceManager::Load(const std::string& path)
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");
	
	// 해당 파일이 존재하지 않은 경우 예외 처리
	if (!std::filesystem::exists(path))
	{
		assert(false);
		return nullptr;
	}

	// Asset/Animation/Idle.xml -> Idle.xml
	auto last_index = path.find_last_of("\\/");						// /또는 \가 있는 위치를 반환함
	auto file_name = path.substr(last_index + 1, path.length());	// /또는 \의 위치 다음부터 문자열을 가져옴

	// Idle.xml -> Idle
	last_index = file_name.find_last_of('.');						// .가 있는 위치를 반환함
	auto resource_name = file_name.substr(0, last_index);			// . 위치 다음부터 문자열을 가져옴

	// 해당 이름의 파일이 이미 있을 경우 해당 데이터를 반환해줌
	if (HasResource(resource_name, IResource::DeduceResourceType<T>()))
		return GetResourceFromName<T>(resource_name);

	// 해당 이름의 파일이 없을 경우 해당 데이터를 만들어줌
	std::shared_ptr<T> resource = std::make_shared<T>(context);
	resource->SetResourceName(resource_name);
	resource->SetResourcePath(path);

	// 해당 경로에 파일 있는지 확인하여 없으면 예외
	if (!resource->LoadFromFile(path))
	{
		assert(false);
		return nullptr;
	}

	RegisterResource<T>(resource);

	return resource;
}

template<typename T>
inline const std::shared_ptr<T> ResourceManager::GetResourceFromName(const std::string& name)
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	for (const auto& resource : resource_groups[IResource::DeduceResourceType<T>()])
	{
		auto resVector = resource->GetResourceName();

		if (std::find(resVector.begin(), resVector.end(), name) != resVector.end())
			return std::static_pointer_cast<T>(resource);
	}

	assert(false && "ERROR (ResourceManager2) : 해당 이름의 스프라이트 객체가 존재하지 않음");
	return nullptr;
}

template<typename T>
inline const std::shared_ptr<T> ResourceManager::GetResourceFromPath(const std::string& path)
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	for (const auto& resource : resource_groups[IResource::DeduceResourceType<T>()])
	{
		auto resVector = resource->GetResourcePath();

		if (std::find(resVector.begin(), resVector.end(), path) != resVector.end())
			return std::static_pointer_cast<T>(resource);

		else assert(false && "ERROR (ResourceManager2) : 해당 이름의 스프라이트 객체가 존재하지 않음");
	}

	return nullptr;
}

template<typename T>
inline void ResourceManager::RegisterResource(const std::shared_ptr<T>& resource)
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

	if (resource == nullptr)
	{
		assert(false);
		return;
	}

	resource_mutex.lock();
	resource_groups[resource->GetResourceType()].emplace_back(resource);
	resource_mutex.unlock();
}