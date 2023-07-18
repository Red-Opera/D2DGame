#include "stdafx.h"
#include "AnimatorComponent.h"

AnimatorComponent::AnimatorComponent(class Context* const context, class Actor* const actor, class TransformComponent* const transform) : IComponent(context, actor, transform)
{
    timer = context->GetSubSystem<Timer>(); // 생성시점부터 시간 측정
}

void AnimatorComponent::Initialize()
{
    
}

void AnimatorComponent::Update()
{
    if (current_animation.expired() || !IsPlaying()) return;

    frame_counter += timer->GetDeltaTimeMS();

    if (frame_counter > GetCurrentKeyframe()->time)
    {
        current_frame_number++;

        switch (current_animation.lock()->GetRepeatType(nowkeyFrameName))
        {
            case RepeatType::Once:
            {
                if (current_frame_number >= current_animation.lock()->GetKeyframeCount(nowkeyFrameName))
                {
                    current_frame_number = static_cast<uint>(current_animation.lock()->GetKeyframeCount(nowkeyFrameName) - 1);
                    Pause();
                }

                break;
            }

            case RepeatType::Loop:
            {
                current_frame_number %= current_animation.lock()->GetKeyframeCount(nowkeyFrameName);
                break;
            }
        }

        frame_counter = 0.0;
    }
}

void AnimatorComponent::Destroy()
{
}

const std::shared_ptr<class Animation> AnimatorComponent::GetCurrentAnimation() const
{
    return current_animation.expired() ? nullptr : current_animation.lock();
}

void AnimatorComponent::SetCurrentAnimation(const std::string& animation_name)
{
    if (animations.find(animation_name) == animations.end())
    {
        assert(false && "Error (AnimationCompoent2) : 해당 이름의 스프라이트 객체가 존재하지 않음");
        return;
    }

    nowkeyFrameName = animation_name;
    current_animation = animations[animation_name];

    current_frame_number = 0;
    frame_counter = 0.0f;
}

const Keyframe* const AnimatorComponent::GetCurrentKeyframe() const
{
    if (current_animation.expired())
    {
        assert(false);
        return nullptr;
    }

    return current_animation.lock()->GetKeyframeFromIndex(nowkeyFrameName, current_frame_number);
}

void AnimatorComponent::AddAnimation(const std::vector<std::string>& animation_name, const std::shared_ptr<class Animation>& animation)
{
    for (auto& orderName : animation_name)
    {
        // 이미 등록되어 있는 애니메이션은 추가하지 않음
        if (animations.find(orderName) != animations.end())
            continue;

        animations[orderName] = animation;
    }
}

void AnimatorComponent::AddAnimation(const std::string& path)
{
    auto resource_manager = context->GetSubSystem<ResourceManager>();
    auto animation = resource_manager->Load<Animation>(path);

    if (animation == nullptr)
    {
        assert(false);
        return;
    }

    AddAnimation(keyFrameName, animation);
}

void AnimatorComponent::SetAnimationFrameName(const int count, char const * const ...)
{
    va_list list;

    va_start(list, 4);
    for (int i = 0; i < count; i++)
    {
        char* data = va_arg(list, char*);

        keyFrameName.push_back(data);
    }
    va_end(list);

}

void AnimatorComponent::Play()
{
    animation_mode = AnimationMode::Play;
    frame_counter = 0.0f;
}

void AnimatorComponent::Stop()
{
    animation_mode = AnimationMode::Stop;
    current_frame_number = 0;
    frame_counter = 0.0f;
}

void AnimatorComponent::Pause()
{
    animation_mode = AnimationMode::Pause;
}