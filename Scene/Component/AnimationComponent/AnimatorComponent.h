// AnimatorComponent.h
// 애니메이션을 객체가 사용하게 가공하는 파일

#pragma once
#include "../IComponent.h"

// 애니메이션 실행 여부 Enum
enum class AnimationMode : uint
{
	Play, Stop, Pause
};

class AnimatorComponent final : public IComponent
{
public:
	AnimatorComponent(class Context* const context, class Actor* const actor, class TransformComponent* const transform);
	~AnimatorComponent() = default;

	// IComponent을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

	const AnimationMode& GetAnimationMode() const { return animation_mode; }
	void SetAnimationMode(const AnimationMode& mode) { this->animation_mode = mode; }

	const uint& GetCurerntFrameNumber() const { return current_frame_number; }
	void SetCurrentFrameNumber(const uint& number) { this->current_frame_number = number; }

	const std::shared_ptr<class Animation> GetCurrentAnimation() const;
	void SetCurrentAnimation(const std::string& animation_name);

	const std::map<std::string, std::shared_ptr<class Animation>>& GetAnimations() const { return animations; }
	const Keyframe* const GetCurrentKeyframe() const;

	void AddAnimation(const std::vector<std::string>& animation_name, const std::shared_ptr<class Animation>& animation);
	void AddAnimation(const std::string& path);

	void SetAnimationFrameName(const int count, char const* const ...);
	std::vector<std::string> GetAnimationFrameNames() const { return keyFrameName; }

	std::string GetNowKeyFrameName() { return nowkeyFrameName; }

	void Play();
	void Stop();
	void Pause();

	bool IsPlaying() const { return animation_mode == AnimationMode::Play; }

private:
	class Timer* timer = nullptr;										// 프레임 시간 측정을 위한 변수

	AnimationMode animation_mode = AnimationMode::Play;
	uint current_frame_number = 0;										// 현재 프레임 번호
	float frame_counter = 0.0f;											// 현재 프레임이 얼마나 보여졌는지 알려주는 변수

	std::weak_ptr<class Animation> current_animation;					// 애니메이션 스프라이트 중 현재 애니메이션 스프라이트
	std::vector<std::string> keyFrameName;								// 객체에 저장되어 있는 애니메이션 스프라이트 이름
	std::string nowkeyFrameName;										// 현재 실행될 스프라이트

	std::map<std::string, std::shared_ptr<class Animation>> animations;	// 애니메이션에 대하여 이름을 맵으로 정의
};