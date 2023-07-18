#pragma once

#include <filesystem>	// c++ 17�̻� ����

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

	// ISubSystem��(��) ���� ��ӵ�
	virtual bool Initialize() override;
	virtual void Update() override;

	// ==============================================
	//					Resource
	// ==============================================

	template <typename T>
	const std::shared_ptr<T> Load(const std::string& path);

	// ���ҽ��� ã�� �Լ�
	template <typename T>
	const std::shared_ptr<T> GetResourceFromName(const std::string& name);

	template <typename T>
	const std::shared_ptr<T> GetResourceFromPath(const std::string& path);

	// ���ҽ� ���
	template <typename T>
	void RegisterResource(const std::shared_ptr<T>& resource);

	// Ư�� ��ο� Ÿ���� ���� ������ �����ϴ��� Ȯ���ϴ� �Լ�
	bool HasResource(const std::string& resource_name, const ResourceType& resource_type);

	// ==============================================
	//				    Directory
	// ==============================================

	const std::string GetAssetDirectory() const { return "Assets/"; }	// ��� ������ ���

	const std::string GetAssetDirectory(const AssetType& type);			// �ش� Ÿ�Կ� ���� ������ ��� 

private:
	void RegisterDirectory(const AssetType& type, const std::string& directory);

private:
	using resource_group_t = std::vector<std::shared_ptr<IResource>>;
	std::map<ResourceType, resource_group_t> resource_groups;			// ���ҽ� ������

	std::mutex resource_mutex;											// ������ ���� ����
	std::map<AssetType, std::string> asset_directories;					// ������ ���
};

template<typename T>
inline const std::shared_ptr<T> ResourceManager::Load(const std::string& path)
{
	static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");
	
	// �ش� ������ �������� ���� ��� ���� ó��
	if (!std::filesystem::exists(path))
	{
		assert(false);
		return nullptr;
	}

	// Asset/Animation/Idle.xml -> Idle.xml
	auto last_index = path.find_last_of("\\/");						// /�Ǵ� \�� �ִ� ��ġ�� ��ȯ��
	auto file_name = path.substr(last_index + 1, path.length());	// /�Ǵ� \�� ��ġ �������� ���ڿ��� ������

	// Idle.xml -> Idle
	last_index = file_name.find_last_of('.');						// .�� �ִ� ��ġ�� ��ȯ��
	auto resource_name = file_name.substr(0, last_index);			// . ��ġ �������� ���ڿ��� ������

	// �ش� �̸��� ������ �̹� ���� ��� �ش� �����͸� ��ȯ����
	if (HasResource(resource_name, IResource::DeduceResourceType<T>()))
		return GetResourceFromName<T>(resource_name);

	// �ش� �̸��� ������ ���� ��� �ش� �����͸� �������
	std::shared_ptr<T> resource = std::make_shared<T>(context);
	resource->SetResourceName(resource_name);
	resource->SetResourcePath(path);

	// �ش� ��ο� ���� �ִ��� Ȯ���Ͽ� ������ ����
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

	assert(false && "ERROR (ResourceManager2) : �ش� �̸��� ��������Ʈ ��ü�� �������� ����");
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

		else assert(false && "ERROR (ResourceManager2) : �ش� �̸��� ��������Ʈ ��ü�� �������� ����");
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