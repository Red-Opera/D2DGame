// AnimatorComponent.h
// �ִϸ��̼��� ��ü�� ����ϰ� �����ϴ� ����

#pragma once
#include "../IComponent.h"

// �ִϸ��̼� ���� ���� Enum
enum class AnimationMode : uint
{
	Play, Stop, Pause
};

class AnimatorComponent final : public IComponent
{
public:
	AnimatorComponent(class Context* const context, class Actor* const actor, class TransformComponent* const transform);
	~AnimatorComponent() = default;

	// IComponent��(��) ���� ��ӵ�
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
	class Timer* timer = nullptr;										// ������ �ð� ������ ���� ����

	AnimationMode animation_mode = AnimationMode::Play;
	uint current_frame_number = 0;										// ���� ������ ��ȣ
	float frame_counter = 0.0f;											// ���� �������� �󸶳� ���������� �˷��ִ� ����

	std::weak_ptr<class Animation> current_animation;					// �ִϸ��̼� ��������Ʈ �� ���� �ִϸ��̼� ��������Ʈ
	std::vector<std::string> keyFrameName;								// ��ü�� ����Ǿ� �ִ� �ִϸ��̼� ��������Ʈ �̸�
	std::string nowkeyFrameName;										// ���� ����� ��������Ʈ

	std::map<std::string, std::shared_ptr<class Animation>> animations;	// �ִϸ��̼ǿ� ���Ͽ� �̸��� ������ ����
};