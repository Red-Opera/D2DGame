#pragma once

enum class ResourceType : uint
{
	Unknown,
	Animation,
	Texture2D,
	Mesh,
	Matrial
};

class IResource
{
public:
	template <typename T>
	static constexpr ResourceType DeduceResourceType();

public:
	IResource(class Context* const context, const ResourceType type = ResourceType::Unknown);
	virtual ~IResource() = default;

	virtual bool SaveToFile(const std::string& path) = 0;
	virtual bool LoadFromFile(const std::string& path) = 0;

	ResourceType GetResourceType() const { return resource_type; }
	const std::vector<std::string>& GetResourceName() const { return animationName; }
	std::string GetResourcePath() const { return resource_path; }

	void SetResourceName(const std::string& name) { this->resource_name = name; }
	void SetResourcePath(const std::string& path) { this->resource_path = path; }

protected:
	class Context* context = nullptr;
	ResourceType resource_type = ResourceType::Unknown;		// 리소스의 타입
	std::string resource_name = "";							// xml파일 경로
	std::string resource_path = "";							// xml 파일의 경로

	std::vector<std::string> animationName;					// Animation 스프라이트 이름
};