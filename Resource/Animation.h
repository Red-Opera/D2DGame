// Animation.h
// XML파일로부터 애니메이션 스프라이트를 가져오는 파일

#pragma once
#include "IResource.h"

// 애니메이션을 한 번만 재생할 것인지 반복할 것인지 정하는 Enum
enum class RepeatType : uint
{
	Once, Loop
};

// 연속 동작 프레임 중 하나를 의미하는 구조체
struct Keyframe final
{
	Keyframe() = default;
	Keyframe(const D3DXVECTOR2& offset, const D3DXVECTOR2& size, const double& time) :offset(offset), size(size), time(time) {}

	D3DXVECTOR2 offset{ 0.0f, 0.0f };	// 텍스쳐를 가져올 시작 픽셀 위치
	D3DXVECTOR2 size{ 0.0f, 0.0f };		// 텍스쳐에 가져올 픽셀의 크기
	double time{ 0.0f };				// 해당 프레임을 보여줄 시간
};

class Animation final : public IResource
{
public:
	Animation(class Context* const context);
	~Animation();

	virtual bool SaveToFile(const std::string& path) override;		// 애니메이션을 파일로 저장하는 함수
	virtual bool LoadFromFile(const std::string& path) override;	// 만들어진 애니메이션을 불려오는 함수
	
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

	// XML로 받아온 모든 애니메이션의 keyframe
	std::unordered_map<std::string, std::vector<Keyframe>> textureKeyFrames;	// 애니메이션 스프라이트 객체의 이름을 키값으로 하여 키프레임을 가져옴
	using keyframeTypeName = std::unordered_map<std::string, std::vector<Keyframe>>;		// textureKeyFrame의 타입 이름

	// 하나 동작을 의미하는 스프라이트 객체의 정보를 담는 변수
	std::vector<RepeatType> repeatType;		// 애니메이션 스프라이트 객체 각각의 반복타입
	std::vector<std::string> texturePath;	// 애니메이션 스프라이트 객체 각각의 경로
	std::vector<D3DXVECTOR2> textureSize;	// 애니메이션 스프라이트 객체 각각의 사이즈
};