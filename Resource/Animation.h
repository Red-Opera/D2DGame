// Animation.h
// XML���Ϸκ��� �ִϸ��̼� ��������Ʈ�� �������� ����

#pragma once
#include "IResource.h"

// �ִϸ��̼��� �� ���� ����� ������ �ݺ��� ������ ���ϴ� Enum
enum class RepeatType : uint
{
	Once, Loop
};

// ���� ���� ������ �� �ϳ��� �ǹ��ϴ� ����ü
struct Keyframe final
{
	Keyframe() = default;
	Keyframe(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time) :offset(offset), size(size), time(time) {}

	D3DXVECTOR2 offset{ 0.0f, 0.0f };	// �ؽ��ĸ� ������ ���� �ȼ� ��ġ
	D3DXVECTOR2 size{ 0.0f, 0.0f };		// �ؽ��Ŀ� ������ �ȼ��� ũ��
	double time{ 0.0f };				// �ش� �������� ������ �ð�
};

class Animation final : public IResource
{
public:
	Animation(class Context* const context);
	~Animation();

	virtual bool SaveToFile(const std::string& path) override;		// �ִϸ��̼��� ���Ϸ� �����ϴ� �Լ�
	virtual bool LoadFromFile(const std::string& path) override;	// ������� �ִϸ��̼��� �ҷ����� �Լ�
	
	const RepeatType& GetRepeatType(const std::string& animationName) const;
	void SetRepeatType(const std::string animationName, const RepeatType& repeat_type);

	const D3DXVECTOR2& GetSpriteTextureSize(const std::string animationName) const;
	void SetSpriteTextureSize(const std::string animationName, const D3DXVECTOR2& size);

	const std::shared_ptr<class D3D11_Texture>& GetSpriteTexture() const { return sprite_texture; }
	void SetSpriteTexture(const std::shared_ptr<class D3D11_Texture>& sprite_texture) { this->sprite_texture = sprite_texture; }

	void SetSpriteTexture(const std::string animationName, const std::string& path);

	const std::string& GetSpriteTexturePath(const std::string& animationName) const;
	void SetSpriteTexturePath(const std::string& animationName, const std::string& path) ;

	const std::unordered_map<std::string, std::vector<Keyframe>>& GetKeyframes() const { return textureKeyFrames; }
	void SetKeyframes(const std::vector<Keyframe>& keyframes) {  } // TODO :

	void AddKeyframe(const std::string animationName, const Keyframe& keyframe);
	void AddKeyframe(const std::string animationName, const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time);

	const Keyframe* const GetKeyframeFromIndex(const std::string animationName, const uint& index);
	const size_t GetKeyframeCount(const std::string animationName) const;

private:
	std::shared_ptr<class D3D11_Texture> sprite_texture;

	// XML�� �޾ƿ� ��� �ִϸ��̼��� keyframe
	std::unordered_map<std::string, std::vector<Keyframe>> textureKeyFrames;	// �ִϸ��̼� ��������Ʈ ��ü�� �̸��� Ű������ �Ͽ� Ű�������� ������
	using keyframeTypeName = std::unordered_map<std::string, std::vector<Keyframe>>;		// textureKeyFrame�� Ÿ�� �̸�

	// �ϳ� ������ �ǹ��ϴ� ��������Ʈ ��ü�� ������ ��� ����
	std::vector<RepeatType> repeatType;		// �ִϸ��̼� ��������Ʈ ��ü ������ �ݺ�Ÿ��
	std::vector<std::string> texturePath;	// �ִϸ��̼� ��������Ʈ ��ü ������ ���
	std::vector<D3DXVECTOR2> textureSize;	// �ִϸ��̼� ��������Ʈ ��ü ������ ������
};